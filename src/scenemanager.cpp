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

void SceneManager::phongRendering(size_t quality)
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
            Ray firstRay=m_camera.castRayFromPixel(x,y);
            //try to find the closest hit
            for(iterator it=begin(); it!=end(); ++it)
            {
                SceneObject::RayHitProperties firstRayHitProperties;
                for(iterator it=begin(); it!=end(); ++it)
                {
                    SceneObject *object=(*it).second;
                    object->intersectsRay(firstRay, firstRayHitProperties);
                }
                if(firstRayHitProperties.occuredHit) //we found something?
                {
                    glm::vec3 finalColor(0,0,0);
                    //is it a material prop?
                    SceneFace_Prop *material=dynamic_cast<SceneFace_Prop*>(firstRayHitProperties.objectHit);
                    if(material!=NULL)
                    {
                        finalColor = lightenMaterialProp(material, firstRayHitProperties.positionHit, firstRayHitProperties.normalHit,
                                                         glm::normalize(firstRay.origin() - firstRayHitProperties.positionHit), quality);
                    }
                    else //is it a light source?
                    {
                        SceneFace_Light *light=dynamic_cast<SceneFace_Light*>(firstRayHitProperties.objectHit);
                        if(light!=NULL)
                        {
                            finalColor = light->lightProperties().vAmbiant
                                    + light->lightProperties().vDiffuse
                                    + light->lightProperties().vSpecular;
                        }
                    }
                    m_camera.setPixelfv(x, y, &finalColor);
                }
                else
                    m_camera.setPixelf(x, y, 0, 0, 0);
            }
        }
    }
    m_camera.showBeautifulRender();
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

//render functions

glm::vec3 SceneManager::lightenMaterialProp(SceneFace_Prop *face, const glm::vec3& positionFace,
                                            const glm::vec3& normalFace, const glm::vec3 vToEye,
                                            size_t quality)
{
    //compute how much of the light's surface the hitPoint can see by integrating its surface.
    glm::vec3 finalColor;

    for(const_iterator itLight=begin(); itLight!=end(); ++itLight)
    {
        SceneFace_Light *lightSource=dynamic_cast<SceneFace_Light*>((*itLight).second);
        if(lightSource!=NULL)
        {
            glm::vec3 singleFaceLightColor;
            SceneFace::UniformIntegral ui(lightSource->beginUniformIntegral(quality));
            for(ui; ui!=lightSource->endUniformIntegral(quality); lightSource->nextUniformIntegral(ui))
            {
                //grab L and N for elegant writting purposes
                glm::vec3 L=glm::normalize(ui.value-positionFace);
                glm::vec3 N=normalFace;

                //check for obstructions
                //also, we need to start casting the ray a little bit further to avoid unwanted collisions with self
                Ray toLight(positionFace+L*EPSILON, L);
                SceneObject::RayHitProperties secondRayHitProperties;
                for(iterator it=begin(); it!=end(); ++it)
                {
                    SceneObject *object=(*it).second;
                    //we're not interested by hitting the light.
                    if(object->id() != lightSource->id())
                        object->intersectsRay(toLight, secondRayHitProperties);
                }
                glm::vec3 diffuse, specular;
                if(!secondRayHitProperties.occuredHit) //no obstruction found?
                {//we need to increment the light of this pixel.
                    diffuse = face->colorDiffuse(*lightSource, N, L);
                    specular = face->colorSpecular(*lightSource, N, L, vToEye);
                }
                singleFaceLightColor += glm::clamp(diffuse+specular, glm::vec3(0,0,0), glm::vec3(1.0f, 1.0f, 1.0f));
            }
            //mean of all computed colors
            singleFaceLightColor /= ui.actualSize;
            //add ambiant color
            glm::vec3 ambiant(face->colorAmbiant(*m_lightSource));
            finalColor += singleFaceLightColor+ambiant;
            //this is our single light source color
        }
    }
    //clamp to keep the colors between 0 and 1
    return glm::clamp(finalColor, glm::vec3(0,0,0), glm::vec3(1.0f, 1.0f, 1.0f));
}
