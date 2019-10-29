#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Camera.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
using namespace std;

//ŷ����ʵ�֣��۲����
glm::mat4 Camera::getView() {
	glm::vec3 Row1 = { cos(glm::radians(yaw)),sin(glm::radians(yaw))*sin(glm::radians(pitch)),-sin(glm::radians(yaw))*cos(glm::radians(pitch)) };
	glm::vec3 Row2 = { 0,cos(glm::radians(pitch)),sin(glm::radians(pitch)) };
	glm::vec3 Row3 = { sin(glm::radians(yaw)),-cos(glm::radians(yaw))*sin(glm::radians(pitch)),cos(glm::radians(yaw))*cos(glm::radians(pitch)) };
	glm::mat4 view(
		glm::vec4(Row1,glm::dot(Row1,-position)),
		glm::vec4(Row2,glm::dot(Row2,-position)),
		glm::vec4(Row3,glm::dot(Row3,-position)),
		glm::vec4(0,0,0,1)
	);//������ǵ����ת��������glm��������ô����ת��
	view = glm::transpose(view);
	return view;
}

float Camera::getFov() {
	return fov;
}

void Camera::mouseMovement(double xoffset, double yoffset) {
	//��������ߣ�������������ҿ�����������ߣ��������������/
	//��������ߣ������̧ͷ����������ߣ��������ͷ
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	pitch += yoffset;
	yaw -= xoffset;
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