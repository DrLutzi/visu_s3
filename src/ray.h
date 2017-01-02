#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include "errorsHandler.hpp"

class Ray
{

public:

    class RandomCone
    {
    public:
        RandomCone(): set(false){}

        bool set;
        float angle;
        glm::vec3 direction;
        glm::vec3 rightVector;
        glm::vec3 upVector;
        glm::vec3 circleCenter;
    };

    Ray();
    Ray(const glm::vec3& origin, const glm::vec3& direction);
    Ray(const glm::vec3& origin, RandomCone& cone);

    inline const glm::vec3& origin() const        {return m_origin;}
    inline const glm::vec3& direction() const     {return m_direction;}

private:

    glm::vec3 m_origin;
    glm::vec3 m_direction;
};

#endif // RAY_H
