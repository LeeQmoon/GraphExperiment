#version 430 core
in vec2 TextureCoord;
in vec3 NormalCoord;
in vec4 position;

uniform int texturecount[2];

uniform sampler2D texture1;
uniform sampler2D texture2;

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
     vec3 LightDirection = normalize(vec3(view*vec4(LightPosition,1.0)-position));
    vec3 spec=vec3(0.2,0.2,0.2);
    float shininess = 96.078431;
    vec3 H = normalize(vec3((LightDirection,1.0)-position));
    float Specular = max(0.0,dot(NormalCoord,H));
    float Specu=max(pow(Specular,shininess),0.0);
    float attenuation =1.0;
   float diffuse = max(0.0,dot(NormalCoord,LightDirection));
    vec3 scatteredLight = Ambient+LightColor*diffuse*attenuation;
    vec3 reflect = spec*Specu;
    if(texturecount[0]==1 && texturecount[1] == 1){
        //vec3 LightDirection = normalize(vec3(vec4(LightPosition,1.0)));
        vec4 tempColor1 = texture(texture1,TextureCoord);
        vec4 tempColor2 = texture(texture2,TextureCoord);
        vec3 rgb = min(tempColor1.xyz*scatteredLight+tempColor2.xyz*reflect,vec3(1.0));
        gl_FragColor = vec4(rgb,1.0);
    }
    if(texturecount[0]==1 && texturecount[1] == 0){
        vec4 tempColor1 = texture(texture1,TextureCoord);
        vec3 rgb = min(tempColor1.xyz*scatteredLight+reflect,vec3(1.0));
        gl_FragColor = vec4(rgb,1.0);
    }
}