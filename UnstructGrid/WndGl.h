#ifndef WNDGL_H
#define WNDGL_H


#include <QVector3D>
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include "Scene.h"

/// Камера
class Camera
{   public:
    QVector3D pos, look; ///< Позиция и точка куда смотрит.
};

/// Окно с графикой.
class WndGl : public QOpenGLWindow
{
    Q_OBJECT
public:
    bool bLineMode = 0; ///< Режим рисования линиями.
    WndGl(QWindow* parent, Scene& scene, QOpenGLContext* shareContext = nullptr);
    ~WndGl();
    virtual void initializeGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void paintGL() override;
    /*! Установить позицию камеры.
        \param[in] pos Позиция камеры.
        \param[in] look Точка куда смотрит камера. */
    void SetCam(QVector3D pos, QVector3D look);
protected:
    Scene& scene;
    Camera camera;
    QOpenGLShaderProgram* m_program = nullptr; ///< Указатель на программу рисования.
};

#endif // WNDGL_H
