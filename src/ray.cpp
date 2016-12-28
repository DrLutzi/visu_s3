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
