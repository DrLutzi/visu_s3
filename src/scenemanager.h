#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "sceneface.h"
#include <map>

class SceneManager
{
public:
    SceneManager();

    typedef std::map<unsigned int, SceneObject*>::iterator iterator;
    typedef std::map<unsigned int, SceneObject*>::const_iterator const_iterator;

private:

    std::map<unsigned int, SceneObject*> m_objects;

    /// OpenGL objects
    GLint                           m_VAOId;
    GLint                           m_VBOPositionId;
    GLint                           m_EBOId;
    GLint                           m_uColorLocation;

    /// VBO/EBO sizes
    GLsizeiptr                      m_VBOPositionSize;
    GLsizeiptr                      m_EBOSize;
    GLint                           m_currentBaseVertex;

    GLsizeiptr                      m_VBOCapacity;
    GLsizeiptr                      m_EBOCapacity;
};

#endif // SCENEMANAGER_H
