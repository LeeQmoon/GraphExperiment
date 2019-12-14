#include "Light.h"



Light::Light()
{
	initLight();
}
void Light::initLight() {
	Ambient = glm::vec3(1.0, 1.0, 1.0);
	LightColor = glm::vec3(1.0, 1.0, 1.0);
	LightPosition = glm::vec3(0.0f, 4.0f, 4.0f);
	ConstantAttenuation = 0.2;
	LinearAttenuation = 0.3;
	QuadraticAttenuation = 0.4;
}

void Light::processLight(GLuint shaderprogram) {
	GLuint ambientID = glGetUniformLocation(shaderprogram, "Ambient");
	GLuint lightColorID = glGetUniformLocation(shaderprogram, "LightColor");
	GLuint lightPosition = glGetUniformLocation(shaderprogram, "LightPosition");
	GLuint constantAttenuationID = glGetUniformLocation(shaderprogram, "ConstantAttenuation");
	GLuint linearAttenuationID = glGetUniformLocation(shaderprogram, "LinearAttenuation");
	GLuint quadraticAttenuationID = glGetUniformLocation(shaderprogram, "QuadraticAttenuation");
	glUniform3fv(ambientID, 1, &Ambient[0]);
	glUniform3fv(lightColorID, 1, &LightColor[0]);
	glUniform3fv(lightPosition, 1, &LightPosition[0]);
	glUniform1f(constantAttenuationID, ConstantAttenuation);
	glUniform1f(linearAttenuationID, LinearAttenuation);
	glUniform1f(quadraticAttenuationID, QuadraticAttenuation);
}

Light::~Light()
{
}
