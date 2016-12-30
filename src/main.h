#ifndef __MAIN_H__
#define __MAIN_H__

#include "openglwindow.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <vector>

#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec2.hpp>                 // glm::vec4
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective, glm::lookAtRH
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr
#include <glm/gtc/matrix_inverse.hpp>
#include <GL/glu.h>

#include <QTime>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtGui/QKeyEvent>

struct LightProperties
{
    glm::vec3   m_vAmbiant;
    glm::vec3   m_vDiffuse;
    glm::vec3   m_vSpecular;

    LightProperties()
    :   m_vAmbiant  ( 0.1f )
    ,   m_vDiffuse  ( 1.f, 1.f, 1.f )
    ,   m_vSpecular ( 1.0f, 1.0f, 1.0f )
    {}
};

struct MaterialProperties
{
    glm::vec3   m_vAmbiant;
    glm::vec3   m_vDiffuse;
    glm::vec3   m_vSpecular;
    float       m_fSpecularPower;

    MaterialProperties()
    :   m_vAmbiant      ( 0.1f, 0.4f, 0.4f )
    ,   m_vDiffuse      ( 0.3f, 0.4f, 1.0f )
    ,   m_vSpecular     ( 1.0f, 1.0f, 1.0f )
    ,   m_fSpecularPower( 300.f )
    {}
};


//====================================================================================================================================

class TPGLWindow : public OpenGLWindow
{
public:
    TPGLWindow();
    virtual ~TPGLWindow();

    void initialize();

    void render();

    void update();

private:

    /// Creates the GPU VertexShader
    void createGPUVertexShader();

    /// Creates the GPU FragmentShader
    void createGPUFragmentShader();

    /// Creates the GPU Program from the GPU VertexShader and GPU FragmentShader
    void createGPUProgramFromGPUShaders();

    /// Destroys GPU Program and Shaders
    void destroyGPUProgramAndShaders();

    /// Print the shader compiler information
    bool printShaderCompileInfo( GLuint _iShaderID, const std::string& _strMsg );

    //------------------------------------------------------------------------------------
    /// Get the GLSL uniform locations
    void getUniformLocations();

    /// Get the GLSL vertex attributes locations
    void getAttributeLocations();

    /// Send the uniform vars from the CPU to the GPU
    void sendUniformsToGPU();

     //------------------------------------------------------------------------------------
    /// Creates the VertexBufferObject that contains the geometry position
    void createVBO();

    /// Creates the VertexArrayObject that contains the geometry info
    void createVAOFromVBO();

    /// Destroys the VertexArrayObject created earlier
    void destroyVAOAndVBO();

    //------------------------------------------------------------------------------------
    /// Update the CameraProjection, CameraView, ObjectWorld matrices
    void updateMatrices();

    /// Handles key press event, and move the 3D object
    void keyPressEvent(QKeyEvent *);

    /// Handles resize event
    void resizeEvent(QResizeEvent*);

    //------------------------------------------------------------------------------------
    // TODO : see these new functions
    /// Creates the Texture object from a texture file
    void createTextures();

    /// Destroys all Texture objects
    void destroyTextures();

    /// Setup the textures objects in their texture unit
    void setupTexturesInUnit();




    GLuint      			m_iGPUProgramID;            ///< OpenGL ID for GPU Program
    GLuint      			m_iGPUVertexShaderID;       ///< OpenGL ID for GPU VertexShader
    GLuint      			m_iGPUFragmentShaderID;     ///< OpenGL ID for GPU FragmentShader

    GLuint                  m_iUniformColour;
    //------------------------------------------------------------------------------------
    GLuint      			m_iVertexCount;             ///< Number of vertices in the VAO
    GLuint      			m_iTrianglesCount;          ///< Number of triangles to draw
    GLuint      			m_iElementsCount;           ///< Number of indices used for drawing
    GLuint      			m_iVAO;                     ///< VertexArrayObject used for drawing

    GLuint      			m_iAttribPosition;          ///< GLSL attribute location for vertex position
    GLuint      			m_iVBOPosition;             ///< VertexBufferObject used to store position vertex data, used by the VAO
    GLuint      			m_iEBO;                     ///< ElementBufferObject used to store indices of vertices

    //------------------------------------------------------------------------------------
    glm::vec3   			m_vObjectTranslate;         ///< Store the 3D object translate component
    glm::vec3   			m_vObjectEulerAngles;       ///< Store the 3D object orientation as euler angles

    glm::mat4   			m_mtxObjectWorld;           ///< Matrix transform, for object -> world coordinates
    glm::mat4   			m_mtxCameraView;            ///< Matrix transform, for world -> camera view coordinates
    glm::mat4   			m_mtxCameraProjection;      ///< Matrix transform, for camera view -> camera projection coordinates

    GLuint      			m_iUniformWorld;            ///< GLSL uniform location for World matrix
    GLuint      			m_iUniformView;             ///< GLSL uniform location for View matrix
    GLuint      			m_iUniformProjection;       ///< GLSL uniform location for Projection matrix

    //------------------------------------------------------------------------------------
    QTime       			m_timer;                    ///< Time used to get elapsed time between 2 frames

    GLuint                  m_iAttribNormal;            ///< GLSL attribute location for vertex normal
    GLuint                  m_iVBONormal;               ///< VertexBufferObject used to store normal vertex data, used by the VAO

    glm::mat4               m_mtxNormal;                ///< Normal matrix (=InverseTranspose matrix of the World matrix) used to transform normals from object -> world coordinates
    GLuint                  m_iUniformNormal;           ///< GLSL uniform location for Noraml matrix

    LightProperties         m_lightProp;                ///< Light properties
    MaterialProperties      m_materialProp;             ///< Material properties
    GLuint                  m_aiUniformLightProp[3];    ///< GLSL uniform location for each light property
    GLuint                  m_aiUniformMaterialProp[4]; ///< GLSL uniform location for each material property

    glm::vec3               m_vCameraPosition;          ///< Camera position - in World Space
    glm::vec3               m_vLightPosition;           ///< Light position  - in World Space
    GLuint                  m_iUniformCameraPosition;   ///< GLSL uniform location for light position
    GLuint                  m_iUniformLightPosition;    ///< GLSL uniform location for light position

    //------------------------------------------------------------------------------------
    // TODO : see this NEW DATA
    GLuint                  m_iAttribTexCoords;         ///< GLSL attribute location for vertex texture coordinates
    GLuint                  m_iVBOTexCoords;            ///< VertexBufferObject used to store texture coordinates vertex data, used by the VAO

    GLuint                  m_iTexture;                 ///< OpenGL Texture id
    GLuint                  m_iUniformTextureSampler;   ///< GLSL uniform location for the first texture

    // TODO : this is for step 2 !
    GLuint                  m_iTexture2;                ///< OpenGL Texture id, 2
    GLuint                  m_iUniformTextureSampler2;  ///< GLSL uniform location for the second texture

    GLint                   m_status;

};

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
// ERROR & DEBUG related

#define DEBUG

// DEBUG_BREAK
#ifdef  WIN32
#   define  TP_DEBUG_BREAK()    __debugbreak()
#else
#   define  TP_DEBUG_BREAK()    asm("int $3")
#endif


// LOG
#define TP_LOG( _message, ...)  fprintf( stdout, (_message), __VA_ARGS__ );fflush( stdout )

// ASSERT
#ifdef DEBUG
#   define TP_ASSERT(_assert, ...)   \
        if( !(_assert) ) {  \
            TP_LOG( "Assertion failed !\n.FILE = '%s'\n.LINE='%d'\n.FUNCTION='%s'\n", __FILE__ , __LINE__, __FUNCTION__);\
            TP_LOG( __VA_ARGS__ );\
            TP_DEBUG_BREAK();\
            assert(_assert); \
        }
#else
#   define TP_ASSERT(_assert, ...)
#endif

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------


//====================================================================================================================================
void TPGLWindow::resizeEvent(QResizeEvent* /*_pEvent*/)
{
    // Force the update of the perspective matrix
    updateMatrices();
}

//====================================================================================================================================
void TPGLWindow::keyPressEvent(QKeyEvent* _pEvent)
{
    if(QEvent::KeyPress == _pEvent->type() )
    {
//        float   fTimeElapsed= (float) m_timer.elapsed();

        float   fMoveSpeed          = 0.07f;// * fTimeElapsed;
        bool    bHasObjectMoved     = false;


        // handle key press to move the 3D object
        switch( _pEvent->key() )
        {
        case Qt::Key_Minus:
            m_vObjectTranslate -= glm::vec3(0, 0, 1) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Plus:
            m_vObjectTranslate += glm::vec3(0, 0, 1) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Left:
            m_vObjectTranslate -= glm::vec3(1, 0, 0) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Right:
            m_vObjectTranslate += glm::vec3(1, 0, 0) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Up:
            m_vObjectTranslate += glm::vec3(0, 1, 0) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Down:
            m_vObjectTranslate -= glm::vec3(0, 1, 0) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Space:
            m_vObjectEulerAngles -= glm::vec3(0, glm::radians(1.0f), 0);
            bHasObjectMoved = true;
            break;

        case Qt::Key_Return:
        case Qt::Key_Enter:
            break;

        case Qt::Key_Escape:
            this->close();
            break;

        }
    }
}

//====================================================================================================================================
bool TPGLWindow::printShaderCompileInfo(GLuint _iShaderID, const std::string& _strMsg)
{
    int iInfologLength = 0;
    int iCharsWritten  = 0;
    char* strInfoLog;

    glGetShaderiv(_iShaderID, GL_INFO_LOG_LENGTH, & iInfologLength);

    if( iInfologLength > 1 )
    {
        strInfoLog = new char[ iInfologLength + 1 ];
        glGetShaderInfoLog(_iShaderID, iInfologLength, & iCharsWritten, strInfoLog);

        std::cerr << "--------------"<< std::endl << "compilation de " << _strMsg <<  " : "<<std::endl << strInfoLog <<std::endl<< "--------------"<< std::endl;

        delete [] strInfoLog;
    }

    return ( 1 == iInfologLength );
}


//====================================================================================================================================
std::string readFileSrc(const std::string& _rstrFilePath)
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    TP_ASSERT( _rstrFilePath.size() > 0 , "Invalid parameter _rstrFilePath(\"%s\") : size() must not be 0.\n", _rstrFilePath.c_str() );
    //-----------------------------------------------------------------------------

    TP_LOG("Opening file %s\n", _rstrFilePath.c_str() );

    std::string strContent;


    std::ifstream file(_rstrFilePath.c_str());
    TP_ASSERT( file, "Could not open file %s\n", _rstrFilePath.c_str() );

    file.seekg(0, std::ios::end);
    strContent.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    strContent.assign((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());


    //-----------------------------------------------------------------------------
    // postcondition
    TP_ASSERT( strContent.size() > 0 ,  "Invalid content read strContent(\"%s\") : size() must not be 0.\n", strContent.c_str() );
    //-----------------------------------------------------------------------------

    return strContent;
}

//====================================================================================================================================


#endif // __MAIN_H__

