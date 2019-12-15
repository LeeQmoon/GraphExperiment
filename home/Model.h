#pragma once
#include"Object.h"
#include<iostream>
#include<vector>
#include"FollowObject.h"
using namespace std;

class Model
{
public:
	vector<Object>objects;//һ��ģ���ɶ����һ���Ķ���
	int indexOfPlayer;//�ĸ������
	string ObjPath;//OBJ�ļ�·��
	string matklu;//��һ�����ʿ�->�˰���һ�����ʿ�·��
	int size;//�м����������
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

