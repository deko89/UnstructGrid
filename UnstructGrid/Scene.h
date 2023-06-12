#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <QOpenGLExtraFunctions>

using std::vector;

extern bool gIsGlInit;
extern QOpenGLExtraFunctions glFun;

// Основные типы.
typedef GLfloat ValT;                   ///< Общий тип вещественных данных (напр. координат вершин).
const GLenum ValTid = GL_FLOAT;         ///< Код типа ValT (для OpenGL).
typedef GLuint IndT;                    ///< Тип индексов вершин (номеров).
const GLenum IndTid = GL_UNSIGNED_INT;  ///< Код типа индексов вершин (для OpenGL).

/// Векторы.
struct Vec3
{   ValT x, y, z;
};

// Вершина.
struct Vertex : public Vec3
{   ValT u, v;
};

/// Цвет.
struct Color
{   uint8_t r, g, b;
};

/// Текстура.
struct Texture
{   vector<Color> aCol; ///< Цвета.
    IndT w, h;          ///< Размер.
    GLuint id = 0;      ///< Текстура в видеокарте.
    void InitGl();      ///< Создать в видеокарте.
    void UpdateGl();    ///< Обновить в видеокарте.
    ~Texture();
};

/// Модель.
struct Mesh
{
    vector<Vertex> aVert;   ///< Вершины.
    vector<IndT> aInd;      ///< Индексы (порядок соединения вершин).
    Texture tex;
    GLuint vao = 0;         ///< Объект в видеокарте.
    GLuint vb, ib;          ///< Буфер вершин, и индексы порядка рисования в видеокарте.
    Mesh();
    ~Mesh();
    void CreateGrid(size_t sgmX, size_t sgmY);  ///< Создать сетку с числом ячеек sgmX, sgmY.
    void InitGl();      ///< Создать в видеокарте.
    void UpdateGl();    ///< Обновить в видеокарте.
    void Draw() const;  ///< Рисование.
};

typedef Mesh Scene;

#endif // SCENE_H
