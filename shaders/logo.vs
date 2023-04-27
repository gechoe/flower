#version 400

layout (location = 0) in vec3 vPos; // the p to use in the specular equation
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vTextureCoords;

uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform bool HasUV;

out float intensity;

void main()
{
   vec4 lightPos; // light position
   vec4 eyePos; // eye position
   vec3 normLight; // normalized light, direction of light
   vec3 normal;

   lightPos = vec4(15, 15, 15, 1);
   eyePos = ModelViewMatrix * vec4(vPos, 1.0);
   normLight = normalize(vec3(lightPos - eyePos));
   normal = normalize(vec3(NormalMatrix * vNormals)); 
   intensity = dot(normal, normLight); // calculates intensity value

   gl_Position = MVP * vec4(vPos, 1.0);
}