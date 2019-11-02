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
using namespace std;

struct Light {
	glm::vec3 Ambient;
	glm::vec3 LightColor;
	glm::vec3 LightPosition;
	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;
};
struct Light light;


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
string fstr;
string vstr;

const char temp = 'a';
const char *vertex_shader = &temp;//���ܳ�ʼ��Ϊ��ָ�룬����ַ����쳣
const char *fragment_shader = &temp;

void frameBufferSizeCallBack(GLFWwindow *window, int width, int height);//֡����ص�
void readShaderSource();//��ȡshader�ĵ�
void checkShader(string type, int &object, int *success, char *info);//����shader
void checkShaderProgram(int &shaderprogram, int &vertexshader, int &fragmentshader, int *success, char *info);
void cursorCallBack(GLFWwindow *window, double xpos, double ypos);//�����仯
void scrollCallBack(GLFWwindow *window, double x, double yoffset);//���ֱ仯
void mouseButtonCallBack(GLFWwindow *window, int button, int action, int mod);//����������
void key_callback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods);//���̻ص�
void handleModel();//����ģ�;���
void initLight();
void processLight(GLuint shaderprogram);


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

	Model modell("homework.obj");
	modell.readObj();
	/*for (int i = 0; i < modell.size; i++)
	modell.objects[i].print();
	*/

	initLight();//��ʼ����Դ����

	int vertexshader, fragmentshader;
	int shaderprogram;
	int success;
	char info[520];
	readShaderSource();
	checkShader("vertex", vertexshader, &success, info);
	checkShader("fragment", fragmentshader, &success, info);
	checkShaderProgram(shaderprogram, vertexshader, fragmentshader, &success, info);
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	model = glm::translate(model, glm::vec3(0.3, -1.0, 0.0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	GLuint modelId = glGetUniformLocation(shaderprogram, "model");

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		camera.keyMovement();
		glClearColor(0.2, 0.3, 0.3, 1.0);//
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //
		glUseProgram(shaderprogram);

		glm::mat4 view = camera.getView();//�۲����
		GLuint viewId = glGetUniformLocation(shaderprogram, "view");
		//ͶӰ����
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)width / (float)height, 0.1f, 100.0f);
		GLuint projectionId = glGetUniformLocation(shaderprogram, "projection");
		glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));
		processLight(shaderprogram);//����Դ��Ϣ����shader
		modell.display();  //   ��������ر���   -------------------------log----------------------
		glfwSwapBuffers(window);
		glfwPollEvents();//��ѯ�¼�����
	}
	glfwTerminate();
	return 0;
}


void frameBufferSizeCallBack(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void readShaderSource() {
	fstream vfile("vertex.vs");
	fstream ffile("fragment.fs");
	stringstream vss;
	stringstream fss;

	if (vfile.is_open()) {
		vss << vfile.rdbuf();//���ļ������������������뵽�ַ�����
		vstr = vss.str();
		vertex_shader = vstr.c_str();//��ö�����ɫ������
		for (int i = 0; i < vstr.size(); i++)
			cout << vertex_shader[i];
		vfile.close();
	}
	if (ffile.is_open()) {
		fss << ffile.rdbuf();//���ļ����Ļ���������Ū���ַ���
		fstr = fss.str();//fstr���ַ����е��ַ���ȫ������
		fragment_shader = fstr.c_str();
		for (int i = 0; i < fstr.size(); i++)
			cout << fragment_shader[i];
		ffile.close();
	}

}

void checkShader(string type, int &object, int *success, char *info) {
	if (type == "vertex") {
		object = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(object, 1, &vertex_shader, NULL);
		glCompileShader(object);
		
		glGetShaderiv(object, GL_COMPILE_STATUS, success);
		if (!*success) {
			glGetShaderInfoLog(object, 520, NULL, info);
			cout << "VERTEXSHADER ERROR:" << info << endl;
		}
	}
	if (type == "fragment") {
		object = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(object, 1, &fragment_shader, NULL);
		glCompileShader(object);
		glGetShaderiv(object, GL_COMPILE_STATUS, success);
		if (!*success) {
			glGetShaderInfoLog(object, 520, NULL, info);
			cout << "FRAGMENTSAHDER ERROR:" << info << endl;
		}
	}
}

void checkShaderProgram(int &shaderprogram, int &vertexshader, int &fragmentshader, int *success, char *info) {
	shaderprogram = glCreateProgram();
	glAttachShader(shaderprogram, vertexshader);
	glAttachShader(shaderprogram, fragmentshader);
	glLinkProgram(shaderprogram);
	glGetProgramiv(shaderprogram, GL_LINK_STATUS, success);
	if (!success) {
		glGetProgramInfoLog(shaderprogram, 520, NULL, info);
		cout << "SHADERPROGRAM ERROR:" << info << endl;
	}
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

void initLight() {
	light.Ambient = glm::vec3(0.0, 0.0, 0.0);
	light.LightColor = glm::vec3(0.5, 0.5, 0.5);
	light.LightPosition = glm::vec3(1.0, 1.0, 1.0);
	light.ConstantAttenuation = 0.2;
	light.LinearAttenuation = 0.3;
	light.QuadraticAttenuation = 0.4;
}
void processLight(GLuint shaderprogram) {
	GLuint ambientID = glGetUniformLocation(shaderprogram, "Ambient");
	GLuint lightColorID = glGetUniformLocation(shaderprogram, "LightColor");
	GLuint lightPosition = glGetUniformLocation(shaderprogram, "LightPosition");
	GLuint constantAttenuationID = glGetUniformLocation(shaderprogram, "ConstantAttenuation");
	GLuint linearAttenuationID = glGetUniformLocation(shaderprogram, "LinearAttenuation");
	GLuint quadraticAttenuationID = glGetUniformLocation(shaderprogram, "QuadraticAttenuation");
	glUniform3fv(ambientID, 1, &light.Ambient[0]);
	glUniform3fv(lightColorID, 1, &light.LightColor[0]);
	glUniform3fv(lightPosition, 1, &light.LightPosition[0]);
	glUniform1f(constantAttenuationID, light.ConstantAttenuation);
	glUniform1f(linearAttenuationID, light.LinearAttenuation);
	glUniform1f(quadraticAttenuationID, light.QuadraticAttenuation);
}