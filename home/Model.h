#pragma once
#include"Object.h"
#include<iostream>
#include<vector>
#include"FollowObject.h"
using namespace std;

class Model
{
public:
	vector<Object>objects;//一个模型由多个不一样的对象
	int indexOfPlayer;//哪个是玩家
	string ObjPath;//OBJ文件路径
	string matklu;//有一个材质库->此包含一个材质库路径
	int size;//有几个对象组成
	glm::mat4 radinaryModel;
	glm::mat4 playerModel;
	//glm::vec4 target;
public:
	Model() {
		size = 0;
	}
	Model(const string &path);
	void readObj();
	void readMtl();
	void spreadModel(int i, unsigned int shaderprogram, glm::mat4 &view);
	void processMaterial(Material *temp, int count);
	void display(unsigned int shaderprogram,glm::mat4 &view);
	~Model();
};

