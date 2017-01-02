#include "viewer.h"
#include <glm/gtc/type_ptr.hpp>
#include <QKeyEvent>

Viewer::Viewer(QWidget *parent) :
    QGLViewer(parent)
{}

void Viewer::tp_init()
{
    srand(0);

    setMouseTracking(true);

    // SHADER
    m_shaderProgram = new ShaderProgram_RayTracer();

    m_shaderProgram->createVBO();
    m_shaderProgram->createVAOFromVBO();

#ifdef USE_QGLVIEWER
    m_manager = new SceneManager(*camera(),
                                 m_shaderProgram->vaoId,
                                 m_shaderProgram->vboPositionId,
                                 m_shaderProgram->eboId,
                                 m_shaderProgram->idOfColor);

    m_manager->setup();
    m_manager->remakeScene();

#endif
}

Viewer::~Viewer()
{
    m_shaderProgram->destroyVAOAndVBO();
    delete m_shaderProgram;
}

void Viewer::init()
{
    makeCurrent();
    glewExperimental = GL_TRUE;
    int glewErr = glewInit();
    if( glewErr != GLEW_OK )
    {
        qDebug("Error %s", glewGetErrorString(glewErr) ) ;
    }
    std::cout << "GL VERSION = " << glGetString(GL_VERSION) << std::endl;

	// la couleur de fond
	glClearColor(0.1,0.1,0.2,0.0);

	// on initialize le rendu que l'on veut faire
    tp_init();

	// LIB QGLViewer scene initialization
    setSceneRadius(20.0);
    setSceneCenter(qglviewer::Vec(0.0,0.0,0.0));
    camera()->showEntireScene();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Viewer::draw()
{
    // recupere les matrices depuis l'interface
    glm::mat4 viewMatrix = getCurrentModelViewMatrix();
    glm::mat4 projectionMatrix = getCurrentProjectionMatrix();

    if(m_shaderProgram != NULL)
    {
        m_shaderProgram->startUseProgram();

        // envoit les matrices au shader
        glUniformMatrix4fv(m_shaderProgram->idOfViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(m_shaderProgram->idOfProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        // travaille avec le vao defini (et donc les buffers associés)
        glBindVertexArray(m_shaderProgram->vaoId);

        //draw here
        m_manager->drawScene();

        glBindVertexArray(0);

        m_shaderProgram->stopUseProgram();
    }
}

void Viewer::keyPressEvent(QKeyEvent *e)
{
	updateGL();

    if(e->key()==Qt::Key_A)
        m_manager->myFirstRendering();

    else if(e->key()==Qt::Key_B)
        m_manager->phongRendering(12);

    else if(e->key()==Qt::Key_C)
        m_manager->phongRendering(12, SceneObject::Integral::UNIFORM);

    else if(e->key()==Qt::Key_D)
        m_manager->phongRendering(12, SceneObject::Integral::UNIFORM_RANDOM);

	QGLViewer::keyPressEvent(e);
}

void Viewer::mousePressEvent(QMouseEvent *e)
{
    qglviewer::Vec origin, direction;
    camera()->convertClickToLine(e->pos(), origin, direction);

    Ray r(vecToGlmVec3(origin), vecToGlmVec3(direction));
    unsigned int intFound=0;
    for(SceneManager::const_iterator it=m_manager->begin(); it!=m_manager->end(); ++it)
    {
        SceneObject *obj=(*it).second;
        SceneObject::RayHitProperties rayHit;
        if(obj!=NULL)
        {
            obj->intersectsRay(r, rayHit);
            if(rayHit.occuredHit)
            {
                std::cout << "Found intersection with " << rayHit.objectHit->id() << " at position " << glm::to_string(rayHit.positionHit) << " and at distance " << rayHit.distanceHit << std::endl;
                ++intFound;
            }
        }
    }

    std::cout << "Intersections found: " << intFound << std::endl;

    QGLViewer::mousePressEvent(e);
}

void Viewer::mouseReleaseEvent(QMouseEvent *e)
{
    QGLViewer::mouseReleaseEvent(e);
}

void Viewer::mouseMoveEvent(QMouseEvent *e)
{
   QGLViewer::mouseMoveEvent(e);
}

glm::vec3 Viewer::vecToGlmVec3(const qglviewer::Vec& v)
{
    return glm::vec3(v.x, v.y, v.z);
}

qglviewer::Vec Viewer::glmVec3ToVec(const glm::vec3 &v)
{
    return qglviewer::Vec(v.x, v.y, v.z);
}

glm::mat4 Viewer::getCurrentModelViewMatrix() const
{
	GLdouble gl_mvm[16];
	camera()->getModelViewMatrix(gl_mvm);
	glm::mat4 mvm;
	for(unsigned int i = 0; i < 4; ++i)
	{
		for(unsigned int j = 0; j < 4; ++j)
			mvm[i][j] = (float)gl_mvm[i*4+j];
	}
	return mvm;
}


glm::mat4 Viewer::getCurrentProjectionMatrix() const
{
	GLdouble gl_pm[16];
	camera()->getProjectionMatrix(gl_pm);
	glm::mat4 pm;
	for(unsigned int i = 0; i < 4; ++i)
	{
		for(unsigned int j = 0; j < 4; ++j)
			pm[i][j] = (float)gl_pm[i*4+j];
	}
	return pm;
}
