#include "ObjLoader.h"
#include <fstream>
#include <cstdlib>    //使用exit必須include
#include <string>
#include<sstream> 


ObjLoader::ObjLoader() {
    ifstream file("octahedron.obj");
    file.close();
}

ObjLoader::ObjLoader(string filename) {
	ifstream file(filename);    //cin
	string line;

    while (getline(file, line)) {   // 從終端接收一行字符串，並放入字符串line中 
        if (line.substr(0, 1) == "v") {     //擷取每行 0-1 位置的val
            vector<GLfloat> Point;
            GLfloat x, y, z;

            istringstream s(line.substr(2));    //把line 2之後的所有字元 存入字符串流s中 
            s >> x; s >> y; s >> z;
            Point.push_back(x);
            Point.push_back(y);
            Point.push_back(z);
            vSets.push_back(Point);
           // cout << "x:" << x << "\ny:" << y << "\nz:" << z << "\n";
        }
        else if (line.substr(0, 1) == "f") {
            vector<GLint> vIndexSets;
            GLint u, v, w;
            istringstream f_s(line.substr(2));    //把line 2之後的所有字元 存入字符串流s中 
            f_s >> u; f_s >> v; f_s >> w;
            vIndexSets.push_back(u - 1); //.obj's index start from 1, vector's indext start from 0,so need to transform to vector index
            vIndexSets.push_back(v - 1);
            vIndexSets.push_back(w - 1);
            fSets.push_back(vIndexSets);
        }

    }
    file.close();

    findBoundingBox();
}

void ObjLoader::findBoundingBox() {
    minX = (vSets[0])[0];
    minY = (vSets[0])[1];
    minZ = (vSets[0])[2];
    maxX = (vSets[0])[0];
    maxY = (vSets[0])[1];
    maxZ = (vSets[0])[2];

    for (int i = 0; i < vSets.size(); i++) {
        
        GLfloat curX = (vSets[i])[0];
        GLfloat curY = (vSets[i])[1];
        GLfloat curZ = (vSets[i])[2];

        if (curX > maxX) {
            maxX = curX;
        }

        if (curX < minX) {
            minX = curX;
        }

        if (curY > maxY) {
            maxY = curY;
        }

        if (curY < minY) {
            minY = curY;
        }

        if (curZ > maxZ) {
            maxZ = curZ;
        }

        if (curZ < minZ) {
            minZ = curZ;
        }
    }

}


void ObjLoader::DrawFace() {
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < fSets.size(); i++) {
        GLfloat VN[3]; //法線,用於光照渲染
        vertex sv1,sv2,sv3; //三個頂點
        if (fSets[i].size() != 3) {
            cout << "ObjLoader error,The size of f is not 3\n";
        }
        else {
            GLint firstVertexIndex = (fSets[i])[0]; //取出頂點index
            GLint sencondtVertexIndex = (fSets[i])[1];
            GLint thirdVertexIndex = (fSets[i])[2];

            sv1.x = (vSets[firstVertexIndex])[0]; // first vertex
            sv1.y = (vSets[firstVertexIndex])[1];
            sv1.z = (vSets[firstVertexIndex])[2];

            sv2.x = (vSets[sencondtVertexIndex])[0]; // second vertex
            sv2.y = (vSets[sencondtVertexIndex])[1];
            sv2.z = (vSets[sencondtVertexIndex])[2];

            sv3.x = (vSets[thirdVertexIndex])[0]; // third vertex
            sv3.y = (vSets[thirdVertexIndex])[1];
            sv3.z = (vSets[thirdVertexIndex])[2];

            GLfloat vec1[3], vec2[3], vec3[3]; ///計算法向量

            //(x2-x1,y2-y1,z2-z1)
            vec1[0] = sv1.x - sv2.x;
            vec1[1] = sv1.y - sv2.y;
            vec1[2] = sv1.z - sv2.z;


            //(x3-x2,y3-y2,z3-z2)
            vec2[0] = sv1.x - sv3.x;
            vec2[1] = sv1.y - sv3.y;
            vec2[2] = sv1.z - sv3.z;

            //(x3-x1,y3-y1,z3-z1)
            vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
            vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
            vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

            GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2)); //pow=次方

            VN[0] = vec3[0] / D;
            VN[1] = vec3[1] / D;
            VN[2] = vec3[2] / D;
            
            glNormal3f(VN[0], VN[1], VN[2]);//繪製法向量

           
            //glColor3f(0.4f, 0.5f, 0); glVertex3f(sv1.x, sv1.y, sv1.z);//繪製三角面
            //glColor3f(0, 0.1f, 0.5f); glVertex3f(sv2.x, sv2.y, sv2.z);
            //glColor3f(0.87f, 0, 0); glVertex3f(sv3.x, sv3.y, sv3.z);
            //繪製三角面
            glColor3f((float)rand() / (RAND_MAX + 1.0), (float)rand() / (RAND_MAX + 1.0), (float)rand() / (RAND_MAX + 1.0));
            glVertex3f(sv1.x, sv1.y, sv1.z);
            glColor3f((float)rand() / (RAND_MAX + 1.0), (float)rand() / (RAND_MAX + 1.0), (float)rand() / (RAND_MAX + 1.0));
            glVertex3f(sv2.x, sv2.y, sv2.z);
            glColor3f(0.87f, 0, 0); glVertex3f(sv3.x, sv3.y, sv3.z);
        }
    }
    glEnd();
}

void ObjLoader::DrawPoint(int mode) {
    glDisable(GL_LIGHTING);

    glPointSize(3);
    if (mode == 0)   //single color 
        glColor3f(0,0, 0);//BLACK
    else if (mode == 1) {
        glColor3f((float)rand() / (RAND_MAX + 1.0), (float)rand() / (RAND_MAX + 1.0), (float)rand() / (RAND_MAX + 1.0));
    }
    glBegin(GL_POINTS);
    for (int i = 0; i < fSets.size(); i++) {
        vertex sv1, sv2, sv3; //三個頂點
        if (fSets[i].size() != 3) {
            cout << "ObjLoader error,The size of f is not 3\n";
        }
        else {
            GLint firstVertexIndex = (fSets[i])[0]; //取出頂點index
            GLint sencondtVertexIndex = (fSets[i])[1];
            GLint thirdVertexIndex = (fSets[i])[2];

            sv1.x = (vSets[firstVertexIndex])[0]; // first vertex
            sv1.y = (vSets[firstVertexIndex])[1];
            sv1.z = (vSets[firstVertexIndex])[2];

            sv2.x = (vSets[sencondtVertexIndex])[0]; // second vertex
            sv2.y = (vSets[sencondtVertexIndex])[1];
            sv2.z = (vSets[sencondtVertexIndex])[2];

            sv3.x = (vSets[thirdVertexIndex])[0]; // third vertex
            sv3.y = (vSets[thirdVertexIndex])[1];
            sv3.z = (vSets[thirdVertexIndex])[2];


           /* glColor3f(0, 1, 0); glVertex3f(sv1.x, sv1.y, sv1.z);
            glColor3f(0, 0, 1); glVertex3f(sv2.x, sv2.y, sv2.z);
            glColor3f(1, 0, 0); glVertex3f(sv3.x, sv3.y, sv3.z);*/
            //繪製POINT   
            glVertex3f(sv1.x, sv1.y, sv1.z);
            glVertex3f(sv2.x, sv2.y, sv2.z);
            glVertex3f(sv3.x, sv3.y, sv3.z);
        }
    }

    glEnd();
    glEnable(GL_LIGHTING);


}
void ObjLoader::DrawLine(int mode) {
    glDisable(GL_LIGHTING);
   
    glLineWidth(1.0f);
    //cout << "color mode:" << mode << endl;
    if (mode == 0)   //single color 
        glColor3f(0.5f, 0.5f, 0.5f);//GRAY
    else if (mode == 1) {
        glColor3f((float)rand() / (RAND_MAX + 1.0), (float)rand() / (RAND_MAX + 1.0), (float)rand() / (RAND_MAX + 1.0));
    }
   
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < fSets.size(); i++) {
        vertex sv1, sv2, sv3; //三個頂點
        if (fSets[i].size() != 3) {
            cout << "ObjLoader error,The size of f is not 3\n";
        }
        else {
            GLint firstVertexIndex = (fSets[i])[0]; //取出頂點index
            GLint sencondtVertexIndex = (fSets[i])[1];
            GLint thirdVertexIndex = (fSets[i])[2];

            sv1.x = (vSets[firstVertexIndex])[0]; // first vertex
            sv1.y = (vSets[firstVertexIndex])[1];
            sv1.z = (vSets[firstVertexIndex])[2];

            sv2.x = (vSets[sencondtVertexIndex])[0]; // second vertex
            sv2.y = (vSets[sencondtVertexIndex])[1];
            sv2.z = (vSets[sencondtVertexIndex])[2];

            sv3.x = (vSets[thirdVertexIndex])[0]; // third vertex
            sv3.y = (vSets[thirdVertexIndex])[1];
            sv3.z = (vSets[thirdVertexIndex])[2];

           
            /* glColor3f(0, 1, 0); glVertex3f(sv1.x, sv1.y, sv1.z);
            glColor3f(0, 0, 1); glVertex3f(sv2.x, sv2.y, sv2.z);
            glColor3f(1, 0, 0); glVertex3f(sv3.x, sv3.y, sv3.z);*/
            //繪製LINE
            //glColor3f((float)rand() / (RAND_MAX + 1.0), (float)rand() / (RAND_MAX + 1.0), (float)rand() / (RAND_MAX + 1.0));
            glVertex3f(sv1.x, sv1.y, sv1.z);
            //glColor3f((float)rand() / (RAND_MAX + 1.0), (float)rand() / (RAND_MAX + 1.0), (float)rand() / (RAND_MAX + 1.0));
            glVertex3f(sv2.x, sv2.y, sv2.z);
            //glColor3f(0.87f, 0, 0); 
            glVertex3f(sv3.x, sv3.y, sv3.z);
        }
    }

    glEnd();
    glEnable(GL_LIGHTING);

}


GLfloat ObjLoader::getXlength() {
  //  cout << "Xlength:" << (abs(minX) + abs(maxX)) / 2 << endl;
    return (abs(minX) + abs(maxX)) / 2;

}
GLfloat ObjLoader::getYlength() {
   // cout << "Ylength:" << (abs(minY) + abs(maxY)) / 2 << endl;
    return (abs(minY) + abs(maxY)) / 2;
}

GLfloat ObjLoader::getZlength() {
  //  cout << "Zlength:" << (abs(minZ) + abs(maxZ)) / 2 << endl;
    return (abs(minZ) + abs(maxZ)) / 2;
}

GLfloat ObjLoader::getminX() {
    return minX;
}
GLfloat ObjLoader::getminY() {
    return minY;
}
GLfloat ObjLoader::getminZ() {
    return minZ;
}
GLfloat ObjLoader::getmaxX() {
    return maxX;
}
GLfloat ObjLoader::getmaxY() {
    return maxY;
}
GLfloat ObjLoader::getmaxZ() {
    return maxZ;
}