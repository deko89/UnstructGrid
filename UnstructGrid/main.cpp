#include <cstdlib>
#include <ctime>
#include <QApplication>
#include "WndGl.h"

bool gIsGlInit; ///< Инициализирована ли OpenGl.
QOpenGLExtraFunctions glFun; ///< Функции OpenGl.

int main(int argc, char *argv[])
{

    std::srand( clock() );

    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true); // Включение общих ресурсов Gl между окнами.
    QApplication a(argc, argv);

    // Создание сцены.
    const size_t sgmX = 500, sgmY = 1000;
    //const size_t sgmX = 200, sgmY = 250;
    Scene scene;
    scene.CreateGrid(sgmX, sgmY);

    // Создание окна.
    WndGl w( nullptr, scene, QOpenGLContext::globalShareContext() );
    w.setPosition(0, 100);
    w.resize(1000, 2000);
    w.SetCam( QVector3D(sgmX / 2.0,   sgmY / 2.0 - 0.01,   sgmY * 0.9),
              QVector3D(sgmX / 2.0,   sgmY / 2.0,          0   ) );
    w.show();

    // Создание окна 2.
    Scene scene2;
    scene2.CreateGrid(sgmX, sgmY);
    WndGl w2( nullptr, scene2, QOpenGLContext::globalShareContext() );
    w2.setPosition(1000, 100);
    w2.resize(1000, 2000);
    w2.SetCam( QVector3D(sgmX / 2.0,   -2.0,  2.0),
               QVector3D(sgmX / 2.0,   3.0,   0) );
    w2.bLineMode = true;
    w2.show();

    return a.exec();
}
