#pragma once
#include"Object.h"
#include<iostream>
#include<vector>
using namespace std;

class Model
{
public:
	Object * objects;//һ��ģ���ɶ����һ���Ķ���
	string ObjPath;//OBJ�ļ�·��
	string matklu;//��һ�����ʿ�->�˰���һ�����ʿ�·��
	int size;//�м����������
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

