#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
using namespace std;

class Camera {
private:
	float fov;//�ӽ�
	float yaw;//ƫ����
	float pitch;//������
	float sensitivity;//λ�����ж�
	float movementSpeed;//
	glm::mat4 view;//δת��ǰ�Ĺ۲���󣬼�������������ת�ó��ľ���   -----�������Ҫ��Ϊ�˼��̿���ʱ��ȡ�۲췽��----

public:
	glm::vec3 Zaxi;
	glm::vec3 position;//λ��
	GLboolean key_status[1024];//���̱�־���� ---���ĸ�����������-----
	Camera(glm::vec3 Position = glm::vec3(0.0,0.0,8.0)) {
		position = Position;
		fov = 45.0;
		yaw = 0;//֮����Ϊ0������Ϊλ�õ����ã�ʹ�����߳�ʼʱ���ص�
		pitch = 0;//֮����Ϊ0������Ϊ���������������Ѿ�����������ϵ�ص���
		sensitivity = 0.05;
		movementSpeed = 1.5f;

	}

	glm::mat4 getView();
	float getFov();
	void mouseMovement(double xoffset,double yoffset);//�����ı�
	void scrollChange(double yoffset);//����ת��
	void keyMovement();
};