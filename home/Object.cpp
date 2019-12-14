#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Object.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Object::Object() {
	this->VAO = 0;
	this->VBO = 0;
	for (int i = 0; i < 2; i++)
		this->texturecount[i] = 0;
}
Object::Object(string material_name) {
	this->VAO = 0;
	this->VBO = 0;
	for (int i = 0; i < 2; i++)
		this->texturecount[i] = 0;
	this->material.material_name = material_name;
	
}
void Object::setBufferAndVertexArray() {
	auto verSize = sizeof(Point)*this->vertices.size();
	auto texSize = sizeof(Texture)*this->texture_coords.size();
	auto norSize = sizeof(Point)*this->normal.size();
	auto totalSize = verSize + texSize + norSize;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);//创建足够大的缓冲区
	glBufferSubData(GL_ARRAY_BUFFER, 0, verSize, &vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, verSize, texSize, &texture_coords[0]);
	glBufferSubData(GL_ARRAY_BUFFER, verSize + texSize, norSize, &normal[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(verSize));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(verSize + texSize));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

//path 图片路径,value纹理单元值
void Object::seTexture() {
	if (this->material.material_name == "map_4_lambert5SG") {
		this->material.emission = glm::vec3(0.0, 0.0, 0.8);
	}
	else
		this->material.emission = glm::vec3(0.0, 0.0, 0.0);

	if (this->material.map_Ka != "")
		this->texturecount[0] = 1;
	if (this->material.map_Kd != "")
		this->texturecount[1] = 1;

	int width, height, channels;
	unsigned char *data;
	stbi_set_flip_vertically_on_load(true);//反转y值
	glGenTextures(2, texture);//最多两张

	if (this->texturecount[0]) {
		glBindTexture(GL_TEXTURE_2D, texture[0]);//生成纹理对象
												 //设置参数
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		//纹素比像素小时，加权平均
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//大时
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//material.map_Ka.c_str()
		data = stbi_load(material.map_Ka.c_str(), &width, &height, &channels, 0);//把纹理解码为图像数据存储
		if (data) {
			//把纹素弄进GPU
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);//多级渐远纹理
		}
		else {
			cout << "Failed to load texture" << endl;
		}
		stbi_image_free(data);//释放数据
	}

	if (this->texturecount[1]) {
		glBindTexture(GL_TEXTURE_2D, texture[1]);//生成纹理对象
												 //设置参数
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		//纹素比像素小时，加权平均
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//大时
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		data = stbi_load(material.map_Kd.c_str(), &width, &height, &channels, 0);//把纹理解码为图像数据存储
		if (data) {
			//把纹素弄进GPU
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);//多级渐远纹理
		}
		else {
			cout << "Failed to load texture" << endl;
		}
		stbi_image_free(data);//释放数据
	}
}

void Object::deleteVBOAndVAOAndTexture() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(2, texture);
}

void Object::draw(unsigned int shaderprogram) {
	glm::ivec2 tt(texturecount[0], texturecount[1]);
	glUniform3iv(glGetUniformLocation(shaderprogram, "texturecount"), 1, &tt[0]);
	if (this->texturecount[0]) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glUniform1i(glGetUniformLocation(shaderprogram, "tex0"), 0);
	}
	if (this->texturecount[1]) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glUniform1i(glGetUniformLocation(shaderprogram, "tex1"), 1);
	}
	//传材质属性到shader
	glUniform1f(glGetUniformLocation(shaderprogram, "material.Ns"), material.Ns);
	glUniform3fv(glGetUniformLocation(shaderprogram, "material.Ka"), 1, &material.Ka[0]);
	glUniform3fv(glGetUniformLocation(shaderprogram, "material.Kd"), 1, &material.Kd[0]);
	glUniform3fv(glGetUniformLocation(shaderprogram, "material.Emission"), 1, &(material.emission[0]));
	glBindVertexArray(VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
}

void Object::print() {
	/*cout << "versize = " << vertices.size() << endl;
	for (int i = 0; i < vertices.size(); i++)
		cout << vertices[i].x << "  " << vertices[i].y << "  " << vertices[i].z << endl;
	cout << "tangent: " << endl;
	for (int i = 0; i < tangent.size(); i++)
		cout << tangent[i].x << "  " << tangent[i].y << "  " << tangent[i].z << endl;
	cout << "normal: " << endl;
	for (int i = 0; i < normal.size(); i++)
		cout << normal[i].x << "  " << normal[i].y << "  " << normal[i].z << endl;*/
	cout << this->material.emission.x << "  " << this->material.emission.y << "  " << this->material.emission.z << endl;

}
Object::~Object() {
}