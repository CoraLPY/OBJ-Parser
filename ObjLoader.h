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
	vector<vector<GLfloat>> vSets; //存放頂點(x,y,z)座標
	vector<vector<GLint>> fSets; ////存放面的三個頂點索引
	GLfloat minX, minY, minZ;
	GLfloat maxX, maxY, maxZ;

};