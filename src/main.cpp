#include <QApplication>
#include "errorsHandler.hpp"

#ifdef USE_QGLVIEWER
#include "viewer.h"
#else
#include "scenemanager.h"
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef USE_QGLVIEWER
    QGLFormat glFormat;
    glFormat.setVersion( 3, 3 );
    glFormat.setProfile( QGLFormat::CoreProfile ); // Requires >=Qt-4.8.0
    glFormat.setSampleBuffers( true );
    QGLFormat::setDefaultFormat(glFormat);

    Viewer v;
    v.show();
#else

    qglviewer_fake::Camera camera;
    SceneManager manager(camera, 0, 0, 0, 0);

    manager.setup();

    manager.phongRendering(6);


#endif



    return a.exec();

}
