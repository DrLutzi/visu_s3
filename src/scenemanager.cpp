#include "scenemanager.h"
#include <algorithm>


#ifdef USE_QGLVIEWER
SceneManager::SceneManager(qglviewer::Camera &camera, GLint vaoId, GLint vboPositionId, GLint eboId, GLuint colorLocation) :
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

#else

SceneManager::SceneManager(qglviewer_fake::Camera &camera, GLint vaoId, GLint vboPositionId, GLint eboId, GLuint colorLocation) :
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

#endif

void SceneManager::setup()
{
    SceneFace_Prop::MaterialProperties_t matProperties;
    SceneFace_Light::LightProperties_t lightProperties;

    SceneFace_Prop *face1 = new SceneFace_Prop(glm::vec3(-5.0f,-3.0f,0), glm::vec3(1,0,0), glm::vec3(0,1,0), 10, 7);
    face1->setColor(glm::vec3(0.1, 0.4, 0.1));

    //a sort of dark green with average specular power and full opacity.
    matProperties.fSpecularPower=30.0f;
    matProperties.vAmbiant=glm::vec3(0.1, 0.3, 0.1);
    matProperties.vDiffuse=glm::vec3(0.2, 0.4, 0.2);
    matProperties.vSpecular=glm::vec3(0.2, 0.8, 0.2);
    matProperties.fReflectionPower = 0;
    face1->setMaterialProperties(matProperties);

    SceneFace_Prop *face2 = new SceneFace_Prop(glm::vec3(9.0f, -1.0f, 4.0f), glm::vec3(-0.5f, 0, -1.0f), glm::vec3(0,1,0), 13, 13);
    face2->setColor(glm::vec3(0.2,0.2,0.6));

    //some nice blue with high specular power, and a bit of reflection
    matProperties.fSpecularPower=100.0f;
    matProperties.vAmbiant=glm::vec3(0.1, 0.1, 0.3);
    matProperties.vDiffuse=glm::vec3(0.2, 0.2, 0.6);
    matProperties.vSpecular=glm::vec3(0.3, 0.3, 0.8);
    matProperties.fReflectionPower = 0.1;
    face2->setMaterialProperties(matProperties);

    SceneFace_Prop *face3 = new SceneFace_Prop(glm::vec3(-5.0f, -3.0f, 10.0f), glm::vec3(1.0f, 0, 0), glm::vec3(0,0,-1.0f), 10, 10);
    face3->setColor(glm::vec3(0.5, 0.5, 0.5));
    //A bright grey with low specular power, and a great reflection
    matProperties.fSpecularPower=10.0f;
    matProperties.vAmbiant=glm::vec3(0.2, 0.2, 0.2);
    matProperties.vDiffuse=glm::vec3(0.5, 0.5, 0.5);
    matProperties.vSpecular=glm::vec3(0.8, 0.8, 0.8);
    matProperties.fReflectionPower = 0.8;
    face3->setMaterialProperties(matProperties);

    SceneFace_Prop *face4 = new SceneFace_Prop(glm::vec3(0.0f, 1.5f, 4.0f), glm::vec3(1.0f, 0.2f, -0.5f), glm::vec3(-1.0f,0.8f,-0.5f), 1, 1);
    face4->setColor(glm::vec3(0.5, 0.5, 0.2));
    //A small yellow face with very high specular power, some reflection power
    matProperties.fSpecularPower=300.0f;
    matProperties.vAmbiant=glm::vec3(0.2, 0.2, 0.1);
    matProperties.vDiffuse=glm::vec3(0.5, 0.5, 0.2);
    matProperties.vSpecular=glm::vec3(0.8, 0.8, 0.2);
    matProperties.fReflectionPower = 0.4;
    face4->setMaterialProperties(matProperties);

    SceneFace_Light *faceLight = new SceneFace_Light(glm::vec3(-5.0f, -1.0f, 5.0f), glm::vec3(-0.1f, 0, -1.0f), glm::vec3(0,1,0), 2, 2);
    faceLight->setColor(glm::vec3(1.0, 1.0, 1.0));

    //a small, but powerful white light
    lightProperties.vAmbiant=glm::vec3(0.5, 0.5, 0.5);
    lightProperties.vDiffuse=glm::vec3(0.7, 0.7, 0.7);
    lightProperties.vSpecular=glm::vec3(1.0, 1.0, 1.0);

    faceLight->setLightProperties(lightProperties);

    SceneFace_Light *faceLight2 = new SceneFace_Light(glm::vec3(0, 8.0f, 5.0f), glm::vec3(1.0f, 0, 0), glm::vec3(0,1,-1.0f), 2.5f, 2.5f);
    faceLight2->setColor(glm::vec3(0.8, 0.2, 0.8));

    //a slighty bigger soft purple light
    lightProperties.vAmbiant=glm::vec3(0.2, 0.1, 0.2);
    lightProperties.vDiffuse=glm::vec3(0.7, 0.2, 0.7);
    lightProperties.vSpecular=glm::vec3(0.9, 0.2, 0.9);

    faceLight2->setLightProperties(lightProperties);

    append(face1, false);
    append(face2, false);
    append(face3, false);
    append(face4, false);
    append(faceLight, false);
    append(faceLight2, false);
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
        if(object!=NULL)
        {
            object->makeVBOPosition(m_VBOPositionId);
            object->makeEBO(m_EBOId);
        }
    }
}

void SceneManager::drawScene()
{
    for(const_iterator it=begin(); it!=end(); ++it)
    {
        SceneObject *object=(*it).second;
        if(object!=NULL)
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

void SceneManager::mainRendering(size_t quality, SceneObject::Integral::Type_t typeIntegral, float reflectionAngle, unsigned int reflectionQuality)
{
    m_camera.setupRendering();
    int w=m_camera.width();
    int h=m_camera.height();
    for(int x=0; x<w; ++x)
    {
        for(int y=0; y<h; ++y)
        {
            glm::vec3 finalColor(0,0,0);
            Ray firstRay;
            firstRay=m_camera.castRayFromPixel(x,y);
            //try to find the closest hit
            SceneObject::RayHitProperties firstRayHitProperties;
            for(iterator it=begin(); it!=end(); ++it)
            {
                SceneObject *object=(*it).second;
                object->intersectsRay(firstRay, firstRayHitProperties);
            }
            if(firstRayHitProperties.occuredHit) //we found something?
            {
                //is it a material prop?
                SceneFace_Prop *material=dynamic_cast<SceneFace_Prop*>(firstRayHitProperties.objectHit);
                if(material!=NULL)
                {
                    //compute vector to camera
                    glm::vec3 vToEye = glm::normalize(firstRay.origin() - firstRayHitProperties.positionHit);
                    //compute material color...
                    finalColor = lightenMaterialProp(material, firstRayHitProperties.positionHit,
                                                     firstRayHitProperties.normalHit,
                                                     vToEye, quality, typeIntegral);
                    //multiply by its opacity, if this is a thing
                    if(reflectionQuality > 0)
                        finalColor *= (1.0f - material->materialProperties().fReflectionPower);
                    //...and add its reflection color
                    //you'll note the "final rush" functions arguments that could easily be packed inside a convenient structure. Sorry about that.
                    finalColor += reflectionMaterialProp(material, firstRayHitProperties.positionHit,
                                                            firstRayHitProperties.normalHit, vToEye,
                                                        quality, typeIntegral, reflectionAngle, reflectionQuality);
                }
                else //is it a light source?
                {
                    SceneFace_Light *light=dynamic_cast<SceneFace_Light*>(firstRayHitProperties.objectHit);
                    if(light!=NULL)
                        finalColor = glm::clamp(light->lightProperties().vAmbiant + light->lightProperties().vDiffuse + light->lightProperties().vSpecular,
                                                        glm::vec3(0,0,0), glm::vec3(1.0f, 1.0f, 1.0f));
                }
                //else this isn't a suitable object for this rendering, black
            }
            m_camera.setPixelfv(x, y, &finalColor);
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
                                            size_t quality, SceneObject::Integral::Type_t typeIntegral)
{
    //compute how much of the light's surface the hitPoint can see by integrating its surface.
    glm::vec3 finalColor(0,0,0);

    if(face->materialProperties().fReflectionPower < (1.0f-EPSILON) ) {
    for(const_iterator itLight=begin(); itLight!=end(); ++itLight)
    {
        SceneFace_Light *lightSource=dynamic_cast<SceneFace_Light*>((*itLight).second);
        if(lightSource!=NULL)
        {
            glm::vec3 singleFaceLightColor;
            SceneFace::Integral ui(lightSource->beginIntegral(quality, typeIntegral));
            for( ; ui!=lightSource->endIntegral(quality, typeIntegral); lightSource->nextIntegral(ui))
            {
                //grab L and N for elegant writting purposes
                glm::vec3 L=glm::normalize(ui.value-positionFace);
                glm::vec3 N=normalFace;

                //check for obstructions
                //also, we need to start casting the ray a little bit further to avoid unwanted collisions with self
                Ray toLight(positionFace+N*EPSILON, L);
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
                singleFaceLightColor += diffuse+specular;
            }
            //mean of all computed colors
            singleFaceLightColor /= ui.actualSize;
            //add ambiant color
            glm::vec3 ambiant(face->colorAmbiant(*lightSource));
            finalColor += singleFaceLightColor+ambiant;
            //this is our single light source color
        }
    }
    }
    return glm::clamp(finalColor, glm::vec3(0,0,0), glm::vec3(1.0f, 1.0f, 1.0f));
}

glm::vec3 SceneManager::reflectionMaterialProp(SceneFace_Prop *face, const glm::vec3& positionFace,
                                            const glm::vec3& normalFace, const glm::vec3 vToEye,
                                            size_t quality, SceneObject::Integral::Type_t typeIntegral,
                                            float angleReflection, unsigned int reflectionQuality)
{
    glm::vec3 finalColor(0,0,0);
    if(face->materialProperties().fReflectionPower > EPSILON)
    {
        //create the cone of reflexion
        Ray::RandomCone cone;
        cone.direction = glm::reflect(-vToEye, normalFace);
        cone.angle = angleReflection;

        for(unsigned int i=0; i<reflectionQuality; ++i)
        {
            //r will have an updated ray every call
            Ray r(positionFace + normalFace*EPSILON, cone);
            SceneObject::RayHitProperties rayHit;
            for(iterator it=begin(); it!=end(); ++it)
            {
                SceneObject *object=(*it).second;
                object->intersectsRay(r, rayHit);
            }
            //should this ever happen, We're really not interested into reflecting ourselves.
            if(rayHit.occuredHit && rayHit.objectHit!=face)
            {
                finalColor+=face->materialProperties().fReflectionPower *
                        (1.0f - face->materialProperties().fReflectionPower) *
                        lightenMaterialProp(face, rayHit.positionHit,
                                            rayHit.normalHit, -r.direction(),
                                            quality, typeIntegral);
            }
        }
    }
    return reflectionQuality > 1 ? finalColor/((float)reflectionQuality) : finalColor;
}
