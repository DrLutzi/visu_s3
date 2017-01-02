#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "ray.h"
#include <GL/glew.h>

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

    class RayHitProperties
    {
    public:
        RayHitProperties() :
            occuredHit(false)
        {}

        bool            occuredHit;
        SceneObject     *objectHit;
        glm::vec3       positionHit;
        glm::vec3       normalHit;
        float           distanceHit;
    };

    SceneObject();

    ///
    /// \brief intersectsRay computes the intersection properties between the ray and this object.
    /// The ray properties are persistent through a whole scene iteration, so they only register the closest hit.
    /// \param ray the ray (with origin and direction)
    /// \param hitProperties hit properties of the intersection
    ///
    virtual void intersectsRay(const Ray &ray, RayHitProperties& properties) =0;

    //uniform integral simulation on the surface, iterator style

    class Integral
    {
    public:

        typedef enum {SINGLE_MEAN, UNIFORM, UNIFORM_RANDOM} Type_t;

        Integral():
            index(0) {}
        Integral(const Integral& other)
            {value=other.value; index=other.index; size=other.size; actualSize=other.size;}
        ~Integral() {}
        bool operator==(const Integral& other)
            {return index==other.index;}
        bool operator!=(const Integral& other)
            {return index!=other.index;}
        glm::vec3 &operator*()
            {return value;}

        Type_t          type;
        glm::vec3       value;
        size_t          index;
        size_t          size;
        size_t          actualSize;
    };

    virtual Integral beginIntegral(size_t N=0, Integral::Type_t type=Integral::SINGLE_MEAN) const=0;
    virtual void nextIntegral(Integral& integral) const=0;
    virtual Integral endIntegral(size_t N=0, Integral::Type_t type=Integral::SINGLE_MEAN) const=0;

    //OpenGL indexes

    inline void setFirstVBOPosition(GLintptr first)     {m_firstVBOPosition=first;}
    inline void setFirstEBO(GLintptr first)             {m_firstEBO=first;}
    inline void setBaseVertexEBO(GLintptr baseVertex)   {m_baseVertexEBO=baseVertex;}


    inline GLintptr firstVBOPosition() const            {return m_firstVBOPosition;}
    inline GLintptr firstEBO() const                    {return m_firstEBO;}
    inline GLintptr baseVertexEBO() const               {return m_baseVertexEBO;}

    //OpenGL sizes

    ///
    /// \brief numberVBOPosition
    /// \return the number of vertices sent to the VBO
    ///
    virtual GLint numberAttributes() const=0;

    ///
    /// \brief sizeVBOPosition
    /// \return the size taken by the position VBO
    ///
    virtual GLsizeiptr sizeVBOPosition() const=0;
    virtual GLsizeiptr sizeEBO() const=0;

    //OpenGL fill given VBO and EBO segment

    virtual void makeVBOPosition(GLint vboId) const=0;
    virtual void makeEBO(GLint eboId) const=0;

    //OpenGL draw with given VBO and EBO segments

    virtual void draw() const=0;

    //properties

    inline unsigned int id() const {return m_id;}

    inline void setColor(const glm::vec3 &color) {m_color=color;}
    inline const glm::vec3& color() {return m_color;}

    inline static void setColorLocation(GLuint uniformColorLocation) {ms_uniformColorLocation=uniformColorLocation;}


protected:

    unsigned int m_id;

    GLintptr                    m_firstVBOPosition;    //first index of the VBO datas, used by the manager
    GLintptr                    m_firstEBO;            //first index of the EBO datas, used by the manager
    GLintptr                    m_baseVertexEBO;       //number that should be added to every index in the EBO

    glm::vec3                   m_color;

    static GLuint       ms_uniformColorLocation;
    static unsigned int ms_currentId;
};

#endif // SCENEOBJECT_H
