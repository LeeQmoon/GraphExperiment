#version 430 core
uniform int texturecount[2];
in vec3 NormalCoord;
in vec2 TextureCoord;
uniform sampler2D tex0;
uniform sampler2D tex1;
struct Material{
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Ns;
};
uniform Material material;
uniform vec3 Ambient;
uniform vec3 LightColor;
uniform vec3 LightPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float ConstantAttenuation;
uniform float LinearAttenuation;
uniform float QuadraticAttenuation;

void main(){
    vec3 LightDirection = normalize(LightPosition);
    //vec3 EyeDirection = vec3(-position);
    
   // vec3 H = normalize(LightDirection+EyeDirection);
   // float Specular = max(0.0,dot(NormalCoord,H));
    //float Specos=pow(max(Specular,0.0),material.Ns);
    
    float attenuation =1.0;
    float diffuse = max(0.0,dot(NormalCoord,LightDirection));
   // vec3 scatteredLight = material.Ka*Ambient+LightColor*material.Kd*diffuse*attenuation;
    //vec3 reflect = material.Ks*Specos;
   // if(texturecount[0]==1 && texturecount[1] == 1){
        //vec3 LightDirection = normalize(vec3(vec4(LightPosition,1.0)));
        vec4 tempColor1 = texture(tex0,TextureCoord);
        vec4 tempColor2 = texture(tex1,TextureCoord);
        vec3 rgb = min(tempColor1.xyz*material.Ka*Ambient+tempColor2.xyz*LightColor*material.Kd*diffuse*attenuation,vec3(1.0));
        gl_FragColor = vec4(rgb,1.0);
   // }
    
}