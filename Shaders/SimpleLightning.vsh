
const char* SimpleLightningVertexShader = MAKESTRING(
precision mediump float;
                                               
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform vec3 LightDirection;
uniform float Shininess;
                                       
attribute vec4 vertexPosition;
attribute vec4 vertexNormal;
attribute vec2 vertexTexCoord;
                                               
varying vec2 texCoord;
varying float diffuseFactor;
varying float shininessFactor;

void main(void)
{
    
    gl_Position = modelViewProjectionMatrix * vertexPosition;
    vec4 vnormal = modelViewMatrix * vec4(vertexNormal.xyz, 0.0);
    vec3 normal = normalize(vnormal.xyz);

   diffuseFactor = max(0.0, dot(normal, LightDirection));
   
   vec3 E = vec3(0, 0, 1);
   vec3 H = normalize(LightDirection - E);
   shininessFactor = max(0.0, dot(normal, H));
   shininessFactor = pow(shininessFactor, Shininess);

    texCoord = vertexTexCoord;

});
