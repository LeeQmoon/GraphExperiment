#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Object.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Object::Object() {
	maxsize = 20000;// 更改后  -------------------------log----------------------
	vertices = new Point[maxsize];
	texture_coords = new Texture[maxsize];
	normal = new Point[maxsize];
	// maxsize = 50000; // 更改前  -------------------------log----------------------
	verSize = 0;
	texSize = 0;
	norSize = 0;
	for (int i = 0; i < 3; i++)
		this->texturecount[i] = 0;
}

Object::Object(const Object &object) {
	
}

void Object::setBufferAndVertexArray() {
	verSize = sizeof(Point)*verSize;
	texSize = sizeof(Texture)*texSize;
	norSize = sizeof(Point)*norSize;
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
	if (this->material.map_Kd != "")
		this->texturecount[0] = 1;
	if (this->material.map_Ks != "")
		this->texturecount[1] = 1;
	int width, height, channels;
	unsigned char *data;
	glGenTextures(2, texture);
	if (this->texturecount[0]) {
		glBindTexture(GL_TEXTURE_2D, texture[0]);//生成纹理对象
											  //设置参数
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		//纹素比像素小时，加权平均
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//大时
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//加载数据
		stbi_set_flip_vertically_on_load(true);//反转y值
		//material.map_Ka.c_str()
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

	if (this->texturecount[1]) {
		glBindTexture(GL_TEXTURE_2D, texture[1]);//生成纹理对象
												 //设置参数
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		//纹素比像素小时，加权平均
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//大时
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		stbi_set_flip_vertically_on_load(true);//反转y值
		data = stbi_load(material.map_Ks.c_str(), &width, &height, &channels, 0);//把纹理解码为图像数据存储
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
	glUniform1iv(glGetUniformLocation(shaderprogram, "texturecount"), 1, &texturecount[0]);
	if (this->texturecount[0]) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
	}
	if (this->texturecount[1]) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
	}
	glBindVertexArray(VAO);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, verSize/sizeof(Point));
}

void Object::print() {
	cout << this->material.map_Kd << "  " << this->material.map_Ks << endl;
	/*vertices;
	cout << this->normal[0].x << "  " << this->normal[0].y << "  " << this->normal[0].z << endl;
	for (int i = 0; i < verSize; i++) {
		cout << vertices[i].x << "  " << vertices[i].y << "  " << vertices[i].z << endl;
	}
	cout << "-------------------" << endl;*/
	//for (int i = 0; i < texSize; i++) {
	//	cout << texture_coords[i].s << "  " << texture_coords[i].t << endl;
	//}
	/*cout << "versize: " << verSize << endl;
	cout << "texsize: " << texSize << endl;
	cout << "norsize: " << norSize << endl;
	cout << endl;
	cout << "Ns: " << material.Ns << endl;
	cout << "d: " << material.d << endl;
	cout << "illum: " << material.illum << endl;
	cout << "Ka: " << material.Ka.x << "  " << material.Ka.y << "  " << material.Ka.z << endl;
	cout << "Kd: " << material.Kd.x << "  " << material.Kd.y << "  " << material.Kd.z << endl;
	cout << "Ks: " << material.Ks.x << "  "<<material.Ks.y<<"  " << material.Ks.z << endl;
	cout << "map_Ka: " << material.map_Ka << endl;
	cout << "map_Kd: " << material.map_Kd << endl;
	cout << endl;*/
}

Object::~Object() {
	delete[]vertices;
	delete[]texture_coords;
	delete[]normal;
}