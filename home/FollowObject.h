#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>

class FollowObject
{
public:

	float fov;//视角
	float yaw;//偏航角
	float pitch;//俯仰角
	float sensitivity;//位移敏感度
	float movementSpeed;//
	glm::mat4 view;//未转置前的观察矩阵，即是相机本身的旋转得出的矩阵   -----存这个主要是为了键盘控制时获取观察方向----
	glm::vec3 target;
public:
	glm::vec3 Zaxi;
	glm::vec3 position;//位置
	GLboolean key_status[1024];//键盘标志设置 ---看哪个键被按下了-----
	FollowObject(glm::vec3 Position = glm::vec3(-4.00417,0.0,-0.03423),glm::vec3 Target= glm::vec3(-3.20417,0.5,-0.23423)) {
		position = Position;
		target = Target;
		fov = 45.0;
		yaw = 0;//之所以为0，是因为位置的设置，使得两者初始时就重叠
		pitch = 0;//之所以为0，是因为摄像机坐标最初就已经跟惯性坐标系重叠了
		sensitivity = 1.5;
		movementSpeed = 0.5f;

	}
	void caculateInitial();
	void trans(glm::mat4 &playerModel);
	glm::mat4 getView();
	float getFov();
	void mouseMovement(double xoffset, double yoffset);//鼠标光标改变
	void scrollChange(double yoffset);//滚轮转动
	void keyMovement(glm::mat4 &playerModel);
	FollowObject();
	~FollowObject();
};

