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
	string material_name;//所用的材质名
	glm::vec3 emission;//自身发光度
	float Ns;//高光度
	float d;//透明度
	int illum;//所用光照模型
	glm::vec3 Ka;//环境光反射系数
	glm::vec3 Kd;//漫反射系数
	glm::vec3 Ks;//镜面反射系数
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
	Material material;//每个对象只有一种材质
	GLuint VBO;
	GLuint VAO;
	GLuint texture[2];//map_Kd or map_Ks
	int texturecount[2];//传进shader

	Object();
	Object(string material_name);
	void setBufferAndVertexArray();//flag传参代表是否有索引
	void seTexture();//path是纹理所在的路径
	void draw(unsigned int shaderprogram);
	void deleteVBOAndVAOAndTexture();
	void print();
	~Object();
};