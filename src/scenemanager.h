#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "sceneface.h"
#include "scenecamera.h"
#include <map>

class SceneManager
{
public:
#ifdef USE_QGLVIEWER
    SceneManager(qglviewer::Camera &camera, GLint vaoId, GLint vboPositionId, GLint eboId, GLuint colorLocation);
#else
    SceneManager(qglviewer_fake::Camera &camera, GLint vaoId, GLint vboPositionId, GLint eboId, GLuint colorLocation);
#endif

    typedef std::map<unsigned int, SceneObject*>::iterator iterator;
    typedef std::map<unsigned int, SceneObject*>::const_iterator const_iterator;


    inline iterator begin(){return m_objects.begin();}
    inline const_iterator begin() const{return m_objects.begin();}

    inline iterator end(){return m_objects.end();}
    inline const_iterator end() const{return m_objects.end();}

    ///
    /// \brief setup
    /// setup what we want for rendering
    ///
    void setup();

    ///
    /// \brief attaches an object to the manager.
    /// \param object object to attach
    /// \param reallocate specifies if the function is authorized to call glBindBuffer again if needed.
    /// It can be efficient to avoid reallocating when several objects are attached to the manager at once.
    ///
    void append(SceneObject* object, bool reallocate=true);

    ///
    /// \brief removes a object from the manager and returns it (often for deletion).
    /// \param id id of the object to be removed
    ///
    SceneObject *remove(unsigned int id);

    //OpenGL rendering

    ///
    /// \brief allocate the memory necessary to transfer class datas to VBO datas, done by remakeScene().
    ///
    ///
    void allocateScene();

    ///
    /// \brief transfer class datas to VAO/VBO/EBO datas.
    /// It updates any needed information about the OpenGL state. It also perform a reallocation if necessary.
    /// This function must be called whenever the size of the VBOs or the EBOs increases.
    /// If you know it doesn't, you probably prefer to call updateScene instead.
    ///
    void remakeScene();

    ///
    /// \brief transfer class datas to VAO/VBO/EBO datas.
    /// This shouldn't be used if the organisation of the VBO or the EBO changed. Use remakeScene() if it did.
    ///
    void updateScene();

    ///
    /// \brief draw the scene, using VBO/EBO datas.
    ///
    void drawScene();

    //Non-OpenGL rendering

    ///
    /// \brief myFirstRendering
    ///  a debugging rendering, just to see if my calculations (ray intersection, etc) are correct.
    ///  Fills a pixel with the face's color if there is something, black otherwise.
    ///
    void myFirstRendering();

    ///
    /// \brief phongRendering
    /// uses phong rendering to render each object, with some faces being light sources.
    /// \param quality precision of the shadowing
    /// \param N N*N stochastic tracing (use 0 if you don't want to use stochastic ray tracing)
    ///
    void mainRendering(size_t quality=0, SceneObject::Integral::Type_t typeIntegral=SceneObject::Integral::SINGLE_MEAN,
                        float reflectionAngle=M_PI, unsigned int reflectionQuality=0);

    //Other functions

    SceneObject *operator[](unsigned int i);

    void setObject(unsigned int index, SceneObject* object);
    SceneObject* getObject(unsigned int index);

    void allocateVBOPosition();
    void allocateEBO();

private:

    //render functions
    glm::vec3 lightenMaterialProp(SceneFace_Prop *face, const glm::vec3& positionFace, const glm::vec3 &normalFace,
                                  const glm::vec3 vToEye, size_t quality, SceneObject::Integral::Type_t type=SceneObject::Integral::SINGLE_MEAN);

    glm::vec3 reflectionMaterialProp(SceneFace_Prop *face, const glm::vec3& positionFace,
                                    const glm::vec3& normalFace, const glm::vec3 vToEye,
                                    size_t quality, SceneObject::Integral::Type_t typeIntegral,
                                    float angleReflection, unsigned int reflectionQuality);

    std::map<unsigned int, SceneObject*> m_objects;

    SceneCamera                     m_camera;

    /// OpenGL objects
    GLint                           m_VAOId;
    GLint                           m_VBOPositionId;
    GLint                           m_EBOId;
    GLint                           m_uColorLocation;

    /// VBO/EBO sizes
    GLsizeiptr                      m_VBOPositionSize;
    GLsizeiptr                      m_EBOSize;
    GLint                           m_currentBaseVertex;

    GLsizeiptr                      m_VBOPositionCapacity;
    GLsizeiptr                      m_EBOCapacity;
};

#endif // SCENEMANAGER_H
