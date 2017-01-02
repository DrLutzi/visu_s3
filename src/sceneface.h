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

    void intersectsRay(const Ray &ray, RayHitProperties& properties);

    //Uniform integration

    Integral beginIntegral(size_t N=0, Integral::Type_t type=Integral::SINGLE_MEAN) const;
    void nextIntegral(Integral& integral) const;
    Integral endIntegral(size_t N=0, Integral::Type_t type=Integral::SINGLE_MEAN) const;

    //OpenGL sizes

    GLint numberAttributes() const;

    GLsizeiptr sizeVBOPosition() const;
    GLsizeiptr sizeEBO() const;

    //OpenGL fill given VBO and EBO segment

    void makeVBOPosition(GLint vboId) const;
    void makeEBO(GLint eboId) const;

    //OpenGL draw with given VBO and EBO segments

    void draw() const;

private:

    /// \brief the positions of the 4 vertices of the face. m_P[0] is bottom left,
    /// and the positions are set in a counter clockwise fashion.
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

class SceneFace_Light;

///
/// \brief The SceneFace_Prop class is a representation of a SceneFace with
///  phong material properties for advanced rendering.
///
class SceneFace_Prop : public SceneFace
{
public:

    SceneFace_Prop(const glm::vec3& p0, const glm::vec3& directionW, const glm::vec3& directionH, float w, float h) :
        SceneFace(p0, directionW, directionH, w, h)
    {}

    typedef struct
    {
        glm::vec3   vAmbiant;
        glm::vec3   vDiffuse;
        glm::vec3   vSpecular;
        float       fSpecularPower;
    } MaterialProperties_t;

    inline void setMaterialProperties(const MaterialProperties_t& properties)   {m_materialProperties=properties;}
    inline const MaterialProperties_t& materialProperties() const               {return m_materialProperties;}

    glm::vec3 colorAmbiant(const SceneFace_Light &light) const;
    glm::vec3 colorDiffuse(const SceneFace_Light &light, const glm::vec3 &N, const glm::vec3 &L) const;
    glm::vec3 colorSpecular(const SceneFace_Light &light, const glm::vec3 &N, const glm::vec3 &L, const glm::vec3 &vToEye) const;

private:
        MaterialProperties_t                m_materialProperties;       //properties of material in the scene
};

///
/// \brief The SceneFace_Light class is a representation of a SceneFace with
///  phong light properties for advanced rendering.
///
class SceneFace_Light : public SceneFace
{
public:
    SceneFace_Light(const glm::vec3& p0, const glm::vec3& directionW, const glm::vec3& directionH, float w, float h):
        SceneFace(p0, directionW, directionH, w, h)
    {}

    typedef struct
    {
        glm::vec3   vAmbiant;
        glm::vec3   vDiffuse;
        glm::vec3   vSpecular;
    } LightProperties_t;

    inline void setLightProperties(const LightProperties_t& properties)     {m_lightProperties=properties;}
    inline const LightProperties_t& lightProperties() const                 {return m_lightProperties;}

private :
    LightProperties_t                   m_lightProperties;          //properties of light source in the scene
};

#endif // SCENEFACE_H
