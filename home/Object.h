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
	string material_name;//材质名
	float Ns;//高光度
	float d;//透明度
	int illum;//所用光照模型
	glm::vec3 Ka;//环境光反射系数
	glm::vec3 Kd;//漫反射系数
	glm::vec3 Ks;//镜面反射系数
				 //环境反射指定贴图
	string map_Ka;
	//漫反射指定贴图
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
	Material material;//每个对象只有一种材质
	GLuint VBO;
	GLuint VAO;
	GLuint texture;

	Object();
	Object(const Object &object);
	void setBufferAndVertexArray();//flag传参代表是否有索引
	void seTexture();//path是纹理所在的路径
	void draw();
	void deleteVBOAndVAOAndTexture();
	void print();
	~Object();
};