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
	point() {}
	point(float X, float Y, float Z) {
		x = X;
		y = Y;
		z = Z;
	}
};
struct texture {
	float s;
	float t;
	texture() {}
	texture(float S, float T) {
		s = S;
		t = T;
	}
};
struct material {
	string material_name;//���õĲ�����
	glm::vec3 emission;//�������
	float Ns;//�߹��
	float d;//͸����
	int illum;//���ù���ģ��
	glm::vec3 Ka;//�����ⷴ��ϵ��
	glm::vec3 Kd;//������ϵ��
	glm::vec3 Ks;//���淴��ϵ��
	string map_Ka;
	string map_Kd;
	string map_Bump;
	string map_Ks;
};

using Point = struct point;
using Texture = struct texture;
using Material = struct material;

class Object {
public:
	vector<Point>vertices;
	vector<Texture>texture_coords;
	vector<Point>normal;
	Material material;//ÿ������ֻ��һ�ֲ���
	GLuint VBO;
	GLuint VAO;
	GLuint texture[2];//map_Kd or map_Ks
	int texturecount[2];//����shader

	Object();
	Object(string material_name);
	void setBufferAndVertexArray();//flag���δ����Ƿ�������
	void seTexture();//path���������ڵ�·��
	void draw(unsigned int shaderprogram);
	void deleteVBOAndVAOAndTexture();
	void print();
	~Object();
};