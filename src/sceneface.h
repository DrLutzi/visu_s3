#ifndef SCENEFACE_H
#define SCENEFACE_H

#include "sceneobject.h"

///
/// \brief The SceneFace class is a representation of a static face in a scene with
/// all its properties pre-computed for time optimization and writting convenience purposes.
///
class SceneFace : public SceneObject
{
public:
    SceneFace(const glm::vec3& p0, const glm::vec3& directionW, const glm::vec3& directionH, float w, float h);

    bool intersectsRay(const Ray& ray, glm::vec3& intersectionPoint, float &distanceIntersect) const;

    //OpenGL sizes

    virtual GLsizeiptr sizeVBO() const;
    virtual GLsizeiptr sizeEBO() const;

    //OpenGL fill given VBO and EBO segment

    virtual void makeVBOPosition(GLint vboId) const;
    virtual void makeEBO(GLint eboId) const;

    //OpenGL draw with given VBO and EBO segments

    virtual void draw() const;

private:

    /// \brief the 4 points of the face
    glm::vec3 m_P[4];

    /// \brief m_normal normal of the face
    glm::vec3 m_normal;

    /// \brief m_axisW normalized vector to the direction of width
    glm::vec3 m_axisW;

    /// \brief m_axisH normalized vector to the direction of height
    glm::vec3 m_axisH;

    float m_width;
    float m_height;
};

#endif // SCENEFACE_H
