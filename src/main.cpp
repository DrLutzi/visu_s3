#include <QApplication>

#ifdef USE_QGLVIEWER
#include "viewer.h"
#else
#include "scenemanager.h"
#endif
#include "errorsHandler.hpp"

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

    QGLVIEWER_NAMESPACE::Camera camera;
    SceneManager manager(camera, 0, 0, 0, 0);

    SceneFace_Prop *face1 = new SceneFace_Prop(glm::vec3(-5.0f,-3.0f,0), glm::vec3(1,0,0), glm::vec3(0,1,0), 10, 6);
    face1->setColor(glm::vec3(0.5, 0.5, 0.8));

    SceneFace_Prop *face2 = new SceneFace_Prop(glm::vec3(7.0f, -1.0f, 2.0f), glm::vec3(-0.5f, 0, -1.0f), glm::vec3(0,1,0), 8, 8);
    face2->setColor(glm::vec3(0.5,0.5,0.5));

    SceneFace_Light *faceLight = new SceneFace_Light(glm::vec3(-5.0f, -1.0f, 5.0f), glm::vec3(-0.1f, 0, -1.0f), glm::vec3(0,1,0), 2, 2);
    faceLight->setColor(glm::vec3(1.0, 1.0, 1.0));

    SceneFace_Light *faceLight2 = new SceneFace_Light(glm::vec3(-5.0f, -1.0f, 5.0f), glm::vec3(-0.1f, 0, -1.0f), glm::vec3(0,1,0), 2, 2);
    faceLight->setColor(glm::vec3(1.0, 1.0, 1.0));

    SceneFace_Prop::MaterialProperties_t matProperties;
    matProperties.fSpecularPower=100.0f;
    matProperties.vAmbiant=glm::vec3(0.1, 0.3, 0.1);
    matProperties.vDiffuse=glm::vec3(0.1, 0.4, 0.1);
    matProperties.vSpecular=glm::vec3(0.1, 0.8, 0.1);

    SceneFace_Light::LightProperties_t lightProperties;
    lightProperties.vAmbiant=glm::vec3(0.2, 0.2, 0.2);
    lightProperties.vDiffuse=glm::vec3(0.6, 0.6, 0.6);
    lightProperties.vSpecular=glm::vec3(1.0, 1.0, 1.0);

    face1->setMaterialProperties(matProperties);
    face2->setMaterialProperties(matProperties);
    faceLight->setLightProperties(lightProperties);

    manager.append(face1, false);
    manager.append(face2, false);
    manager.append(faceLight, false);

    manager.phongRendering(6);


#endif



    return a.exec();

}
