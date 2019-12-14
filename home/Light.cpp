#include "Light.h"
#include<iostream>
using namespace std;


Light::Light()
{
	count = 0;
	initLight();
}
void Light::initLight() {
	openSpot = 0;
	Ambient = glm::vec3(1.0,1.0,1.0);
	LightColor = glm::vec3(1.0, 1.0, 1.0);//平行光
	LightPosition = glm::vec3(8.0f, 8.0f, 8.0f);
	//点光
	spotLight[0] = glm::vec3(4.0107, 9.2170, -4.5086);
	spotColor[0] = glm::vec3(0.3,0.3,0.3);
	spotLight[1] = glm::vec3(3.8851, 9.3665, 27.3500);
	spotColor[1] = glm::vec3(0.3, 0.3, 0.3);
	spotLight[2] = glm::vec3(-1.7702, 9.8142, 27.2124);
	spotColor[2] = glm::vec3(0.3, 0.3, 0.3);

	ConstantAttenuation = 0.2;
	LinearAttenuation = 0.3;
	QuadraticAttenuation = 0.4;
}

void Light::processLight(GLuint shaderprogram) {
	if (count <= 5000&&count>=4000) {
		//cout << "ttt" << endl;
		openSpot = 1;
		LightColor = glm::vec3(1.0, 1.0, 1.0);
		Ambient = glm::vec3(0.2, 0.2, 0.2);
		count++;
		if (count == 5000)
			count = 0;
	}
	else if(count>=0&&count<4000){
		LightColor.x -= 0.001;
		LightColor.y -= 0.001;
		LightColor.z -= 0.001;
		openSpot = 0;
		count++;
		Ambient = glm::vec3(1.0, 1.0, 1.0);
	}
		
	GLuint ambientID = glGetUniformLocation(shaderprogram, "light.Ambient");
	GLuint lightColorID = glGetUniformLocation(shaderprogram, "light.LightColor");
	GLuint lightPosition = glGetUniformLocation(shaderprogram, "light.LightPosition");
	GLuint constantAttenuationID = glGetUniformLocation(shaderprogram, "light.ConstantAttenuation");
	GLuint linearAttenuationID = glGetUniformLocation(shaderprogram, "light.LinearAttenuation");
	GLuint quadraticAttenuationID = glGetUniformLocation(shaderprogram, "light.QuadraticAttenuation");
	GLuint spotOne = glGetUniformLocation(shaderprogram, "light.spotLight[0]");
	GLuint spotOneColor = glGetUniformLocation(shaderprogram, "light.spotColor[0]");
	GLuint spotTwo = glGetUniformLocation(shaderprogram, "light.spotLight[1]");
	GLuint spotTwoColor = glGetUniformLocation(shaderprogram, "light.spotColor[1]");
	GLuint spotThree = glGetUniformLocation(shaderprogram, "light.spotLight[2]");
	GLuint spotThreeColor = glGetUniformLocation(shaderprogram, "light.spotColor[2]");
	GLuint open = glGetUniformLocation(shaderprogram, "light.openSpot");
	glUniform3fv(spotOne, 1, &spotLight[0][0]);
	glUniform3fv(spotOneColor, 1, &spotColor[0][0]);
	glUniform3fv(spotTwo, 1, &spotLight[1][0]);
	glUniform3fv(spotTwoColor, 1, &spotColor[1][0]);
	glUniform3fv(spotThree, 1, &spotLight[2][0]);
	glUniform3fv(spotThreeColor, 1, &spotColor[2][0]);
	glUniform1i(open, openSpot);
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
