#include "ray.h"

Ray::Ray():
    m_origin(0,0,0),
    m_direction(0,0,0)
{
}

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction):
    m_origin(origin),
    m_direction(direction)
{
}

Ray::Ray(const glm::vec3& origin, RandomCone &cone):
    m_origin(origin)
{
    if(!cone.set)
    {
        if(cone.angle <= -M_PI || cone.angle >= M_PI)
            ERROR("Ray: Unable to create a random cone with an angle > or equal to Pi");
        //find an orthogonal vector
        cone.rightVector = glm::cross(glm::vec3(1,0,0), m_direction);
        if(glm::length(cone.rightVector) < EPSILON)
        {
            //that's really unfortunate
            cone.rightVector = glm::cross(glm::vec3(0,1,0), m_direction);
        }
        cone.upVector = glm::cross(cone.direction, cone.rightVector);

        //find the distance to some circle center with r=1. Simple trigonometry
        float distanceToCenter= 1 / std::tan(cone.angle);

        cone.circleCenter = origin + cone.direction * distanceToCenter;
    }

    //pick a point inside the created circle
    std::uniform_real_distribution<float> randomGen(0.0f, 1.0f);

    float angle2d = randomGen(Random::genMt19937)*2*M_PI;
    float rand2 = randomGen(Random::genMt19937)+randomGen(Random::genMt19937);
    float r = rand2 > 1.0f ? 2.0f-rand2 : rand2;

    glm::vec3 pickedPoint = cone.circleCenter +
            r*std::cos(angle2d)*cone.rightVector +
            r*std::sin(angle2d)*cone.upVector;
    m_direction = glm::normalize(pickedPoint - m_origin);
}
