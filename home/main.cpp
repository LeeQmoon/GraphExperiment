/*
author: Lee by 2019/12/14
idea:
	1.�а����ҹ֮��
	2.����ֻ��̫���⼰����Ӧ�ػ�����
	3.������������Ⲣ��������Ӧ�ػ�����
	4.�����˳������
*/
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include"Camera.h"
#include"Model.h"
#include<iomanip>
#include"Shader.h"
#include"Light.h"
using namespace std;


Light light;

const int width = 800;
const int height = 600;
Camera camera;
double last_xpos = 0.0;
double last_ypos = 0.0;
bool first = true;//��һ�θı�����λ��
bool flag = false;//ֻ�е����������µ�ʱ����ģ��������ת�Ƕ�
static float theta = 0.0;//�ı�ĽǶ�
glm::mat4 model;//ģ�ͱ任--->��������ϵ
double xoff = 0.0;//��������õ�

void frameBufferSizeCallBack(GLFWwindow *window, int width, int height);//֡����ص�
void cursorCallBack(GLFWwindow *window, double xpos, double ypos);//�����仯
void scrollCallBack(GLFWwindow *window, double x, double yoffset);//���ֱ仯
void mouseButtonCallBack(GLFWwindow *window, int button, int action, int mod);//����������
void key_callback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods);//���̻ص�
void handleModel();//����ģ�;���


int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(width, height, "Experiment", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
	glfwSetCursorPosCallback(window, cursorCallBack);//������ƶ���ʱ�򣬻ص�
	glfwSetScrollCallback(window, scrollCallBack);
	glfwSetMouseButtonCallback(window, mouseButtonCallBack);
	glfwSetKeyCallback(window, key_callback);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	
	Model modell("FinalHome.obj");
	modell.readObj();
	/*for (int i = 0; i < modell.size; i++)
	modell.objects[i].print();*/


	light.initLight();//��ʼ����Դ����

	Shader shader("vertex.vs", "fragment.fs");

	model = glm::translate(model, glm::vec3(0.0, -1.5, 0.0));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
	//model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	GLuint modelId = shader.getLocation("model");

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		camera.keyMovement();
		glClearColor(0.2, 0.3, 0.3, 1.0);//
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //
		shader.activeProgram();

		//glUniform3fv(shader.getLocation("Eye"), 1, &camera.position[0]);
		glm::mat4 view = camera.getView();//�۲����
		GLuint viewId = shader.getLocation("view");
		//ͶӰ����
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)width / (float)height, 0.1f, 100.0f);
		GLuint projectionId = shader.getLocation("projection");
		glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));
		light.processLight(shader.shaderprogram);//����Դ��Ϣ����shader
		modell.display(shader.shaderprogram);  //   ��������ر���   -------------------------log----------------------
		glfwSwapBuffers(window);
		glfwPollEvents();//��ѯ�¼�����
	}
	glfwTerminate();
	return 0;
}


void frameBufferSizeCallBack(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void handleModel() {
	if (flag) {
		theta = xoff;
		model = glm::rotate(model, glm::radians(theta), glm::vec3(0.0, 1.0, 0.0));
	}
}

void cursorCallBack(GLFWwindow *window, double xpos, double ypos) {
	double xoffset, yoffset;
	if (first) {
		last_xpos = xpos;
		last_ypos = ypos;
		first = false;
	}
	xoffset = xpos - last_xpos;
	yoffset = last_ypos - ypos;
	last_xpos = xpos;
	last_ypos = ypos;
	xoff = xoffset;
	handleModel();
	camera.mouseMovement(xoffset, yoffset);
}

void scrollCallBack(GLFWwindow *window, double x, double yoffset) {
	camera.scrollChange(yoffset);
}

void mouseButtonCallBack(GLFWwindow *window, int button, int action, int mod) {
	if (action == GLFW_PRESS) {
		if (button == GLFW_MOUSE_BUTTON_LEFT)
			flag = true;//���ش�
	}
	else if (action == GLFW_RELEASE) {
		flag = false;//���ش���
	}
}


//���̻ص�
void key_callback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods)
{

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		//���ð���/�ͷż�Ϊtrue��false
		if (action == GLFW_PRESS)
			camera.key_status[key] = true;
		else if (action == GLFW_RELEASE)
			camera.key_status[key] = false;
	}

}
