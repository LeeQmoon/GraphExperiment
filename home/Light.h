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
	glm::vec3 LightPosition;
	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;
	Light();
	void initLight();
	void processLight(GLuint shaderprogram);
	~Light();
};

