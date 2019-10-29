#pragma once
#include<glad/glad.h>
#include<iostream>
#include<vector>
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
using Point = struct point;
using Texture = struct texture;

class Object {
public:
	vector<Point> vertices;
	vector<Texture> texture_coords;
	GLuint VBO;
	GLuint VAO;
	GLuint texture;

	Object() {}
	void setBufferAndVertexArray();//flag���δ����Ƿ�������
	void seTexture(const char *path);//path���������ڵ�·��
	void draw();
	void deleteVBOAndVAOAndTexture();
	void print();
};