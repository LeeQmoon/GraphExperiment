#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Camera.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
using namespace std;
//���ɰ��ⲿ�ַ���camera.h�У������ض���
GLdouble currenttime = 0.0f;
GLdouble lasttime = 0.0f;
GLdouble deltatime = 0.0f;

//ŷ����ʵ�֣��۲����
glm::mat4 Camera::getView() {
	////glm::vec3 Row1 = { cos(glm::radians(yaw)),sin(glm::radians(yaw))*sin(glm::radians(pitch)),-sin(glm::radians(yaw))*cos(glm::radians(pitch)) };
	////glm::vec3 Row2 = { 0,cos(glm::radians(pitch)),sin(glm::radians(pitch)) };
	////glm::vec3 Row3 = { sin(glm::radians(yaw)),-cos(glm::radians(yaw))*sin(glm::radians(pitch)),cos(glm::radians(yaw))*cos(glm::radians(pitch)) };
	//////glm::mat4 view(
	//////	glm::vec4(Row1,glm::dot(Row1,-position)),
	//////	glm::vec4(Row2,glm::dot(Row2,-position)),
	//////	glm::vec4(Row3,glm::dot(Row3,-position)),
	//////	glm::vec4(0,0,0,1)
	//////);//������ǵ����ת��������glm��������ô����ת��
	////this->view = view;
	////return glm::transpose(view);

	glm::vec3 Row1 = { cos(glm::radians(yaw)),sin(glm::radians(yaw))*sin(glm::radians(pitch)),sin(glm::radians(yaw))*cos(glm::radians(pitch)) };
	glm::vec3 Row2 = { 0,cos(glm::radians(pitch)),-sin(glm::radians(pitch)) };
	glm::vec3 Row3 = { -sin(glm::radians(yaw)),cos(glm::radians(yaw))*sin(glm::radians(pitch)),cos(glm::radians(yaw))*cos(glm::radians(pitch)) };
	glm::mat4 view1(
		glm::vec4(Row1, glm::dot(Row1, -position)),
		glm::vec4(Row2, glm::dot(Row2, -position)),
		glm::vec4(Row3, glm::dot(Row3, -position)),
		glm::vec4(0, 0, 0, 1)
	);//������ǵ����ת��������glm��������ô����ת��
	this->view = view1;
	return glm::transpose(view1);
}

float Camera::getFov() {
	return fov;
}

void Camera::mouseMovement(double xoffset, double yoffset) {
	//��������ߣ�������������ҿ�����������ߣ��������������/
	//��������ߣ������̧ͷ����������ߣ��������ͷ
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	pitch -= yoffset;
	yaw += xoffset;
	//���ܿ���Ҳ���ܿ��أ�����������������
	if (pitch >= 89.0)
		pitch = 89.0;
	if (pitch <= -89.0)
		pitch = -89.0;
}

void Camera::scrollChange(double yoffset) {
	if (fov >= 45.0)
		fov = 45.0;
	if (fov <= 1.0)
		fov = 1.0;
	if (fov >= 1.0 && fov <= 45.0)
		fov += yoffset;
}

void Camera::keyMovement() {
	currenttime = glfwGetTime();
	deltatime = currenttime - lasttime;
	lasttime = currenttime;//ÿ֡��ʱ��

						   //w --> �����z�������� ��ģ����z��������   A --> �����x�������� ��ģ����x��������
	glm::vec3 Front = glm::normalize(glm::transpose(view) *glm::vec4(0.0, 0.0, -1.0, 0.0));//������۲췽��
	glm::vec3 Right = glm::normalize(glm::transpose(view) *glm::vec4(1.0, 0.0, 0.0, 0.0));
	GLfloat vary = movementSpeed * deltatime;

	if (key_status[GLFW_KEY_W]) //GLFW_KEY_W
		position += Front * vary;
	if (key_status[GLFW_KEY_S])//GLFW_KEY_S
		position -= Front * vary;
	if (key_status[GLFW_KEY_A])//GLFW_KEY_A
		position -= Right * vary;
	if (key_status[GLFW_KEY_D])//GLFW_KEY_D
		position += Right * vary;
}