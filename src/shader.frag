#version 330

uniform vec3 u_vColor;

out vec4 out_fragColor;


void main()
{
     out_fragColor=vec4(u_vColor, 0.0);
}
