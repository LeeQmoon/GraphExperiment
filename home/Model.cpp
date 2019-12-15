#include "Model.h"
#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;

Model::Model(const string &path) {
	ObjPath = path;
	// maxsize = 30 // ����ǰ  -------------------------log----------------------
	size = 0;
}

void Model::readObj() {
	//����obj·��
	fstream ffile(ObjPath);
	string str;
	int v = 0, t = 0, n = 0;//��¼�����������
	int flag = -1;//��¼�ֵ��Ǹ�������
	Point pointTemp[20000];//��ʱ��������
	Texture textureTemp[10000];//��ʱ��������
	Point normalTemp[15000];//��ʱ����������
	int vv = 0, tt = 0, nn = 0;
	if (ffile.is_open()) {
		//�ж�
		while (getline(ffile, str)) {
			stringstream ss;
			string temp;
			//cout << str << endl;
			ss << str;
			ss >> temp;
			if (temp == "mtllib") {//������ʿ�·��
				ss >> temp;
				this->matklu = temp;
				//cout << "matklu: " << matklu << endl;
			}
			else if (temp == "v") {
				Point point;
				ss >> point.x >> point.y >> point.z;
				pointTemp[vv++] = point;
			}
			else if (temp == "vn") {
				Point point;
				ss >> point.x >> point.y >> point.z;
				normalTemp[nn++] = point;
			}
			else if (temp == "vt") {
				Texture tex;
				ss >> tex.s >> tex.t;
				textureTemp[tt++] = tex;
			}
			else if (temp == "usemtl") {
				ss >> temp;
				//Object oo(temp);
				this->objects.emplace_back(temp);
				flag++;
			}
			else if (temp == "f") {
				char ch;//����'/'
				int verIndex, texIndex, norIndex;
				for (int i = 0; i < 3; i++) {//������ͼԪ
					ss >> verIndex >> ch >> texIndex >> ch >> norIndex;
					this->objects[flag].vertices.emplace_back(pointTemp[verIndex - 1].x, pointTemp[verIndex - 1].y, pointTemp[verIndex - 1].z);
					this->objects[flag].normal.emplace_back(normalTemp[norIndex - 1].x, normalTemp[norIndex - 1].y, normalTemp[norIndex - 1].z);
					this->objects[flag].texture_coords.emplace_back(textureTemp[texIndex - 1].s, textureTemp[texIndex - 1].t);
					//this->objects[flag].normal.emplace_back(normalTemp[norIndex - 1].x, normalTemp[norIndex - 1].y, normalTemp[norIndex - 1].z);

				}
			}
		}
	}

	ffile.close();
	readMtl();
}

void Model::readMtl() {
	//���ز��ʿ��ļ�
	//matklu = "test.mtl";
	fstream ffile(matklu);
	//������ʱ���ʿ�
	Material materialTemp[10];//������ʿ�ֻ��10������
	int mm = 0;
	string str;
	int flag = -1;//��¼�ڼ�������
	if (ffile.is_open()) {
		while (getline(ffile, str)) {
			//cout << str<<endl;
			stringstream ss;
			string temp;
			ss << str;
			ss >> temp;
			if (temp == "newmtl") {
				flag++;
				Material mat;
				ss >> temp;
				mat.material_name = temp;//���������
				materialTemp[mm++] = mat;

			}
			else if (temp == "Ns") {
				float Ns;
				ss >> Ns;
				materialTemp[flag].Ns = Ns;
			}
			else if (temp == "d") {
				float d;
				ss >> d;
				materialTemp[flag].d = d;
			}
			else if (temp == "illum") {
				int illum;
				ss >> illum;
				materialTemp[flag].illum = illum;
			}
			else if (temp == "Ka") {
				glm::vec3 Ka;
				ss >> Ka.x >> Ka.y >> Ka.z;
				materialTemp[flag].Ka = Ka;
			}
			else if (temp == "Kd") {
				glm::vec3 Kd;
				ss >> Kd.x >> Kd.y >> Kd.z;
				materialTemp[flag].Kd = Kd;
			}
			else if (temp == "Ks") {
				glm::vec3 Ks;
				ss >> Ks.x >> Ks.y >> Ks.z;
				materialTemp[flag].Ks = Ks;
			}
			else if (temp == "map_Ks") {
				ss >> temp;
				materialTemp[flag].map_Ks = temp;
			}
			else if (temp == "map_Kd") {
				ss >> temp;
				materialTemp[flag].map_Kd = temp;
			}
			else if (temp == "map_Ka") {
				ss >> temp;
				materialTemp[flag].map_Ka = temp;
			}
		}
	}
	ffile.close();
	processMaterial(materialTemp, mm);
}

void Model::processMaterial(Material *materialTemp, int count) {
	for (int i = 0; i < objects.size(); i++) {
		for (int m = 0; m < count; m++) {
			if (materialTemp[m].material_name == objects[i].material.material_name) {
				objects[i].material = materialTemp[m];//��������
				break;
			}
		}
	}

	//���ĺ�----lee---
	//ֻ����һ��VBO,VAO��Ҫ���᲻�ϵ������ڴ�
	for (int i = 0; i < objects.size(); i++) {

		//objects[i].caculateTangent();
		objects[i].seTexture();
		objects[i].setBufferAndVertexArray();
		//objects[i].print();
	}

}

void Model::display(unsigned int shaderprogram) {
	for (int i = 0; i < objects.size(); i++) {
		//---����ǰ----
		//objects[i].setBufferAndVertexArray();
		//objects[i].seTexture();
		objects[i].draw(shaderprogram);
	}
}

Model::~Model() {
	for (int i = 0; i < size; i++) {
		objects[i].deleteVBOAndVAOAndTexture();
	}
}
