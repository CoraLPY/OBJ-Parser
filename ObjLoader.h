#pragma once
#include <vector>
#include <iostream>
#include <./GL/freeglut.h>
#include<iostream>
using namespace std;


class ObjLoader {
public:
	struct vertex {
		float x;
		float y;
		float z;
	};
	ObjLoader();
	ObjLoader(string filename); 
	void findBoundingBox();
	void DrawFace();
	void DrawPoint(int mode);
	void DrawLine(int mode);
	GLfloat getminX();
	GLfloat getminY();
	GLfloat getminZ();
	GLfloat getmaxX();
	GLfloat getmaxY();
	GLfloat getmaxZ();
	GLfloat getXlength();
	GLfloat getYlength();
	GLfloat getZlength();


private:
	vector<vector<GLfloat>> vSets; //�s���I(x,y,z)�y��
	vector<vector<GLint>> fSets; ////�s�񭱪��T�ӳ��I����
	GLfloat minX, minY, minZ;
	GLfloat maxX, maxY, maxZ;

};