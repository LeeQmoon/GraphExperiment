#version 430 core
layout(location = 0) in vec3 aPos;
layout(location = 1)in vec2 Texcoord;
layout(location = 2)in vec3 Normal;

out vec2 TextureCoord;
out vec3 NormalCoord;
out vec4 position;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    position = view*model*vec4(aPos,1.0);
    gl_Position=projection*view*model*vec4(aPos.x,aPos.y,aPos.z,1.0);
    //NormalCoord = normalize(Normal);
    NormalCoord = normalize(mat3(transpose(inverse((view*model))))*Normal);
    //NormalCoord = vec3(1.0,1.0,1.0);
    TextureCoord = Texcoord;
}