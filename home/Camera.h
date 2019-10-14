#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
using namespace std;

class Camera {
private:
	glm::vec3 position;//位置
	glm::vec3 target;//目标点
	glm::vec3 direction;//z轴的反向
	glm::vec3 world_up;//世界坐标系下的y轴
	float fov;//视角
	float yaw;//偏航角
	float pitch;//俯仰角
	float sensitivity;//位移敏感度

public:
	Camera(glm::vec3 Position = glm::vec3(0.0,0.0,3.0), glm::vec3 Target = glm::vec3(0.0, 0.0, 0.0), glm::vec3 WorldUp = glm::vec3(0.0, 1.0, 0.0)) {
		position = Position;
		target = Target;
		direction = position - target;//观察方向
		world_up = WorldUp;
		fov = 45.0;
		yaw = -90.0;
		pitch = 0.0;
		sensitivity = 0.05;

		updateDirection();
	}

	void updateDirection();
	glm::mat4 getView();
	float getFov();
	void mouseMovement(double xoffset,double yoffset);//鼠标光标改变
	void scrollChange(double yoffset);//滚轮转动
};