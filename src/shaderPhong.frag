#version 330

// Data we receive from the previous Shader Unit - here, the Vertex Shader Unit
in vec3 vs_vNormal;
in vec3 vs_vToCamera;
in vec3 vs_vToLight;

uniform vec3 u_vColour;

// out color value used to fill in the Framebuffer
out vec4 out_fragColor;

struct LightProperties
{
    vec3   vAmbiant;
    vec3   vDiffuse;
    vec3   vSpecular;
};
uniform LightProperties u_light;

struct MaterialProperties
{
    vec3   vAmbiant;
    vec3   vDiffuse;
    vec3   vSpecular;
    float  fSpecPower;
};
uniform MaterialProperties u_material;


// returns intensity of reflected ambient lighting
vec3 ambientLighting()
{
    return u_light.vAmbiant;
}


// returns intensity of diffuse reflection
vec3 diffuseLighting( in vec3 N, in vec3 L )
{
    // calculation as for Lambertian reflection
    float NdotL = dot( N, L );
    float diffuseTerm = clamp( NdotL, 0, 1 );

    return u_light.vDiffuse * diffuseTerm;
}


// returns intensity of specular reflection
vec3 specularLightingGouraudPhong( in vec3 N, in vec3 L, in vec3 V )
{
    // Light vector reflected by the Normal
    vec3 R = reflect( -L, N );

    float specularTerm = pow( max( dot( R, V ), 0.f ), u_material.fSpecPower );

    return u_material.vSpecular * u_light.vSpecular * specularTerm;
}



//////////////////////////////////////////////////////////////////////
void main(void)
{
   // re-normalize vectors after interpolation - THIS IS REQUIRED !
    vec3 L = normalize(vs_vToLight);
    vec3 V = normalize(vs_vToCamera);
    vec3 N = normalize(vs_vNormal);

   // get reflectance components
   vec3 Iamb = ambientLighting();
   vec3 Idif = diffuseLighting( N, L );
   vec3 Ispe = specularLightingGouraudPhong( N, L, V );
   vec3 Iemi = vec3(0,0,0);

    out_fragColor=vec4(Iemi+Iamb*u_vColour+Idif*u_vColour+Ispe,1);
}
