#include "viewer.h"
#include <glm/gtc/type_ptr.hpp>
#include <QKeyEvent>

Viewer::Viewer(QWidget *parent) :
    QGLViewer(parent),
    m_manager()
{}

void Viewer::tp_init()
{
    srand(0);

    setMouseTracking(true);

    // SHADER
    m_ShaderProgram = new ShaderProgram_RayTracer();

    //VBO
    glGenBuffers(1, &m_vbo_id);

    //EBO
    glGenBuffers(1, &m_ebo_id);

    // genere 1 VAO
    glGenVertexArrays(1, &m_Vao);

    // on travaille sur celui-ci
    glBindVertexArray(m_Vao);

    //associe l'ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo_id);

    // associe le VBO de position
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_id);
    // avec l'attribut position du shader
    glEnableVertexAttribArray(m_ShaderProgram->idOfVertexAttribute);
    // en donne les bon parametre (idAttrib, 3 x float / sommets, pas de normalisation,
    // et 0, 0 : on ne saute rien et on commence au debut)
    glVertexAttribPointer(m_ShaderProgram->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //vao fini
    glBindVertexArray(0);
}

Viewer::~Viewer()
{
    delete m_ShaderProgram;
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

    if(m_ShaderProgram != NULL)
    {
        m_ShaderProgram->startUseProgram();

        // envoit les matrices au shader
        glUniformMatrix4fv(m_ShaderProgram->idOfViewMatrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(m_ShaderProgram->idOfProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        // travaille avec le vao defini (et donc les buffers associés)
        glBindVertexArray(m_Vao);

        glPointSize(4.0); // clair !!
        glLineWidth(2.0);

        //draw here

        glBindVertexArray(0);

        m_ShaderProgram->stopUseProgram();
    }
}

void Viewer::keyPressEvent(QKeyEvent *e)
{
	updateGL();
	QGLViewer::keyPressEvent(e);
}

void Viewer::mousePressEvent(QMouseEvent *e)
{
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
