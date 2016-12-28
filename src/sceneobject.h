#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "ray.h"
#include "GL/glew.h"

///
/// \brief The SceneObject class is an abstract representation for a scene object
/// that is being drawn and manipulated inside and outside shader programs.
/// It provides a convenient openGL interface for the SceneManager class which is in charge of the VBO and EBO segments,
/// abstract methods to transfer objects to the VBO and EBO segments and to draw these objects,
/// as well as logic methods to make calculations based on the objects.
///
class SceneObject
{
public:
    SceneObject();

    ///
    /// \brief intersectsRay computes the intersection properties between the ray and this object.
    /// \param ray the ray (with origin and direction)
    /// \param intersectionPoint a reference to the point of the first intersection, might be changed even if no intersection was found.
    /// \param distanceIntersect the distance between the intersected point and the origin of the ray, might be changed even if no intersection was found.
    /// \return true if the ray intersects the face and isn't parallel to the face, false otherwise.
    ///
    virtual bool intersectsRay(const Ray& ray, glm::vec3& intersectionPoint, float &distanceIntersect) const=0;

    //OpenGL indexes

    inline void setFirstVBO(GLintptr first)             {m_firstVBO=first;}
    inline void setFirstEBO(GLintptr first)             {m_firstEBO=first;}
    inline void setBaseVertexEBO(GLintptr baseVertex)   {m_baseVertexEBO=baseVertex;}


    inline GLintptr firstVBO() const                    {return m_firstVBO;}
    inline GLintptr firstEBO() const                    {return m_firstEBO;}
    inline GLintptr baseVertexEBO() const               {return m_baseVertexEBO;}

    //OpenGL sizes

    virtual GLsizeiptr sizeVBO() const=0;
    virtual GLsizeiptr sizeEBO() const=0;

    //OpenGL fill given VBO and EBO segment

    virtual void makeVBOPosition(GLint vboId) const=0;
    virtual void makeEBO(GLint eboId) const=0;

    //OpenGL draw with given VBO and EBO segments

    virtual void draw() const=0;

protected:

    unsigned int m_id;

    GLintptr                    m_firstVBO;            //first index of the VBO datas, used by the manager
    GLintptr                    m_firstEBO;            //first index of the EBO datas, used by the manager
    GLintptr                    m_baseVertexEBO;       //number that should be added to every index in the EBO

    static unsigned int ms_currentId;
};

#endif // SCENEOBJECT_H
