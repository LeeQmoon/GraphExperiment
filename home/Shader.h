#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;

class Shader
{
public:
	int vertexshader;
	int fragmentshader;
	int shaderprogram;
	Shader(){}
	Shader(const char *vShaderPath, const char *fShaderPath);
	void loadShaderSource(const char *vShaderPath, const char *fShaderPath);
	void createShader(const char *shadersource,string type);
	void createShaderProgram();
	GLuint getLocation(const char *name);
	void activeProgram();
	~Shader();
};
