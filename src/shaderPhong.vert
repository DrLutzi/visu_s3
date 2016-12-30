#version 330

uniform mat4 u_mtxWorld;
uniform mat4 u_mtxView;
uniform mat4 u_mtxProjection;

uniform mat4 u_mtxNormal;

uniform vec3 u_vCameraPositionWS;
uniform vec3 u_vLightPositionWS;

// get the vertex position, normal, colour as vec3 input - sent from the CPU, VertexBuffer
in vec3 vtx_position;
in vec3 vtx_normal;

// transfer these data to the Fragment Shader !
out vec3 vs_vNormal;
out vec3 vs_vToCamera;
out vec3 vs_vToLight;

void main()
{
    // we are going to do all the light related calculations in WORLD SPACE

    vec4 vWorldPosition = u_mtxWorld * vec4( vtx_position, 1.0f );

    gl_Position = u_mtxProjection * u_mtxView * vWorldPosition;

    // normal in world space
    //vs_vNormal	= ( u_mtxNormal * vec4( vtx_normal, 1.0f ) ).xyz;
    vs_vNormal	= mat3( u_mtxWorld ) * vtx_normal; // -- tweak... faster and simpler... ;)

    // direction from surface point to camera
    vs_vToCamera= u_vCameraPositionWS - vWorldPosition.xyz;

    // direction from surface point to light
    vs_vToLight	= u_vLightPositionWS - vWorldPosition.xyz;
}
