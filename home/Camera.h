#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
using namespace std;

class Camera {
private:
	glm::vec3 position;//λ��
	float fov;//�ӽ�
	float yaw;//ƫ����
	float pitch;//������
	float sensitivity;//λ�����ж�
public:
	Camera(glm::vec3 Position = glm::vec3(0.0,0.0,5.0)) {
		position = Position;
		fov = 45.0;
		yaw = 0;//֮����Ϊ0������Ϊλ�õ����ã�ʹ�����߳�ʼʱ���ص�
		pitch = 0;//֮����Ϊ0������Ϊ���������������Ѿ�����������ϵ�ص���
		sensitivity = 0.05;

	}

	glm::mat4 getView();
	float getFov();
	void mouseMovement(double xoffset,double yoffset);//�����ı�
	void scrollChange(double yoffset);//����ת��
};