#pragma once
#include<glad/glad.h>
#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
using namespace std;

struct point {
	float x;
	float y;
	float z;
};
struct texture {
	float s;
	float t;
};
struct material {
	string material_name;//������
	float Ns;//�߹��
	float d;//͸����
	int illum;//���ù���ģ��
	glm::vec3 Ka;//�����ⷴ��ϵ��
	glm::vec3 Kd;//������ϵ��
	glm::vec3 Ks;//���淴��ϵ��
				 //��������ָ����ͼ
	string map_Ka;
	//������ָ����ͼ
	string map_Kd;
};
using Point = struct point;
using Texture = struct texture;
using Material = struct material;

class Object {
public:
	Point * vertices;
	Texture *texture_coords;
	Point *normal;
	int verSize;
	int texSize;
	int norSize;
	int maxsize;
	Material material;//ÿ������ֻ��һ�ֲ���
	GLuint VBO;
	GLuint VAO;
	GLuint texture;

	Object();
	Object(const Object &object);
	void setBufferAndVertexArray();//flag���δ����Ƿ�������
	void seTexture();//path���������ڵ�·��
	void draw();
	void deleteVBOAndVAOAndTexture();
	void print();
	~Object();
};