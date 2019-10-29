#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
using namespace std;

class Camera {
private:
	glm::vec3 position;//位置
	float fov;//视角
	float yaw;//偏航角
	float pitch;//俯仰角
	float sensitivity;//位移敏感度
public:
	Camera(glm::vec3 Position = glm::vec3(0.0,0.0,5.0)) {
		position = Position;
		fov = 45.0;
		yaw = 0;//之所以为0，是因为位置的设置，使得两者初始时就重叠
		pitch = 0;//之所以为0，是因为摄像机坐标最初就已经跟惯性坐标系重叠了
		sensitivity = 0.05;

	}

	glm::mat4 getView();
	float getFov();
	void mouseMovement(double xoffset,double yoffset);//鼠标光标改变
	void scrollChange(double yoffset);//滚轮转动
};