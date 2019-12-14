#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Light
{
public:
	glm::vec3 Ambient;
	glm::vec3 LightColor;
	glm::vec3 LightPosition;//平行光
	int openSpot;//是否打开点光
	glm::vec3 spotLight[3];//3个点光位置
	glm::vec3 spotColor[3];//3个点光的颜色
	int count = 0;//计时器
	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;
	Light();
	void initLight();
	void processLight(GLuint shaderprogram);
	~Light();
};

