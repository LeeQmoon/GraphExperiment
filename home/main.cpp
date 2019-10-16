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
#include"Object.h"
#include<iomanip>
using namespace std;

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
void readData(vector<Object> &objects,const char *objPath);//��ȡobj�ļ�
void readShaderSource();//��ȡshader�ĵ�
void checkShader(string type, int &object, int *success, char *info);//����shader
void checkShaderProgram(int &shaderprogram, int &vertexshader, int &fragmentshader, int *success, char *info);
void cursorCallBack(GLFWwindow *window, double xpos, double ypos);//�����仯
void scrollCallBack(GLFWwindow *window, double x, double yoffset);//���ֱ仯
void mouseButtonCallBack(GLFWwindow *window, int button, int action, int mod);//����������
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
	glViewport(0, 0, width, height);

	vector<Object>objects(2);//4������
	const char *objPath = "C:/Users/Fullmoon/Desktop/hehhe.obj";
	readData(objects, objPath);//�Դ����õķ�ʽ���룬��ȡ����
	objects[0].setBufferAndVertexArray();
	objects[0].seTexture("texture/LogWall_A.tga");
	objects[1].setBufferAndVertexArray();
	objects[1].seTexture("texture/WallCuts_A.tga");
	
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

	model = glm::translate(model, glm::vec3(-0.1, -0.5, -5.0));
	model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01));
	GLuint modelId = glGetUniformLocation(shaderprogram, "model");

	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window)) {
		glm::mat4 view = camera.getView();//�۲����
		GLuint viewId = glGetUniformLocation(shaderprogram, "view");
		//ͶӰ����
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), (float)width / (float)height, 0.1f, 100.0f);
		GLuint projectionId = glGetUniformLocation(shaderprogram, "projection");

		glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));
		glClearColor(0.2, 0.3, 0.3, 1.0);//
		glClear(GL_COLOR_BUFFER_BIT);//�������в��ɸ������VAO��shaderprogram����˳��
		glUseProgram(shaderprogram);
		for (int i = 0; i < objects.size(); i++) 
			objects[i].draw();
		glfwSwapBuffers(window);
		glfwPollEvents();//��ѯ�¼�����
	}

	for (int i = 0; i < objects.size(); i++) 
		objects[i].deleteVBOAndVAOAndTexture();
	glfwTerminate();
	return 0;
}


void frameBufferSizeCallBack(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void readData(vector<Object> &objects, const char *objPath) {//question:��ȡ��ʱ���ַ������������
	ifstream flie(objPath, std::ifstream::in);//ֻ����
	vector<Point>pointsTemp;//�м�����
	vector<Texture>texTemp;//�м�����
	Point point;
	Texture texture_coord;
	string str;
	char ch;//��ȡ/
	int flag = -1;//��־Ŀǰ��ȡ�Ķ���
	int OneTemp,twoTemp;//�����߶�ȡ�ߴ���

	//���ж��ļ��Ƿ��Ѿ�����
	if (flie.is_open()) {
		while (getline(flie, str)) {//���ж�ȡ
			stringstream ss;
			if (str.find("object") != string::npos)
					flag++;

			//ͼԪ����
			if (str.substr(0,1) == "f") {
				ss.clear();//д����߶�����һ��������ʱ���ַ���Ҫ���һ��
				ss << str;//���ַ���д�����У������ȡ������
				string temp;//��Ҫ��Ϊ�˶�ȡ��v/f
				ss >> temp >> OneTemp >> ch >> twoTemp;
				objects[flag].vertices.emplace_back(pointsTemp[OneTemp - 1]);
				objects[flag].texture_coords.emplace_back(texTemp[twoTemp - 1]);
				ss >> OneTemp >> ch >> twoTemp;
				objects[flag].vertices.emplace_back(pointsTemp[OneTemp - 1]);
				objects[flag].texture_coords.emplace_back(texTemp[twoTemp - 1]);
				ss >> OneTemp >> ch >> twoTemp;
				objects[flag].vertices.emplace_back(pointsTemp[OneTemp - 1]);
				objects[flag].texture_coords.emplace_back(texTemp[twoTemp - 1]);
			}

			//��������
			if (str.substr(0,1) == "v" && str.substr(0, 2) != "vt") {
				ss.clear();
				ss << str;
				string temp;
				ss >> temp >> point.x >> point.y >> point.z;
				pointsTemp.emplace_back(point);
			}
			if (str.substr(0, 2) == "vt") {
				ss.clear();
				ss << str;
				string temp;
				ss >> temp >> texture_coord.s >> texture_coord.t;
				texTemp.emplace_back(texture_coord);
			}

		}
	}

	flie.close();//�ر��ļ�
}

void readShaderSource() {
	fstream vfile("vertex.txt");
	fstream ffile("fragment.txt");
	stringstream vss;
	stringstream fss;
	/*string fstr;
	string vstr;*/

	if (vfile.is_open()) {
		vss << vfile.rdbuf();//���ļ������������������뵽�ַ�����
		vstr = vss.str();
		vertex_shader = vstr.c_str();//��ö�����ɫ������
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
	yoffset = last_ypos - ypos;//��������ϵy�����϶��µ������µ�
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

