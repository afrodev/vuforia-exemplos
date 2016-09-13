
const char* SimpleLightningFragmentShader = MAKESTRING(
precision mediump float;

uniform vec3 AmbientMaterial;
uniform vec3 SpecularMaterial;
uniform vec3 DiffuseMaterial;
uniform vec3 EmissiveMaterial;

uniform float Alpha;
uniform int LightOn;

varying vec2 texCoord;
                                            
uniform sampler2D texSampler2D;
uniform int texCount;

varying float diffuseFactor;
varying float shininessFactor;
                                                 
void main(void)
{

   vec3 shadedColor = AmbientMaterial + diffuseFactor * DiffuseMaterial + shininessFactor * SpecularMaterial + EmissiveMaterial; 
   
    if(texCount > 0)
    {
        vec3 color = texture2D(texSampler2D, texCoord).rgb;
        float tex_alpha = texture2D(texSampler2D, texCoord).a;
        gl_FragColor = vec4(color, tex_alpha * Alpha);
    }
    else
    if(LightOn >0)
        gl_FragColor = vec4(shadedColor, Alpha);
    else
        gl_FragColor = vec4(DiffuseMaterial, Alpha);
}
                                                 
);
