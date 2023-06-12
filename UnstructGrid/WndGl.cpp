#include "WndGl.h"

WndGl::WndGl(QWindow* parent, Scene &scene, QOpenGLContext* shareContext) :
    QOpenGLWindow(shareContext, NoPartialUpdate, parent),
    scene(scene)
{
}

WndGl::~WndGl()
{
}

const GLchar* const pCodeVert = R"~(
#version 330 core
layout (location = 0) in vec3  pos;
layout (location = 1) in vec2  tex;
uniform highp mat4 matrix;

out vec2 posTex;

void main()
{	gl_Position = matrix * vec4(pos, 1.0);
    posTex = tex;
}
)~";

const GLchar* const pCodeFrag = R"~(
#version 330 core
uniform sampler2D tex;
in vec2 posTex;

out vec4 color;

void main()
{	color = texture(tex, posTex);
}
)~";

void WndGl::initializeGL()
{
    // Инициализация OpenGl.
    //if (!gIsGlInit)
    {
        glFun.initializeOpenGLFunctions();

        // Обновление данных OpenGL.
        scene.InitGl();
        scene.UpdateGl();

        // Опции.
        glFun.glEnable(GL_DEPTH_TEST); // Тест глубины.
        glFun.glEnable(GL_CULL_FACE); // Отсечение невидимых граней.
        if (bLineMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Включаем режим линий.

        gIsGlInit = true;
    }

    // Создаём шейдеры.
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, pCodeVert);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, pCodeFrag);
    m_program->link();
    m_program->bind();

    // Обновление камеры.
    int iMatrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(iMatrixUniform != -1);

    QMatrix4x4 matCam;
    float ratio = ( (ValT)width() ) / height();
    matCam.perspective(60.0f, ratio, 0.1f, 1000.0f);
    matCam.lookAt( camera.pos, camera.look, QVector3D(0, 0, 1) );

    m_program->setUniformValue(iMatrixUniform, matCam);
}

void WndGl::resizeGL(int w, int h)
{
    makeCurrent();
    glFun.glViewport(0, 0, w, h);
}

void WndGl::paintGL()
{
    m_program->bind();
    glFun.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.Draw();
}

void WndGl::SetCam(QVector3D pos, QVector3D look)
{
    camera.pos = pos;
    camera.look = look;
}

