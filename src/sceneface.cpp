#include "sceneface.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

SceneFace::SceneFace(const glm::vec3& bottomLeftPos, const glm::vec3& directionW, const glm::vec3& directionH, float w, float h) :
    SceneObject(),
    m_width(w),
    m_height(h)
{
    if(w==0 || h==0)
        ERROR("SceneFace: forbidden to have faces with width or height equal to 0 \
(please check your values before face creation)");

    m_P[0]=bottomLeftPos;
    //find another direction as the cross product of normal and direction to get the third axis of the plane
    m_normal=glm::normalize(glm::cross(directionW, directionH));

    //check if this direction is a valid vector, if it isn't, then the plane is not completly determined, which we decide to forbid
    if(glm::length(m_normal)<=0)
        ERROR("SceneFace: directions are colinear or their length \
is too small (face generation not deterministic)");

    //we can only work with our directions as axis if they are normalized
    m_axisW = glm::normalize(directionW);

    //we also make sure we have all our axis orthogonal to each others, it doesn't matter too much if directionH is changed a bit
    m_axisH = glm::normalize(glm::cross(m_normal, directionW));

    //now we can compute our points. We already have p0, the rest is easy
    m_P[1] = m_P[0] + m_axisW*w;
    m_P[2] = m_P[1] + m_axisH*h;
    m_P[3] = m_P[0] + m_axisH*h;
}

void SceneFace::intersectsRay(const Ray &ray, RayHitProperties& properties)
{
    //to do this, first we check if the ray intersects the plane defined by the face,
    //then we check if the point on the plane is inside the face.
    //This approach allows us to eliminate every cases where the ray isn't even able to intersect the plane before it does.

    float NdotrD=glm::dot(m_normal, ray.direction());
    if(std::abs(NdotrD)>EPSILON)
    {
        //< plane not parallel to the ray
        //get the distance from the plane

        float distanceFromPlane = glm::dot(m_normal, m_P[0] - ray.origin()) / NdotrD;
        if(distanceFromPlane>0 && (!properties.occuredHit || distanceFromPlane < properties.distanceHit))
        { //< plane intersection is not behind the ray and closer than the previous intersection

            //get the intersection point between the ray and the plane as "pM"
            glm::vec3 pM;
            pM = ray.origin() + ray.direction() * distanceFromPlane;

            //now we just need to check if this point is inside the rectangle.
            //we will use the dot product of each segment to do the job.

            if( -EPSILON > glm::dot(m_P[1] - m_P[0], pM - m_P[1]) &&
                -EPSILON > glm::dot(m_P[2] - m_P[1], pM - m_P[2]) &&
                -EPSILON > glm::dot(m_P[3] - m_P[2], pM - m_P[3]) &&
                -EPSILON > glm::dot(m_P[0] - m_P[3], pM - m_P[0]))
            {
                //We found a new intersection better than any previous intersection.
                properties.occuredHit   = true;
                properties.objectHit    = this;
                properties.positionHit  = pM;
                properties.normalHit    = NdotrD>0 ? -m_normal : m_normal;
                properties.distanceHit  = distanceFromPlane;
            }
        }
    }
    return;
}

SceneFace::UniformIntegral SceneFace::beginUniformIntegral(size_t N) const
{
    UniformIntegral ui;
    ui.size=N;
    ui.actualSize=N*N;
    ui.index=0;
    ui.value=m_P[0];
    return ui;
}

void SceneFace::nextUniformIntegral(UniformIntegral& integral) const
{
    ++integral.index;
    size_t i=integral.index%integral.size;
    size_t j=integral.index/integral.size;

    integral.value=(m_P[0] + m_axisW * (float)i / m_width + m_axisH * (float)j / m_height);
}

SceneFace::UniformIntegral SceneFace::endUniformIntegral(size_t N) const
{
    UniformIntegral ui;
    ui.index=N*N;
    return ui;
}

//OpenGL sizes

GLint SceneFace::numberAttributes() const
{
    return 4;
}

GLsizeiptr SceneFace::sizeVBOPosition() const
{
    return 4 * 3 * sizeof(GL_FLOAT);
}

GLsizeiptr SceneFace::sizeEBO() const
{
    //0, we don't need an EBO here.
    return 0;
}

//OpenGL fill given VBO and EBO segment

void SceneFace::makeVBOPosition(GLint vboId) const
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    //just copy the content of P
    glBufferSubData(GL_ARRAY_BUFFER, m_firstVBOPosition, sizeVBOPosition(), &m_P[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SceneFace::makeEBO(GLint eboId) const
{
    //nothing. We don't need an EBO here.
}

//OpenGL draw with given VBO and EBO segments

void SceneFace::draw() const
{
    glUniform3fv(ms_uniformColorLocation, 1, &m_color[0]);
    glDrawArrays(GL_TRIANGLE_FAN, m_firstVBOPosition, 4);
}

glm::vec3 SceneFace_Prop::colorAmbiant(SceneFace_Light &light)
{
    return light.lightProperties().vAmbiant * m_materialProperties.vAmbiant;
}

glm::vec3 SceneFace_Prop::colorDiffuse(SceneFace_Light &light, glm::vec3& N, glm::vec3& L)
{
    // calculation as for Lambertian reflection
    float NdotL = glm::dot(N , L);
    float diffuseTerm = std::max(0.0f, NdotL); //0 <= diffuseTerm <= 1

    return light.lightProperties().vDiffuse * diffuseTerm * m_materialProperties.vDiffuse;
}

glm::vec3 SceneFace_Prop::colorSpecular(SceneFace_Light &light, glm::vec3& N, glm::vec3& L, glm::vec3& vToEye)
{
    //get light reflected from the surface
    glm::vec3 reflectedL = glm::reflect(-L , N);

    float specularTerm = std::pow( std::max(0.0f, glm::dot(reflectedL, vToEye)), m_materialProperties.fSpecularPower );
    //0 <= specularTerm <= 1

    return m_materialProperties.vSpecular * specularTerm * light.lightProperties().vSpecular;
}
