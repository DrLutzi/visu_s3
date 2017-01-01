#ifndef SCENECAMERA_H
#define SCENECAMERA_H
#include <QImage>
#include <QLabel>
#include "ray.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include "dialog_renderedimage.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#ifdef USE_QGLVIEWER
#include <QGLViewer/qglviewer.h>
#define QGLVIEWER_NAMESPACE gqlviewer
#else

namespace qglviewer_fake
{

class Camera
{
public:
    Camera();

    int screenWidth();
    int screenHeight();

    const glm::vec3& position();
    const glm::vec3& viewDirection();
    const glm::vec3& rightVector();
    const glm::vec3& upVector();

    float zNear();

    float fieldOfView();
    float aspectRatio();

private:
    const glm::vec3 m_position;
    const glm::vec3 m_viewDirection;
    const glm::vec3 m_rightVector;
    const glm::vec3 m_upVector;
};

#define QGLVIEWER_NAMESPACE qglviewer_fake

}

#endif

class SceneCamera
{
public:
    SceneCamera(QGLVIEWER_NAMESPACE::Camera &camera);
    ~SceneCamera();

    void setupRendering();

    int width() const {return m_renderedImage != NULL ? m_renderedImage->width() : 0;}
    int height() const {return m_renderedImage != NULL ? m_renderedImage->height() : 0;}

    Ray castRayFromPixel(int x, int y) const;

    void setPixelf(int x, int y, float r, float g, float b);
    void setPixelfv(int x, int y, const glm::vec3* rgb);

    void setPixelb(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    void setPixelbv(int x, int y, const unsigned char *rgb);


#ifdef USE_QGLVIEWER
    static glm::vec3 vecToGlmVec3(const qglviewer::Vec &v) {return glm::vec3(v.x, v.y, v.z);}
    static qglviewer::Vec glmVec3ToVec(const glm::vec3 &v) {return qglviewer::Vec(v.x, v.y, v.z);}
#else
    static glm::vec3 vecToGlmVec3(const glm::vec3& v) {return v;}
    static glm::vec3 glmVec3ToVec(const glm::vec3 &v) {return v;}
#endif
    void showBeautifulRender();


private:

    QGLVIEWER_NAMESPACE::Camera   &m_camera;

    QImage              *m_renderedImage;

    glm::vec3           m_position;

    glm::vec3           m_viewDirection;
    glm::vec3           m_upVector;
    glm::vec3           m_rightVector;

    float m_screenWidthReal, m_screenHeightReal;

    glm::vec3           m_topLeftScreen;

    Dialog_RenderedImage m_dialog;

};

#endif // SCENECAMERA_H
