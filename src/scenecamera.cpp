#include "scenecamera.h"

#ifndef USE_QGLVIEWER

qglviewer_fake::Camera::Camera() :
    m_position(0, 0, 10.0f),
    m_viewDirection(0,0, -1.0f),
    m_rightVector(1.0f, 0, 0),
    m_upVector(0, 1.0f, 0)
{}

int qglviewer_fake::Camera::screenWidth()
{
    return 800;
}

int qglviewer_fake::Camera::screenHeight()
{
    return 600;
}

const glm::vec3& qglviewer_fake::Camera::position()
{
    return m_position;
}

const glm::vec3& qglviewer_fake::Camera::viewDirection()
{
    return m_viewDirection;
}

const glm::vec3& qglviewer_fake::Camera::rightVector()
{
    return m_rightVector;
}

const glm::vec3& qglviewer_fake::Camera::upVector()
{
    return m_upVector;
}

float qglviewer_fake::Camera::zNear()
{
    return 0.005f;
}

float qglviewer_fake::Camera::fieldOfView()
{
    return M_PI/2.0f;
}

float qglviewer_fake::Camera::aspectRatio()
{
    return 16.0f/9.0f;
}

SceneCamera::SceneCamera(qglviewer_fake::Camera &camera) :
    m_camera        (camera),
    m_renderedImage (NULL)
{}

#else

SceneCamera::SceneCamera(qglviewer::Camera &camera) :
    m_camera        (camera),
    m_renderedImage (NULL)
{}

#endif

SceneCamera::~SceneCamera()
{
    if(m_renderedImage!=NULL)
        delete m_renderedImage;
}

void SceneCamera::setupRendering()
{
    //allocate the image and register the current camera state (at least what is useful for us)
    if(m_renderedImage!=NULL)
        delete m_renderedImage;
    m_renderedImage = new QImage(m_camera.screenWidth(), m_camera.screenHeight(), QImage::Format_RGB888);
    m_position = vecToGlmVec3(m_camera.position());

    m_viewDirection = vecToGlmVec3(m_camera.viewDirection());
    m_rightVector = vecToGlmVec3(m_camera.rightVector());
    m_upVector    = vecToGlmVec3(m_camera.upVector());

    //project on the zPlane
    float distanceFromZPlane = (float)m_camera.zNear();
    glm::vec3 toZPlaneCenter = m_viewDirection * distanceFromZPlane;
    glm::vec3 zPlaneCenter = m_position + toZPlaneCenter;

    //find the zPlane actual width using tan (opposite side = tan(angle)*length adjacent size)
    m_screenHeightReal = std::tan(m_camera.fieldOfView()/2)*2 * distanceFromZPlane;
    m_screenWidthReal = m_screenHeightReal * m_camera.aspectRatio();

    std::cout << "center of the camera: " << glm::to_string(zPlaneCenter) << std::endl;

    //find the bottom left of the screen for convenience
    m_topLeftScreen = zPlaneCenter + (m_upVector * m_screenHeightReal - m_rightVector * m_screenWidthReal)/2.0f;
}

Ray SceneCamera::castRayFromPixel(int x, int y) const
{
    if(m_renderedImage==NULL)
        ERROR("setupRendering() not called before castRayFromPixel!");
    glm::vec3 R3Pixel = m_topLeftScreen
            + m_rightVector*(((float)x+0.5f)/m_renderedImage->width())  *   m_screenWidthReal
            - m_upVector*(((float)y+0.5f)/m_renderedImage->height())    *   m_screenHeightReal;

    return Ray(m_position, glm::normalize(R3Pixel - m_position));
}

Ray SceneCamera::castStochasticRayFromPixel(int x, int y) const
{
    if(m_renderedImage==NULL)
        ERROR("setupRendering() not called before castStochasticRayFromPixel!");
    std::uniform_real_distribution<float> randomGen(0.0f, 1.0f);
    glm::vec3 R3Pixel = m_topLeftScreen
            + m_rightVector*(((float)x+randomGen(Random::genMt19937))/m_renderedImage->width())  *   m_screenWidthReal
            - m_upVector*(((float)y+randomGen(Random::genMt19937))/m_renderedImage->height())    *   m_screenHeightReal;

    return Ray(m_position, glm::normalize(R3Pixel - m_position));
}

void SceneCamera::setPixelf(int x, int y, float r, float g, float b)
{
    m_renderedImage->setPixel(x, y, qRgb(int(r*255), int(g*255), int(b*255)));
}

void SceneCamera::setPixelfv(int x, int y, const glm::vec3 *rgb)
{
    m_renderedImage->setPixel(x, y, qRgb(int(rgb->r*255), int(rgb->g*255), int(rgb->b*255)));
}

void SceneCamera::setPixelb(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    m_renderedImage->setPixel(x, y, qRgb(r,g,b));
}

void SceneCamera::setPixelbv(int x, int y, const unsigned char *rgb)
{
    m_renderedImage->setPixel(x, y, qRgb(rgb[0],rgb[1],rgb[2]));
}

void SceneCamera::showBeautifulRender()
{
    if(m_renderedImage==NULL)
        ERROR("setupRendering() not called before showBeautifulRender!");
    m_dialog.setImage(m_renderedImage);
    m_dialog.show();
}
