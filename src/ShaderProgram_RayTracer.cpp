#include "ShaderProgram_RayTracer.h"

ShaderProgram_RayTracer::ShaderProgram_RayTracer()
{
    // load & compile & link shaders
    load("shader.vert","shader.frag");

    getUniformLocations();
}

/// Creates the VertexBufferObject that contains the geometry position and the EBO with the indexes
void ShaderProgram_RayTracer::createVBO()
{
    //VBO
    glGenBuffers(1, &vboPositionId);

    //EBO
    glGenBuffers(1, &eboId);
}

/// Creates the VertexArrayObject that contains the geometry info
void ShaderProgram_RayTracer::createVAOFromVBO()
{
    // genere 1 VAO
    glGenVertexArrays(1, &vaoId);

    // on travaille sur celui-ci
    glBindVertexArray(vaoId);

    //associe l'ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);

    // associe le VBO de position
    glBindBuffer(GL_ARRAY_BUFFER, vboPositionId);
    // avec l'attribut position du shader
    glEnableVertexAttribArray(idOfPositionAttribute);
    // en donne les bon parametre (idAttrib, 3 x float / sommets, pas de normalisation,
    // et 0, 0 : on ne saute rien et on commence au debut)
    glVertexAttribPointer(idOfPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //vao fini
    glBindVertexArray(0);
}

/// Destroys the VertexArrayObject created earlier
void ShaderProgram_RayTracer::destroyVAOAndVBO()
{
    glDeleteBuffers(1, &vboPositionId);
    glDeleteBuffers(1, &eboId);

    glDeleteVertexArrays(1, &vaoId);
}

/// Get the GLSL uniform locations
void ShaderProgram_RayTracer::getUniformLocations()
{
    // get id of uniforms
    idOfProjectionMatrix =  glGetUniformLocation(m_programId, "u_mtxProjection");
    idOfViewMatrix =        glGetUniformLocation(m_programId, "u_mtxView");
    idOfColor =             glGetUniformLocation(m_programId, "u_vColor");
}

/// Get the GLSL vertex attributes locations
void ShaderProgram_RayTracer::bindAttributeLocations()
{
    // set id of attribute and bind
    glBindAttribLocation(m_programId, idOfPositionAttribute, "vtx_position");
}
