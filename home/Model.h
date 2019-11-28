#pragma once
#include"Object.h"
#include<iostream>
#include<vector>
using namespace std;

class Model
{
public:
	Object * objects;//一个模型由多个不一样的对象
	string ObjPath;//OBJ文件路径
	string matklu;//有一个材质库->此包含一个材质库路径
	int size;//有几个对象组成
	int maxsize;
public:
	Model() {
		size = 0;
	}
	Model(const string &path);
	void readObj();
	void readMtl();
	void processMaterial(Material *temp, int count);
	void display(unsigned int shaderprogram);
	~Model();
};

