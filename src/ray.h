#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include "errorsHandler.hpp"

class Ray
{
public:
    Ray();
    Ray(const glm::vec3& origin, const glm::vec3& direction);

    inline const glm::vec3& origin() const        {return m_origin;}
    inline const glm::vec3& direction() const     {return m_direction;}

private:

    glm::vec3 m_origin;
    glm::vec3 m_direction;
};

#endif // RAY_H
