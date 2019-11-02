#version 330 core
in vec2 TextureCoord;
in vec3 NormalCoord;
in vec4 position;

uniform sampler2D texture1;
uniform vec3 Ambient;
uniform vec3 LightColor;
uniform vec3 LightPosition;

uniform float ConstantAttenuation;
uniform float LinearAttenuation;
uniform float QuadraticAttenuation;

void main(){
    vec3 LightDirection = LightPosition-vec3(position);
    float LightDistance = length(LightDirection);
    LightDirection = LightDirection/LightDistance;
    //float attenuation = 1.0/(ConstantAttenuation+LinearAttenuation*LightDistance+QuadraticAttenuation*LightDistance*LightDistance);
    float attenuation =1.0;
   float diffuse = max(0.0,dot(NormalCoord,LightDirection));
    vec3 scatteredLight = Ambient+LightColor*diffuse*attenuation;
    vec4 tempColor = texture(texture1,TextureCoord);
   vec3 rgb = min(tempColor.rgb*scatteredLight,vec3(1.0));
   gl_FragColor = vec4(rgb,1.0);
}