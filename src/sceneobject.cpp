#include "sceneobject.h"

unsigned int SceneObject::ms_currentId=0;
GLuint SceneObject::ms_uniformColorLocation=0;

SceneObject::SceneObject() :
    m_id(ms_currentId++),
    m_color(0,0,0)
{
}
