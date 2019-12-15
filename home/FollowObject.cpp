#include "FollowObject.h"
#include<algorithm>
#include<iostream>
using namespace std;


//不可把这部分放在camera.h中，出现重定义
GLdouble currenttime = 0.0f;
GLdouble lasttime = 0.0f;
GLdouble deltatime = 0.0f;

FollowObject::FollowObject()
{
	pitch = 0.0;
	yaw = 0.0;
}

void FollowObject::caculateInitial() {
	float squareLength = this->position.x*this->position.x + this->position.y*this->position.y;
	float Length = squareLength + this->position.z*this->position.z;
	float sqrtLength = sqrt(Length);
	float XandYLength = sqrt(squareLength);
	pitch = asin(position.y / sqrtLength);//弧度制
	yaw = acos(position.z / XandYLength);
	cout << "pitch: " << pitch << "yaw:" << yaw << endl;
}

//欧拉角实现，观察矩阵
glm::mat4 FollowObject::getView() {
	////glm::vec3 Row1 = { cos(glm::radians(yaw)),sin(glm::radians(yaw))*sin(glm::radians(pitch)),-sin(glm::radians(yaw))*cos(glm::radians(pitch)) };
	////glm::vec3 Row2 = { 0,cos(glm::radians(pitch)),sin(glm::radians(pitch)) };
	////glm::vec3 Row3 = { sin(glm::radians(yaw)),-cos(glm::radians(yaw))*sin(glm::radians(pitch)),cos(glm::radians(yaw))*cos(glm::radians(pitch)) };
	//////glm::mat4 view(
	//////	glm::vec4(Row1,glm::dot(Row1,-position)),
	//////	glm::vec4(Row2,glm::dot(Row2,-position)),
	//////	glm::vec4(Row3,glm::dot(Row3,-position)),
	//////	glm::vec4(0,0,0,1)
	//////);//这个就是点的旋转矩阵，由于glm行主序，那么得先转置
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
	);//这个就是点的旋转矩阵，由于glm行主序，那么得先转置
	glm::vec3 Z = glm::mat3(view1)*glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 X = glm::mat3(view1)*glm::vec3(1.0, 0.0, 0.0);
	glm::vec3 Y = glm::mat3(view1)*glm::vec3(0.0, 1.0, 0.0);
	glm::mat4 view2(
		glm::vec4(X, glm::dot(Row1, -position)),
		glm::vec4(Y, glm::dot(Row2, -position)),
		glm::vec4(Z, glm::dot(Row3, -position)),
		glm::vec4(0, 0, 0, 1)
	);

	this->view = view2;
	//return glm::transpose(view1);
	return glm::lookAt(position, target, glm::vec3(0.0, 1.0, 0.0));
}

float FollowObject::getFov() {
	return fov;
}

void FollowObject::mouseMovement(double xoffset, double yoffset) {
	//鼠标往右走，就是摄像机往右看，鼠标往左走，就是摄像机往左看/
	//鼠标往上走，摄像机抬头，鼠标往下走，摄像机低头
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	pitch -= yoffset;
	yaw += xoffset;
	//不能看天也不能看地，否则出现万向节死锁
	if (pitch >= 89.0)
		pitch = 89.0;
	if (pitch <= -89.0)
		pitch = -89.0;
}

void FollowObject::scrollChange(double yoffset) {
	if (fov >= 45.0)
		fov = 45.0;
	if (fov <= 1.0)
		fov = 1.0;
	if (fov >= 1.0 && fov <= 45.0)
		fov += yoffset;
}

void FollowObject::keyMovement(glm::mat4 &playerModel) {
	currenttime = glfwGetTime();
	deltatime = currenttime - lasttime;
	lasttime = currenttime;//每帧的时间

						   //w --> 相机往z负方向走 即模型往z正方向走   A --> 相机往x负方向走 即模型往x正方向走
	glm::vec3 Front = glm::normalize(view *glm::vec4(0.0, 0.0, -1.0, 0.0));//摄像机观察方向
	glm::vec3 Right = glm::normalize(view *glm::vec4(1.0, 0.0, 0.0, 0.0));
	GLfloat vary = movementSpeed * deltatime;

	if (key_status[GLFW_KEY_W]) //GLFW_KEY_W
	{
		position += Front * vary;
		playerModel = glm::translate(playerModel, glm::vec3(Front * vary));
		target += glm::vec3(Front * vary);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				cout << playerModel[i][j] << "  ";
			cout << endl;
		}
	}
	if (key_status[GLFW_KEY_S])//GLFW_KEY_S
	{
		position -= Front * vary;
		playerModel = glm::translate(playerModel, -glm::vec3(Front * vary));
		target -= glm::vec3(Front * vary);
	}
	if (key_status[GLFW_KEY_A])//GLFW_KEY_A
	{
		position -= Right * vary;
		playerModel = glm::translate(playerModel, -glm::vec3(Right * vary));
		target -= glm::vec3(Right * vary);
	}
	if (key_status[GLFW_KEY_D])//GLFW_KEY_D
	{
		position += Right * vary;
		playerModel = glm::translate(playerModel, glm::vec3(Right * vary));
		target += glm::vec3(Right * vary);
	}
}

void FollowObject::trans(glm::mat4 &playerModel) {

}

FollowObject::~FollowObject()
{
}
