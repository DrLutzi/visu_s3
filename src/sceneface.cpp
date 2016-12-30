#include "sceneface.h"
#include <glm/gtc/type_ptr.hpp>

SceneFace::SceneFace(const glm::vec3& p0, const glm::vec3& directionW, const glm::vec3& directionH, float w, float h) :
    SceneObject(),
    m_width(w),
    m_height(h)
{
    m_P[0]=p0;
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

bool SceneFace::intersectsRay(const Ray &ray, glm::vec3 &intersectionPoint, float &distanceIntersect) const
{
    //to do this, first we check if the ray intersects the plane defined by the face,
    //then we check if the point on the plane is inside the face.
    //This approach allows us to eliminate every cases where the ray isn't even able to intersect the plane before it does.

    float NdotrD=glm::dot(m_normal, ray.direction());
    if(std::abs(NdotrD)<=EPSILON)
        return false;   //< plane is parallel to the ray

    //get the distance from the plane
    distanceIntersect = glm::dot(m_normal, m_P[0] - ray.origin()) / NdotrD;
    if(distanceIntersect<=0)
        return false;   //< plane is somewhere behind the ray

    //get the intersection point between the ray and the plane as "pM"
    intersectionPoint = ray.origin() + ray.direction() * distanceIntersect;


    //now we just need to check if this point is inside the rectangle and we're all set.
    //we will use the dot product of each segment to do the job.

    return( -EPSILON > glm::dot(m_P[1] - m_P[0], intersectionPoint - m_P[1]) &&
            -EPSILON > glm::dot(m_P[2] - m_P[1], intersectionPoint - m_P[2]) &&
            -EPSILON > glm::dot(m_P[3] - m_P[2], intersectionPoint - m_P[3]) &&
            -EPSILON > glm::dot(m_P[0] - m_P[3], intersectionPoint - m_P[0]));

}

bool SceneFace::intersectsRay(const Ray &ray, RayHitProperties& properties) const
{
    //to do this, first we check if the ray intersects the plane defined by the face,
    //then we check if the point on the plane is inside the face.
    //This approach allows us to eliminate every cases where the ray isn't even able to intersect the plane before it does.

    float NdotrD=glm::dot(m_normal, ray.direction());
    if(std::abs(NdotrD)<=EPSILON)
        return false;   //< plane is parallel to the ray

    //get the distance from the plane
    properties.distanceHit = glm::dot(m_normal, m_P[0] - ray.origin()) / NdotrD;
    if(properties.distanceHit<=0)
        return false;   //< plane is somewhere behind the ray

    //get the intersection point between the ray and the plane as "pM"
    properties.positionHit = ray.origin() + ray.direction() * properties.distanceHit;

    properties.idObjectHit = id();


    //now we just need to check if this point is inside the rectangle and we're all set.
    //we will use the dot product of each segment to do the job.

    return( -EPSILON > glm::dot(m_P[1] - m_P[0], properties.positionHit - m_P[1]) &&
            -EPSILON > glm::dot(m_P[2] - m_P[1], properties.positionHit - m_P[2]) &&
            -EPSILON > glm::dot(m_P[3] - m_P[2], properties.positionHit - m_P[3]) &&
            -EPSILON > glm::dot(m_P[0] - m_P[3], properties.positionHit - m_P[0]));
}

//OpenGL sizes

size_t SceneFace::numberAttributes() const
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
