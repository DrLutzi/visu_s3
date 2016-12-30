#ifndef VIEWER_H
#define VIEWER_H

#include <GL/glew.h>
#include <QGLViewer/qglviewer.h>
#include <glm/glm.hpp>
#include <cstdlib>
#include "ShaderProgram_RayTracer.h"
#include "scenemanager.h"
#include <iostream>
#include "glm/gtx/string_cast.hpp"

/**
 * @brief The Viewer class
 * makes the relation between the programs, the manager and the view.
 * inherits QGLViewer.
 */
class Viewer : public QGLViewer
{
public:
    Viewer(QWidget *parent=NULL);
    ~Viewer();

protected:
	/// OpenGL intialisation appelee par la QGLViewer
    void init();

    /// transforms rectangular patch into one upper triangular patch
    void rectangularPatch2UpperTrianglePatch();

	/// draw callback de la QGLViewer
    void draw();

	/// callback when key pressed
    void keyPressEvent(QKeyEvent *e);

    /// callback when mouse pressed
    void mousePressEvent(QMouseEvent *e);

    /// callback when mouse released
    void mouseReleaseEvent(QMouseEvent *e);

    /// callback when mouse moved
    void mouseMoveEvent(QMouseEvent *e);

    static glm::vec3 vecToGlmVec3(const qglviewer::Vec &v);
    static qglviewer::Vec glmVec3ToVec(const glm::vec3 &v);

	/// recupere la matrice de modelview de la QGLViewer
	glm::mat4 getCurrentModelViewMatrix() const;

    /// recupere la matrice de projection de la QGLViewer
	glm::mat4 getCurrentProjectionMatrix() const;

    /// init specific au TP
    void tp_init();

    ShaderProgram_RayTracer* m_shaderProgram;

    SceneManager *m_manager;
};

#endif
