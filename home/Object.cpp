#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Object.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Object::setBufferAndVertexArray() {
	auto verSize = sizeof(Point)*vertices.size();
	auto texSize = sizeof(Texture)*texture_coords.size();
	auto totalSize = verSize + texSize;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, totalSize, NULL, GL_STATIC_DRAW);//创建足够大的缓冲区
	glBufferSubData(GL_ARRAY_BUFFER, 0, verSize, &vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, verSize, texSize, &texture_coords[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(sizeof(Point)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

//path 图片路径,value纹理单元值
void Object::seTexture(const char *path) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);//生成纹理对象
	//设置参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//纹素比像素小时，加权平均
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//大时
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//加载数据
	stbi_set_flip_vertically_on_load(true);//反转y值
	int width, height, channels;
	unsigned char *data = stbi_load(path, &width, &height, &channels, 0);//把纹理解码为图像数据存储
	if (data) {
		//把纹素弄进GPU
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);//多级渐远纹理
	}
	else {
		cout << "Failed to load textue" << endl;
	}
	stbi_image_free(data);//释放数据
	
}

void Object::deleteVBOAndVAOAndTexture() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &texture);
}

void Object::draw() {
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Object::print() {
	for (int i = 0; i < vertices.size(); i++) {
		cout << vertices[i].x<<"  "<< vertices[i].y<<"  "<< vertices[i].z << endl;
	}
	cout << "-------------------" << endl;
	for (int i = 0; i < texture_coords.size(); i++) {
		cout << texture_coords[i].s << "  " << texture_coords[i].t<< endl;
	}
}