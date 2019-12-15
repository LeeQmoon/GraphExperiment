#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>

class FollowObject
{
public:

	float fov;//�ӽ�
	float yaw;//ƫ����
	float pitch;//������
	float sensitivity;//λ�����ж�
	float movementSpeed;//
	glm::mat4 view;//δת��ǰ�Ĺ۲���󣬼�������������ת�ó��ľ���   -----�������Ҫ��Ϊ�˼��̿���ʱ��ȡ�۲췽��----
	glm::vec3 target;
public:
	glm::vec3 Zaxi;
	glm::vec3 position;//λ��
	GLboolean key_status[1024];//���̱�־���� ---���ĸ�����������-----
	FollowObject(glm::vec3 Position = glm::vec3(-4.00417,0.0,-0.03423),glm::vec3 Target= glm::vec3(-3.20417,0.5,-0.23423)) {
		position = Position;
		target = Target;
		fov = 45.0;
		yaw = 0;//֮����Ϊ0������Ϊλ�õ����ã�ʹ�����߳�ʼʱ���ص�
		pitch = 0;//֮����Ϊ0������Ϊ���������������Ѿ�����������ϵ�ص���
		sensitivity = 1.5;
		movementSpeed = 0.5f;

	}
	void caculateInitial();
	void trans(glm::mat4 &playerModel);
	glm::mat4 getView();
	float getFov();
	void mouseMovement(double xoffset, double yoffset);//�����ı�
	void scrollChange(double yoffset);//����ת��
	void keyMovement(glm::mat4 &playerModel);
	FollowObject();
	~FollowObject();
};

