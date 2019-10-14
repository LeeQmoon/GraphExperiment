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
#include<iomanip>
using namespace std;

struct point {
	float x;
	float y;
	float z;
};

struct face {
	int Ione;//索引一
	int Itwo;//索引二
	int Ithree;//索引三
};

using Point = struct point;
using Face = struct face;
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

const char temp = 'a';
const char *vertex_shader = &temp;//不能初始化为空指针，会出现访问异常
const char *fragment_shader = &temp;
string fstr;
string vstr;

void frameBufferSizeCallBack(GLFWwindow *window, int width, int height);//帧缓冲回调
void readData(vector<Point> &vertices, vector<Face> &faces);//读取obj文件
void readShaderSource();//读取shader文档
void checkShader(string type, int &object, int *success, char *info);//生成shader
void checkShaderProgram(int &shaderprogram, int &vertexshader, int &fragmentshader, int *success, char *info);
void cursorCallBack(GLFWwindow *window, double xpos, double ypos);//鼠标光标变化
void scrollCallBack(GLFWwindow *window, double x, double yoffset);//滚轮变化
void mouseButtonCallBack(GLFWwindow *window,int button,int action,int mod);//鼠标左键按着
void handleModel();//处理模型矩阵

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
	glViewport(100, 100, width, height);
	vector<Point>vertices;
	vector<Face>indices;
	readData(vertices, indices);//以传引用的方式传入，获取数据

	GLuint VBO, EBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point)*vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face)*indices.size(), &indices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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

	model = glm::translate(model, glm::vec3(-0.1, -0.5, 0.1));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	GLuint modelId = glGetUniformLocation(shaderprogram, "model");

	//渲染循环
	while (!glfwWindowShouldClose(window)) {
		glm::mat4 view = camera.getView();//观察矩阵
		GLuint viewId = glGetUniformLocation(shaderprogram, "view");
		//投影矩阵
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)width / (float)height, 0.1f, 100.0f);
		GLuint projectionId = glGetUniformLocation(shaderprogram, "projection");

		glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(VAO);
		glUseProgram(shaderprogram);
		glClearColor(0.2, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 3 * indices.size(), GL_UNSIGNED_INT, (void*)0);
		glfwSwapBuffers(window);
		glfwPollEvents();//轮询事件队列
	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}


void frameBufferSizeCallBack(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void readData(vector<Point> &vertices, vector<Face> &faces) {

	ifstream flie("C:/Users/Fullmoon/Desktop/house.obj", std::ifstream::in);//只读打开
	string str;
	stringstream ss;

	//先判断文件是否已经打开了
	if (flie.is_open()) {
		while (getline(flie, str)) {//逐行读取
			Point point;
			Face face;

			//图元索引
			if (str[0] == 'f') {
				ss.clear();//写入或者读出不一样的类型时，字符流要清空一次
				ss << str;//把字符串写入流中，方便读取了类型
				string temp;//主要是为了读取掉v/f
				ss >> temp >> face.Ione >> face.Itwo >> face.Ithree;
				face.Ione--;//文件索引比实际索引大1
				face.Itwo--;
				face.Ithree--;
				faces.emplace_back(face);
			}

			//顶点数据
			if (str[0] == 'v') {
				ss.clear();
				ss << str;
				string temp;
				ss >> temp >> point.x >> point.y >> point.z;
				vertices.emplace_back(point);
			}

		}
	}

	flie.close();//关闭文件
}

void readShaderSource() {
	fstream vfile("vertex.txt");
	fstream ffile("fragment.txt");
	stringstream vss;
	stringstream fss;

	if (vfile.is_open()) {
		vss << vfile.rdbuf();//把文件流缓冲区的内容输入到字符流中
		vstr = vss.str();
		vertex_shader = vstr.c_str();//获得顶点着色器代码
		vfile.close();
	}
	if (ffile.is_open()) {
		fss << ffile.rdbuf();
		fstr = fss.str();
		fragment_shader = fstr.c_str();
		ffile.close();
	}

}

void checkShader(string type, int &object, int *success, char *info) {
	if (type == "vertex") {
		object = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(object, 1, &vertex_shader, NULL);
		glCompileShader(object);
		glGetShaderiv(object, GL_COMPILE_STATUS, success);
		if (!success) {
			glGetShaderInfoLog(object, 520, NULL, info);
			cout << "VERTEXSHADER ERROR:" << info << endl;
		}
	}
	if (type == "fragment") {
		object = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(object, 1, &fragment_shader, NULL);
		glCompileShader(object);
		glGetShaderiv(object, GL_COMPILE_STATUS, success);
		if (!success) {
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
	yoffset = last_ypos - ypos;//窗口坐标系y轴自上而下递增导致的
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

