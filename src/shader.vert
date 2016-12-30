#version 330

//Every given coordinate is already in World coordinate.
//The program is too simple to be bothered by Object matrixes.

in vec3 vtx_position;

uniform mat4 u_mtxProjection;
uniform mat4 u_mtxView;

void main()
{
        gl_Position = u_mtxProjection * u_mtxView * vec4(vtx_position, 1.0); //this is in View
}
