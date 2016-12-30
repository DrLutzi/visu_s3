#ifndef SCENECAMERA_H
#define SCENECAMERA_H

#include <QGLViewer/qglviewer.h>
#include <QImage>
#include <QLabel>
#include "ray.h"
#include <cmath>
#include "dialog_renderedimage.h"
#include <glm/gtx/string_cast.hpp>

class SceneCamera
{
public:
    SceneCamera(qglviewer::Camera &camera);
    ~SceneCamera();

    void setupRendering();

    int width() const {return m_renderedImage != NULL ? m_renderedImage->width() : 0;}
    int height() const {return m_renderedImage != NULL ? m_renderedImage->height() : 0;}

    Ray castRayFromPixel(int x, int y) const;

    void setPixelf(int x, int y, float r, float g, float b);
    void setPixelfv(int x, int y, const glm::vec3* rgb);

    void setPixelb(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    void setPixelbv(int x, int y, const unsigned char *rgb);

    static glm::vec3 vecToGlmVec3(const qglviewer::Vec &v) {return glm::vec3(v.x, v.y, v.z);}
    static qglviewer::Vec glmVec3ToVec(const glm::vec3 &v) {return qglviewer::Vec(v.x, v.y, v.z);}

    void showBeautifulRender();


private:

    qglviewer::Camera   &m_camera;

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
