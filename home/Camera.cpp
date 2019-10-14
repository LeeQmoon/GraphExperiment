#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Camera.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
using namespace std;

void Camera::updateDirection() {
	direction.x = cos(glm::radians(this->pitch))*cos(glm::radians(this->yaw));
	direction.y = sin(glm::radians(pitch));//角度转弧度
	direction.z = cos(glm::radians(this->pitch))*sin(glm::radians(this->yaw));
	direction = glm::normalize(direction);//归一化
}

//观察矩阵
glm::mat4 Camera::getView() {
	//摄像机始终看向一个方向
	return glm::lookAt(position, position + direction,world_up);
}

float Camera::getFov() {
	return fov;
}

void Camera::mouseMovement(double xoffset, double yoffset) {
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	
	pitch += yoffset;
	yaw += xoffset;
	//不能看天也不能看地
	if (pitch >= 89.0)
		pitch = 89.0;
	if (pitch <= -89.0)
		pitch = -89.0;
	updateDirection();
}

void Camera::scrollChange(double yoffset) {
	if (fov >= 45.0)
		fov = 45.0;
	if (fov <= 1.0)
		fov = 1.0;
	if (fov >= 1.0 && fov <= 45.0)
		fov += yoffset;
}