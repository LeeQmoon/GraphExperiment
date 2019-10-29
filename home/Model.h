#pragma once
#include"Object.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<iostream>
#include<vector>
using namespace std;

struct material {
	glm::vec3 emission;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

class Model
{
private:
	vector<Object>model;//һ��ģ���ɶ����һ���Ķ���
public:
	Model() {}
	void getObject(Object object);
	~Model(){}
};

