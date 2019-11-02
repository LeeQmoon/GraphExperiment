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
bool first = true;//第一次改变鼠标的位置
bool flag = false;//只有当鼠标左键按下的时候让模型自行旋转角度
static float theta = 0.0;//改变的角度
glm::mat4 model;//模型变换--->世界坐标系
double xoff = 0.0;//左键按下用的
string fstr;
string vstr;

const char temp = 'a';
const char *vertex_shader = &temp;//不能初始化为空指针，会出现访问异常
const char *fragment_shader = &temp;

void frameBufferSizeCallBack(GLFWwindow *window, int width, int height);//帧缓冲回调
void readShaderSource();//读取shader文档
void checkShader(string type, int &object, int *success, char *info);//生成shader
void checkShaderProgram(int &shaderprogram, int &vertexshader, int &fragmentshader, int *success, char *info);
void cursorCallBack(GLFWwindow *window, double xpos, double ypos);//鼠标光标变化
void scrollCallBack(GLFWwindow *window, double x, double yoffset);//滚轮变化
void mouseButtonCallBack(GLFWwindow *window, int button, int action, int mod);//鼠标左键按着
void key_callback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods);//键盘回调
void handleModel();//处理模型矩阵
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
	glfwSetCursorPosCallback(window, cursorCallBack);//当鼠标移动的时候，回调
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

	initLight();//初始化光源参数

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

	//渲染循环
	while (!glfwWindowShouldClose(window)) {
		camera.keyMovement();
		glClearColor(0.2, 0.3, 0.3, 1.0);//
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //
		glUseProgram(shaderprogram);

		glm::mat4 view = camera.getView();//观察矩阵
		GLuint viewId = glGetUniformLocation(shaderprogram, "view");
		//投影矩阵
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)width / (float)height, 0.1f, 100.0f);
		GLuint projectionId = glGetUniformLocation(shaderprogram, "projection");
		glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));
		processLight(shaderprogram);//将光源信息传至shader
		modell.display();  //   这个好像特别慢   -------------------------log----------------------
		glfwSwapBuffers(window);
		glfwPollEvents();//轮询事件队列
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
		vss << vfile.rdbuf();//把文件流缓冲区的内容输入到字符流中
		vstr = vss.str();
		vertex_shader = vstr.c_str();//获得顶点着色器代码
		for (int i = 0; i < vstr.size(); i++)
			cout << vertex_shader[i];
		vfile.close();
	}
	if (ffile.is_open()) {
		fss << ffile.rdbuf();//把文件流的缓冲区内容弄到字符流
		fstr = fss.str();//fstr把字符流中的字符串全部读出
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
			flag = true;//开关打开
	}
	else if (action == GLFW_RELEASE) {
		flag = false;//开关打上
	}
}


//键盘回调
void key_callback(GLFWwindow *window, GLint key, GLint scancode, GLint action, GLint mods)
{

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		//设置按下/释放键为true或false
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