#include "Scene.h"

/// Получить случайное число от min до max.
ValT Rnd(ValT min, ValT max)
{
    return min + (max - min) * rand() / RAND_MAX;
}
// Texture //////////////////////////////////////////////////////////
void Texture::InitGl()
{
    glFun.glGenTextures(1, &id);
}
void Texture::UpdateGl()
{
    glFun.glBindTexture(GL_TEXTURE_2D, id);
    //glFun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); //Фильтрация текстур и MipMap.
    glFun.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFun.glTexImage2D( GL_TEXTURE_2D,
        0, // уровень мипмапа - авто
        GL_RGB, // получаемый формат текстуры
        w, h, 0,
        GL_RGB, GL_UNSIGNED_BYTE, aCol.data() ); // формат данных исходного изображения, и оно само.
    glFun.glGenerateMipmap(GL_TEXTURE_2D);
}
Texture::~Texture()
{
    if (id) glFun.glDeleteTextures(1, &id);
}
// Mesh /////////////////////////////////////////////////////////////
Mesh::Mesh()
{
}
Mesh::~Mesh()
{
    if (vao == 0) return;
    glFun.glDeleteBuffers(1, &ib);
    glFun.glDeleteBuffers(1, &vb);
    glFun.glDeleteVertexArrays(1, &vao);
}
void Mesh::CreateGrid(size_t sgmX, size_t sgmY)
{   // Сетка создаётся из сегментов. В каждом сегменте 7 угловых вершин и 1 центральная.
    // У каждого сегмента 3 вершины сверху, 3 снизу.
    // У чётных сегментов (0, 2, ...) только правая вершина, у нечётных только левая.
    const ValT stepGrid = 1.0; // Общий шаг сетки.
    const ValT rndMax = stepGrid / 4.0, rndMin = -rndMax; // Разброз случайности.
    const IndT nVertLine = sgmX * 2 + 1;        // Число вершин в чётной линии (0, 2).
    const IndT nVertLine1 = ceil(sgmX * 1.5);   // Число вершин в нечётной линии (1, 3).
    // Вершины.
    aVert.resize( (nVertLine + nVertLine1) * sgmY + nVertLine );
    Vertex* vec = aVert.data();
    ValT posY = 0;
    for (uint16_t y = 0; y < sgmY * 2 + 1; ++y)
    {   ValT posX = 0;
        const bool bY2 = y % 2; // Нечётная линия по Y.
        for (uint16_t x = 0; x < nVertLine; ++x)
        {
            if (bY2 == 0 || x % 4)
            {
                vec->x = posX + Rnd(rndMin, rndMax);
                vec->y = posY + Rnd(rndMin, rndMax);
                vec->z = Rnd(rndMin, rndMax);
                vec->u = posX / (sgmX * stepGrid);
                vec->v = posY / (sgmY * stepGrid);
                ++vec;
            }
            posX += stepGrid / 2;
        }
        posY += stepGrid / 2;
    }
    // Индексы.
    aInd.resize(sgmX * sgmY * 7 * 3);
    size_t ind = 0;
    IndT uVertBeg = 0; // Текущая первая (нижняя) вершина сегмента.
    IndT uVertCenter = nVertLine; // Текущая центральная вершина сегмента.
    IndT uVertTop = uVertCenter + nVertLine1; // Текущая первая верхняя вершина сегмента.
    for (uint16_t y = 0; y < sgmY; ++y)
    {
        for (uint16_t x = 0; x < sgmX; ++x)
        {
            bool bX2 = x % 2; // Нечётный ли сегмент по Х.
            aInd[ind++] = uVertCenter;
            aInd[ind++] = uVertBeg;
            aInd[ind++] = uVertBeg + 1;

            aInd[ind++] = uVertCenter;
            aInd[ind++] = uVertBeg + 1;
            aInd[ind++] = uVertBeg + 2;

            aInd[ind++] = uVertCenter;
            aInd[ind++] = uVertBeg + 2;
            aInd[ind++] = bX2? uVertTop + 2: uVertCenter + 1;

            if (bX2 == 0)
            {   aInd[ind++] = uVertCenter;
                aInd[ind++] = uVertCenter + 1;
                aInd[ind++] = uVertTop + 2;
            }

            aInd[ind++] = uVertCenter;
            aInd[ind++] = uVertTop + 2;
            aInd[ind++] = uVertTop + 1;

            aInd[ind++] = uVertCenter;
            aInd[ind++] = uVertTop + 1;
            aInd[ind++] = uVertTop;

            if (bX2 == 0)
            {   aInd[ind++] = uVertCenter;
                aInd[ind++] = uVertTop;
                aInd[ind++] = uVertBeg;
            } else
            {   aInd[ind++] = uVertCenter;
                aInd[ind++] = uVertTop;
                aInd[ind++] = uVertCenter - 1;

                aInd[ind++] = uVertCenter;
                aInd[ind++] = uVertCenter - 1;
                aInd[ind++] = uVertBeg;
            }

            uVertBeg += 2;
            uVertCenter += bX2? 1 : 2;
            uVertTop += 2;
        }
        uVertBeg += nVertLine1 + 1;
        uVertCenter = uVertBeg + nVertLine;
        uVertTop = uVertCenter + nVertLine1;
    }
    // Текстура.
    tex.w = sgmX;
    tex.h = sgmY;
    tex.aCol.resize(sgmX * sgmY);
    Color* px = tex.aCol.data();
    for (uint16_t y = 0; y < sgmY; ++y)
    {   for (uint16_t x = 0; x < sgmX; ++x)
        {   *px = { (uint8_t)Rnd(0, 255),
                    (uint8_t)Rnd(0, 255),
                    (uint8_t)Rnd(0, 255) };
            ++px;
        }
    }
}
void Mesh::InitGl()
{   assert(vao == 0);
    glFun.glGenVertexArrays(1, &vao);
    glFun.glGenBuffers(1, &vb);
    glFun.glGenBuffers(1, &ib);
    tex.InitGl();
}
void Mesh::UpdateGl()
{   assert(vao);
    // Привязываем объект (VAO).
    glFun.glBindVertexArray(vao);

    // Копируем вершины в буфер OpenGL.
    glFun.glBindBuffer(GL_ARRAY_BUFFER, vb); // Привязываем буфер
    glFun.glBufferData(GL_ARRAY_BUFFER, aVert.size() * sizeof(Vertex), aVert.data(), GL_STATIC_DRAW);
    // - Настраиваем вершинные атрибуты.
    glFun.glEnableVertexAttribArray(0);
    glFun.glVertexAttribPointer(0, 3, ValTid, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    glFun.glEnableVertexAttribArray(1);
    glFun.glVertexAttribPointer(1, 2, ValTid, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vec3));

    // Копируем индексы в буфер OpenGL.
    glFun.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glFun.glBufferData(GL_ELEMENT_ARRAY_BUFFER, aInd.size() * sizeof(IndT), aInd.data(), GL_STATIC_DRAW);

    // Копируем текстуру.
    tex.UpdateGl();
}
void Mesh::Draw() const
{
    glFun.glBindVertexArray(vao);
    glFun.glBindBuffer(GL_ARRAY_BUFFER, vb);
    glFun.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glFun.glBindTexture(GL_TEXTURE_2D, tex.id);
    glFun.glDrawElements(GL_TRIANGLES, (GLsizei) aInd.size(), IndTid, 0);
}
