#include "Shader.h"

Shader::Shader(const char *vShaderPath, const char *fShaderPath) {
	loadShaderSource(vShaderPath, fShaderPath);
	createShaderProgram();
}

void Shader::createShader(const char *shadersource, string type) {
	int success;
	char info[520];
	if (type == "vertex") {
		vertexshader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexshader, 1, &shadersource, NULL);
		glCompileShader(vertexshader);

		glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexshader, 520, NULL, info);
			cout << "VERTEXSHADER ERROR:" << info << endl;
		}
	}
	if (type == "fragment") {
		fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentshader, 1, &shadersource, NULL);
		glCompileShader(fragmentshader);
		glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentshader, 520, NULL, info);
			cout << "FRAGMENTSAHDER ERROR:" << info << endl;
		}
	}
}

void Shader::loadShaderSource(const char *vShaderPath, const char *fShaderPath) {
	fstream vfile(vShaderPath);
	fstream ffile(fShaderPath);
	stringstream vss;
	stringstream fss;

	if (vfile.is_open()) {
		vss << vfile.rdbuf();//把文件流缓冲区的内容输入到字符流中
		string vstr = vss.str();
		const char *vertex_shader = vstr.c_str();//获得顶点着色器代码
		/*for (int i = 0; i < vstr.size(); i++)
			cout << vertex_shader[i];			*/							 
		createShader(vertex_shader, "vertex");
		vfile.close();
	}
	if (ffile.is_open()) {
		fss << ffile.rdbuf();//把文件流的缓冲区内容弄到字符流
		string fstr = fss.str();//fstr把字符流中的字符串全部读出
		const char *fragment_shader = fstr.c_str();
		/*for (int i = 0; i < fstr.size(); i++)
			cout << fragment_shader[i];*/
		createShader(fragment_shader, "fragment");
		ffile.close();
	}
}

void Shader::createShaderProgram() {
	int success;
	char info[520];
	shaderprogram = glCreateProgram();
	glAttachShader(shaderprogram, vertexshader);
	glAttachShader(shaderprogram, fragmentshader);
	glLinkProgram(shaderprogram);
	glGetProgramiv(shaderprogram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderprogram, 520, NULL, info);
		cout << "SHADERPROGRAM ERROR:" << info << endl;
	}
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
}

GLuint Shader::getLocation(const char *name) {
	return glGetUniformLocation(shaderprogram, name);
}

void Shader::activeProgram() {
	glUseProgram(shaderprogram);
}
Shader::~Shader()
{
}
