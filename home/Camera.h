#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
using namespace std;

class Camera {
private:
	glm::vec3 position;//λ��
	glm::vec3 target;//Ŀ���
	glm::vec3 direction;//z��ķ���
	glm::vec3 world_up;//��������ϵ�µ�y��
	float fov;//�ӽ�
	float yaw;//ƫ����
	float pitch;//������
	float sensitivity;//λ�����ж�

public:
	Camera(glm::vec3 Position = glm::vec3(0.0,0.0,3.0), glm::vec3 Target = glm::vec3(0.0, 0.0, 0.0), glm::vec3 WorldUp = glm::vec3(0.0, 1.0, 0.0)) {
		position = Position;
		target = Target;
		direction = position - target;//�۲췽��
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
	void mouseMovement(double xoffset,double yoffset);//�����ı�
	void scrollChange(double yoffset);//����ת��
};