#include "scenemanager.h"
#include <algorithm>

SceneManager::SceneManager(QGLVIEWER_NAMESPACE::Camera &camera, GLint vaoId, GLint vboPositionId, GLint eboId, GLuint colorLocation) :
    m_objects(),
    m_camera(camera),
    m_VAOId(vaoId),
    m_VBOPositionId(vboPositionId),
    m_EBOId(eboId),
    m_VBOPositionSize(0),
    m_EBOSize(0),
    m_currentBaseVertex(0),
    m_VBOPositionCapacity(0),
    m_EBOCapacity(0)
{
    SceneObject::setColorLocation(colorLocation);
}

void SceneManager::append(SceneObject* object, bool reallocate)
{
    if(object!=NULL)
    {
        m_objects.insert(std::pair<unsigned int, SceneObject*>(object->id(), object));

        //the indexes where we finished writting
        GLsizeiptr firstVBOPos=m_VBOPositionSize;
        GLsizeiptr firstEBO=m_EBOSize;
        GLsizeiptr baseVertex=m_currentBaseVertex;

        m_VBOPositionSize   +=      object->sizeVBOPosition();
        m_EBOSize           +=      object->sizeEBO();
        m_currentBaseVertex +=      object->numberAttributes();

        if(reallocate && (m_VBOPositionCapacity<m_VBOPositionSize || m_EBOCapacity<m_EBOSize))
        {
            //remake the whole scene
            remakeScene();
        }
        else
        {
            //set first indexes and base vertex only
            object->setFirstVBOPosition(firstVBOPos);
            object->setFirstEBO(firstEBO);
            object->setBaseVertexEBO(baseVertex);
        }
    }
}

void SceneManager::append(SceneFace_Light *object, bool reallocate)
{
    append((SceneObject *)object, reallocate);
    m_lightSource = object;
}

SceneObject *SceneManager::remove(unsigned int id)
{
    iterator position=m_objects.find(id);
    if(position!=m_objects.end())
    {
        SceneObject *removedPtr=(*position).second;
        m_objects.erase(position);
        remakeScene();

        return removedPtr;
    }
    else
        WARNING("SceneManager - remove: id wasn't found");
    return NULL;
}

void SceneManager::allocateScene()
{
    allocateVBOPosition();
    allocateEBO();
}

void SceneManager::remakeScene()
{
    //first check if the desired size of the scene is bigger than its current maximum size and update first indexes
    m_VBOPositionSize   =   0;
    m_EBOSize           =   0;
    m_currentBaseVertex =   0;
    for(iterator it=begin(); it!=end(); ++it)
    {
        SceneObject* object=(*it).second;

        if(object!=NULL)
        {
            object->setFirstVBOPosition (m_VBOPositionSize);
            object->setFirstEBO         (m_EBOSize);
            object->setBaseVertexEBO    (m_currentBaseVertex);

            m_VBOPositionSize   +=      object->sizeVBOPosition();
            m_EBOSize           +=      object->sizeEBO();
            m_currentBaseVertex +=      object->numberAttributes();
        }
    }

    //re-allocate if the new size of the scene is bigger than the old size
    if(m_VBOPositionSize>m_VBOPositionCapacity || m_VBOPositionCapacity > 2*m_VBOPositionSize)
    {
        allocateVBOPosition();
    }

    //do that for the EBO, too
    if(m_EBOSize>m_EBOCapacity || m_EBOCapacity > 2*m_EBOSize)
    {
        allocateEBO();
    }

    //finally, let classes fill whatever information they want in their VBOs or EBOs.
    updateScene();
}

void SceneManager::updateScene()
{
    for(iterator it=begin(); it!=end(); ++it)
    {
        SceneObject *object=(*it).second;
        object->makeVBOPosition(m_VBOPositionId);
        object->makeEBO(m_EBOId);
    }
}

void SceneManager::drawScene()
{
    for(const_iterator it=begin(); it!=end(); ++it)
    {
        SceneObject *object=(*it).second;
        object->draw();
    }
}

//Non-OpenGL rendering

void SceneManager::myFirstRendering()
{
    m_camera.setupRendering();
    int w=m_camera.width();
    int h=m_camera.height();

    for(int x=0; x<w; ++x)
    {
        for(int y=0; y<h; ++y)
        {
            Ray r=m_camera.castRayFromPixel(x,y);
            SceneObject::RayHitProperties hitProperties;
            for(iterator it=begin(); it!=end(); ++it)
            {
                SceneObject *object=(*it).second;
                object->intersectsRay(r, hitProperties);
            }
            if(hitProperties.occuredHit)
            {
                m_camera.setPixelfv(x, y, &hitProperties.objectHit->color());
            }
            else
            {
                m_camera.setPixelf(x, y, 0, 0, 0);
            }
        }
    }
    m_camera.showBeautifulRender();
}

void SceneManager::phongRendering(size_t N)
{
    if(m_lightSource==NULL)
        ERROR("Why would you try a phong rendering without a light source?");
    m_camera.setupRendering();
    int w=m_camera.width();
    int h=m_camera.height();

    for(int x=0; x<w; ++x)
    {
        for(int y=0; y<h; ++y)
        {
            //cast ray through the pixel, from the position of the eye
            Ray r=m_camera.castRayFromPixel(x,y);
            //try to find the closest hit
            for(iterator it=begin(); it!=end(); ++it)
            {
                SceneObject::RayHitProperties rayHitProperties;
                for(iterator it=begin(); it!=end(); ++it)
                {
                    SceneObject *object=(*it).second;
                    object->intersectsRay(r, rayHitProperties);
                }
                if(rayHitProperties.occuredHit) //we found something?
                {
                    //is it a material prop?
                    SceneFace_Prop *material=dynamic_cast<SceneFace_Prop*>(rayHitProperties.objectHit);
                    if(material!=NULL)
                    {
                        //compute how much of the light's surface the hitPoint can see by integrating its surface.
                        glm::vec3 finalColor;
                        SceneFace::UniformIntegral ui(m_lightSource->beginUniformIntegral(N));
                        for(ui; ui!=m_lightSource->endUniformIntegral(N); m_lightSource->nextUniformIntegral(ui))
                        {
                            //grab L and N for elegant writting purposes
                            glm::vec3 L=glm::normalize(ui.value-rayHitProperties.positionHit);
                            glm::vec3 N=rayHitProperties.normalHit;

                            //check for obstructions
                            //also, TODO : we'll probably have to start a bit further following the normal.
                            Ray toLight(rayHitProperties.positionHit, L);
                            SceneObject::RayHitProperties secondRayHitProperties;
                            for(iterator it=begin(); it!=end(); ++it)
                            {
                                SceneObject *object=(*it).second;
                                //we're not interested by hitting the light.
                                if(object->id() != m_lightSource->id())
                                    object->intersectsRay(r, secondRayHitProperties);
                            }
                            if(!secondRayHitProperties.occuredHit) //no obstruction found?
                            {//we need to increment the light of this pixel.
                                float NdotL = glm::dot(L, N);
                                float diffuseTerm = 0;//std::clamp( NdotL, 0, 1 );
                            }

                        }
                    }
                    else //is it a light source?
                    {
                        SceneFace_Light *light=dynamic_cast<SceneFace_Light*>(rayHitProperties.objectHit);

                    }
                }
                else
                    m_camera.setPixelf(x, y, 0, 0, 0);
            }
        }
    }
}

SceneObject *SceneManager::operator[](unsigned int i)
{
    return m_objects[i];
}

void SceneManager::setObject(unsigned int index, SceneObject* object)
{
    m_objects.at(index)=object;
}

SceneObject* SceneManager::getObject(unsigned int index)
{
    return m_objects.at(index);
}

void SceneManager::allocateVBOPosition()
{
    //simple memory allocation
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositionId);
    glBufferData(GL_ARRAY_BUFFER, m_VBOPositionSize, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_VBOPositionCapacity=m_VBOPositionSize;
}

void SceneManager::allocateEBO()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_EBOSize, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_EBOCapacity=m_EBOSize;
}

