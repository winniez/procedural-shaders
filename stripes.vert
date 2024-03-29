uniform vec3 LightPosition; 
uniform vec3 LightColor; 
uniform vec3 EyePosition; 
uniform vec3 Specular; 
uniform vec3 Ambient;
uniform float Kd;
uniform mat4 MVMatrix; 
uniform mat4 MVPMatrix; 
uniform mat3 NormalMatrix;

attribute vec4 MCVertex;
attribute vec3 MCNormal;
attribute vec2 TexCoord0;

varying vec3 DiffuseColor;
varying vec3 SpecularColor;
varying float TexCoord;

void main() {
vec3 ecPosition = vec3(MVMatrix * MCVertex);

vec3 tnorm = normalize(NormalMatrix * MCNormal);
vec3 lightVec = normalize(LightPosition - ecPosition);
vec3 viewVec = normalize(EyePosition - ecPosition);
vec3 hvec = normalize(viewVec + lightVec);

float spec = clamp(dot(hvec, tnorm), 0.0, 1.0); 
spec = pow(spec, 16.0);

DiffuseColor = LightColor * vec3(Kd * dot(lightVec, tnorm));
DiffuseColor = clamp(Ambient + DiffuseColor, 0.0, 1.0);
SpecularColor = clamp((LightColor * Specular * spec), 0.0, 1.0);
TexCoord  = TexCoord0.t;
gl_Position = MVPMatrix * MCVertex;

}