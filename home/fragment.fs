#version 430 core
uniform int texturecount[2];
in vec3 NormalCoord;
in vec2 TextureCoord;
in vec4 position;
uniform sampler2D tex0;
uniform sampler2D tex1;
struct Material{
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Ns;
    vec3 Emission;
};
uniform Material material;

struct Light{
    vec3 Ambient;
    vec3 LightColor;
    vec3 LightPosition;
    int openSpot;
    vec3 spotLight[3];
    vec3 spotColor[3];
    float ConstantAttenuation;
    float LinearAttenuation;
    float QuadraticAttenuation;
};
uniform Light light;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    vec4 tempColor1 = texture(tex0,TextureCoord);
    vec4 tempColor2 = texture(tex1,TextureCoord);
    vec3 rgb = vec3(0.0,0.0,0.0);
    if(light.openSpot == 1){
        for(int i=0;i<3;i++){
            vec3 spotDirection = normalize(vec3(model*vec4(light.spotLight[i],1.0)-position));
            float Diff = max(0.0,dot(NormalCoord,spotDirection));
            rgb += min(tempColor1.xyz*material.Ka*light.Ambient+tempColor2.xyz*light.spotColor[i]*material.Kd*Diff,vec3(1.0));
        } 
        rgb = min(material.Emission+rgb,vec3(1.0));
        gl_FragColor = vec4(rgb,1.0);
    }
    else{
        vec3 LightDirection = normalize(light.LightPosition);
        //float attenuation =1.0;
        float diffuse = max(0.0,dot(NormalCoord,LightDirection));
        rgb = min(tempColor1.xyz*material.Ka*light.Ambient+tempColor2.xyz*light.LightColor*material.Kd*diffuse,vec3(1.0));
        gl_FragColor = vec4(rgb,1.0);
    }
}