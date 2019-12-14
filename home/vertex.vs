#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1)in vec2 Texcoord;
layout(location = 2)in vec3 Normal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 NormalCoord;
out vec2 TextureCoord;

void main(){
    
    gl_Position=projection*view*model*vec4(aPos.x,aPos.y,aPos.z,1.0);
    NormalCoord = normalize(mat3(transpose(inverse((model))))*Normal);
    TextureCoord = Texcoord;
}