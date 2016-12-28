#ifndef SHADERPROGRAM_RAYTRACER_H
#define SHADERPROGRAM_RAYTRACER_H

#include <stdio.h>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include "ShaderProgram.h"



class ShaderProgram_RayTracer: public ShaderProgram
{
public:
    /// uniform id pour matrice de projection
    GLint idOfProjectionMatrix;

    /// uniform id pour matrice de model-view
    GLint idOfViewMatrix;

    /// uniform id pour couleur
    GLint idOfColor;

    /// attribute id
    GLuint idOfVertexAttribute;

    ShaderProgram_RayTracer();

};

#endif // SHADERPROGRAM_RAYTRACER_H
