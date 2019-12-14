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
	glm::vec3 LightPosition;//ƽ�й�
	int openSpot;//�Ƿ�򿪵��
	glm::vec3 spotLight[3];//3�����λ��
	glm::vec3 spotColor[3];//3��������ɫ
	int count = 0;//��ʱ��
	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;
	Light();
	void initLight();
	void processLight(GLuint shaderprogram);
	~Light();
};

