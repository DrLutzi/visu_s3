#ifndef SHADERPROGRAM_RAYTRACER_H
#define SHADERPROGRAM_RAYTRACER_H

#include <stdio.h>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"



class ShaderProgram_RayTracer: public ShaderProgram
{
public:

    ShaderProgram_RayTracer();

    /// Creates the VertexBufferObject that contains the geometry position and the EBO with the indexes
    void createVBO();

    /// Creates the VertexArrayObject that contains the geometry info
    void createVAOFromVBO();

    /// Destroys the VertexArrayObject created earlier
    void destroyVAOAndVBO();

    /// Get the GLSL uniform locations
    void getUniformLocations();

    /// Get the GLSL vertex attributes locations
    void bindAttributeLocations();

    /// id de VAO
    GLuint vaoId;
    /// id de VBO pour les positions
    GLuint vboPositionId;
    /// id de EBO pour desiner les primitives
    GLuint eboId;
    /// shader prg

    /// uniform Id for projection matrix
    GLint idOfProjectionMatrix;

    /// uniform Id for model-view matrix
    GLint idOfViewMatrix;

    /// uniform Id for colour
    GLint idOfColor;

    GLint idOfLightPosition;

    GLint idOfCameraPosition;

    //TODO

    /// vertex attribute Id for positions
    GLuint idOfPositionAttribute;
};

#endif // SHADERPROGRAM_RAYTRACER_H
