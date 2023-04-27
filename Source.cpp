
#include <stdio.h> 
#include <stdlib.h>
#include <cmath>
/*** freeglut***/
#include <./GL/freeglut.h>
#include<iostream>
#include <algorithm>
#include "ObjLoader.h"
using namespace std;

#define PI 3.14159265


void ChangeSize(int, int);
void RenderScene(void);
void SetupRC();
void myKeyboard(unsigned char key, int x, int y);
void My_Mouse(int button, int state, int x, int y);
void mySpecialKey(int key, int x, int y);
void RotateMatrix(float angle, GLfloat X, GLfloat Y, GLfloat Z);
void MatrixReset();
void mainMenu(int index);
void fileMenu1(int index);
void renderMenu2(int index);
void colorMenu3(int index);
void BuildPopupMenu();
void initObjModel();  
void setGenList();


int window_h;  //screen size
int window_w;
GLfloat maxBoundingBoxLenght;  //max BoundingBox Lenght  of x,y,z axis in obj model 
GLfloat scaledRatio; // scaledRatio for scale obj model  to fit the screen 
float tx, ty, tz, thetaX, thetaY, thetaZ; //rotation x,y,z
float cx, cy, cz; //camera x,y,z axis
float look_x, look_y, look_z;//   point's  x,y,z axis that camera look at 
GLfloat angle; //rotation angle
GLfloat dot1X, dot1Y, dot2X, dot2Y; //2 dots for controling arbitrary axis
bool showDot, click; //control dot1 or dot2
ObjLoader obj; 
GLuint showPointList, showLineList,showFaceList; //save init obj model state
bool showFace, showLine,showPoint; //control the display mode 
int colorMode;
//GLfloat scaledRatioSet[4]{ 0.0095, 0.0454, 0.492, 0.8 }; //file scaledRatioSet
GLfloat scaledlocationRatio; //scaled ratio for convert screen location to opengl location when mouse down 

GLfloat rotXMatrix[] = {
1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0 };

GLfloat rotYMatrix[] = {
1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0 };

GLfloat rotZMatrix[] = {
1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0 };

GLfloat translateMatrix[] = {
1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0 };

GLfloat multiMatrix[] = {
1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0 };

//set Light
void setLightRes() {
    GLfloat lightPosition[] = { 0.0f, 0.0f, 1.0f, 0.0f }; 
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glEnable(GL_LIGHTING); //enable light
    glEnable(GL_LIGHT0);   //使用指定light


}


void setGenList() {
    showFaceList = glGenLists(1);//creat gen list加速渲染
    showPointList = glGenLists(1);
    showLineList = glGenLists(1);

 
     // set model display mode lists to pre-compile obj model state, waiting for callList() to display obj model
    //set Face mode
    glNewList(showFaceList, GL_COMPILE);
    obj.DrawFace();
    glEndList();

    //set line mode
    glNewList(showLineList, GL_COMPILE);
    obj.DrawLine(colorMode);
    glEndList();

    //set point mode
    glNewList(showPointList, GL_COMPILE);
    obj.DrawPoint(colorMode);
    glEndList();

    
}

void initObjModel() {
    maxBoundingBoxLenght = std::max({ obj.getXlength(),obj.getYlength(),obj.getZlength() });
    scaledRatio = 1 / maxBoundingBoxLenght*0.95;


    glClearColor(0.0, 0.0, 0.0, 0.0);
    //visible object's depth
    glClearDepth(2.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST); 
    setLightRes();//set light
    SetupRC();//设置环境光
    colorMode = 0;
    setGenList(); // set glGenLists 
}

int main(int argc, char** argv)
{

     if (argv[1] != NULL) {
         obj = ObjLoader(argv[1]);

     }
     else {
         obj = ObjLoader("teddy.obj");    
     }
     
    window_h = 400;
    window_w = 400;
    
    if (window_w > window_h)
        scaledlocationRatio = (static_cast<GLdouble>(window_h) / window_w) /( window_w / 2);
    else
        scaledlocationRatio = 1 / (window_w / 2);

    maxBoundingBoxLenght = std::max({obj.getXlength(),obj.getYlength(),obj.getZlength()});
    scaledRatio = 1 / maxBoundingBoxLenght * 0.75;
    cout<<"scaledRatio:"<< scaledRatio<<endl;
    showFace = true;
    showLine = false;
    showPoint = false;
    colorMode = 0; //single color mode
 

    angle = 5;

    tx = 0.f;
    ty = 0.f;
    tz = 0.f;
    thetaX = 0.f;
    thetaY = 0.f;
    thetaZ = 0.f;

    cx = 0.f;
    cy = 0.f;
    cz = 10.0f;

    dot1X = 0.f;
    dot1Y = 0.f;
    dot2X = 0.f;
    dot2Y = 0.f;
    showDot = false;
    click = false;

    glutInit(&argc, argv);
     window_h = glutGet(GLUT_WINDOW_HEIGHT);
     window_w = glutGet(GLUT_WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window_w, window_h);
    glutInitWindowPosition(600, 80);
    glutCreateWindow("OBJ Parser");

    initObjModel();

   // SetupRC();

    BuildPopupMenu();

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(myKeyboard);
    glutMouseFunc(My_Mouse);
    glutSpecialFunc(mySpecialKey);
    glutMainLoop();	
    return 0;
}

void SetupRC() {
    // Light values and coordinates
    GLfloat whiteLight[] = { 0.45f, 0.45f, 0.45f, 1.0f };

    GLfloat sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat lightPos[] = { 0.f, 25.0f, 20.0f, 0.0f };
    // Enable lighting
    glEnable(GL_LIGHTING);
    // Setup and enable light 0
    // set light model, whiteLigh的RGBA强度值用於環境光
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);
    // Enable color tracking 
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);

}
void ChangeSize(int w, int h)
{
    printf("Window Size= %d X %d\n", w, h); 
    glViewport(0, 0, w, h);//改變顯示區域，起始位置爲SCREEN左下角（非座標原點）
    glMatrixMode(GL_PROJECTION); //修改投影矩陣
    glLoadIdentity();//導入單位陣
 
    //glOrtho(-maxBoundingBoxLenght, maxBoundingBoxLenght, -maxBoundingBoxLenght, maxBoundingBoxLenght, -maxBoundingBoxLenght, maxBoundingBoxLenght);
    //resize when change window size, 設置一個矩形框架，截取包含其中的圖形用於顯示.
    if (w > h)
        glOrtho(-w / h, static_cast<GLdouble>(w) / h, -1.0, 1.0, -100.0, 100.0);
    else
        glOrtho(-1.0, 1.0, -static_cast<GLdouble>(h) / w, static_cast<GLdouble>(h) / w, -100.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


   
  
}
void RenderScene(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //清除之前的depth and color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cx, cy, cz, look_x, look_y, look_z, 0, 1, 0);

    //glPushMatrix(); // push the matrix onto the matrix stack
   

    //old
    //draw X-axis, Y-axis and Z-axis
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    GLfloat x = 1.0f;
    glVertex3f(-x, 0, 0);
    glVertex3f(x, 0, 0);
    glEnd();

    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    GLfloat y =1.0f;
    glVertex3f(0, -y, 0);
    glVertex3f(0, y, 0);
    glEnd();

    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
    GLfloat z = 1.0f;
    glVertex3f(0, 0, -z);
    glVertex3f(0, 0, z);
    glEnd();

    //DRAW DOT
    glPointSize(6.0);
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    glVertex3f(dot1X, dot1Y, 0.f);
    glVertex3f(dot2X, dot2Y, 0.f);
    glEnd();


    //draw line between dots 
    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(dot1X, dot1Y, 0.f);
    glVertex3f(dot2X, dot2Y, 0.f);
    glEnd();

    //glRotatef(thetaX, 1, 0, 0);
    //glRotatef(thetaY, 0, 1, 0);
   // glRotatef(thetaZ, 0, 0, 1);
    //lab03
    rotXMatrix[5] = cos(thetaX * PI / 180.0);
    rotXMatrix[6] = -sin(thetaX * PI / 180.0);
    rotXMatrix[9] = sin(thetaX * PI / 180.0);
    rotXMatrix[10] = cos(thetaX * PI / 180.0);

    rotYMatrix[0] = cos(thetaY * PI / 180.0);
    rotYMatrix[2] = sin(thetaY * PI / 180.0);
    rotYMatrix[8] = -sin(thetaY * PI / 180.0);
    rotYMatrix[10] = cos(thetaY * PI / 180.0);

    rotZMatrix[0] = cos(thetaZ * PI / 180.0);
    rotZMatrix[0] = cos(thetaZ * PI / 180.0);
    rotZMatrix[1] = -sin(thetaZ * PI / 180.0);
    rotZMatrix[4] = sin(thetaZ * PI / 180.0);
    rotZMatrix[5] = cos(thetaZ * PI / 180.0);

    glMultMatrixf(rotXMatrix);
    glMultMatrixf(rotYMatrix);
    glMultMatrixf(rotZMatrix);





//lab 04 - Rotate your object along the arbitrary axis
    GLfloat new_x = dot2X - dot1X;
    GLfloat new_y = dot2Y - dot1Y;
    GLfloat new_z = 0 - 0;
    //cube height
    GLfloat cube_h = sqrt(new_x * new_x + new_y * new_y + new_z * new_z);
    //Rotate your object along the arbitrary axis
    RotateMatrix(angle, new_x / cube_h, new_y / cube_h, new_z / cube_h);


    //glTranslatef(tx, ty, tz); 
    //平移
    translateMatrix[12] = tx;
    translateMatrix[13] = ty;
    translateMatrix[14] = tz;
    glMultMatrixf(translateMatrix);



    //scale obj model to fit the clip space
    glScalef(scaledRatio, scaledRatio, scaledRatio);

    if(showFace)
        glCallList(showFaceList);
    if (showLine)
        //obj.DrawLine(colorMode);
        glCallList(showLineList);
    if (showPoint)
        glCallList(showPointList);


    glutSwapBuffers();

}

//Rotate your object along the arbitrary axis
void RotateMatrix(float angle, GLfloat X, GLfloat Y, GLfloat Z) {
    MatrixReset();
    GLfloat Cos = cos(angle * PI / 180);
    GLfloat Sin = sin(angle * PI / 180);
    multiMatrix[0] = Cos + (1 - Cos) * X * X;
    multiMatrix[1] = (1 - Cos) * X * Y - (Sin * Z);
    multiMatrix[2] = (1 - Cos) * X * Z + (Sin * Y);
    multiMatrix[4] = (1 - Cos) * Y * X + (Sin * Z);
    multiMatrix[5] = Cos + (1 - Cos) * Y * Y;
    multiMatrix[6] = (1 - Cos) * Y * Z - (Sin * X);
    multiMatrix[8] = (1 - Cos) * Z * X - (Sin * Y);
    multiMatrix[9] = (1 - Cos) * Z * Y + (Sin * X);
    multiMatrix[10] = Cos + (1 - Cos) * Z * Z;


    glMultMatrixf(multiMatrix);

}

void MatrixReset() {
    for (int i = 0; i < 16; i++) {
        if (i == 0 || i == 5 || i == 10 || i == 15) {
            multiMatrix[i] = 1;
        }
        else {
            multiMatrix[i] = 0;
        }

    }

}


void My_Mouse(int button, int state, int x, int y) {
    GLfloat locationX, locationY;
    cout << "mouse: maxBoundingBoxLenght:" << maxBoundingBoxLenght << endl;
    //GLfloat scaledlocationRatio =(maxBoundingBoxLenght) / (window_w / 2); 
    //GLfloat scaledlocationRatio = maxBoundingBoxLenght* scaledRatioSet[0] / (window_w / 2);
   
    
    window_h = glutGet(GLUT_WINDOW_HEIGHT);
    window_w = glutGet(GLUT_WINDOW_WIDTH);
  
    if (window_w > window_h)
        //(Ortho)left length/ (window width/2)
        scaledlocationRatio = (window_h / window_w) /( window_w / 2.0);
    else
        scaledlocationRatio = 1/( window_w / 2.0);

    std::cout << "#Mouse-scaledlocationRatio:" << scaledlocationRatio << std::endl;
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {

            
            std::cout << "Mouse left button down" << std::endl;
            std::cout << "x:" << x << " , y : " << y << std::endl;
            std::cout << "window_h:" << window_h << " , window_w : " << window_w << std::endl;
            click = !click; //control dot1 or dot2
    
         
            if (x >= window_w / 2) {
                locationX = (x - window_w / 2) * scaledlocationRatio;
            }
            else {
                locationX = (window_w / 2 - x) * scaledlocationRatio * -1;
            }

            if (y < window_h / 2) {
                locationY = (y - window_h / 2) * scaledlocationRatio * -1;
            }
            else {
                locationY = (window_h / 2 - y) * scaledlocationRatio;
            }
            std::cout << "OpenGL x:" << locationX << " , OpenGL y : " << locationY << std::endl;
            if (click) {
                dot1Y = locationY;
                dot1X = locationX;
            }
            else {
                dot2Y = locationY;
                dot2X = locationX;
            }


        }
        break;
    case GLUT_MIDDLE_BUTTON:
        if (state == GLUT_DOWN) {
            std::cout << "Mouse middle button down" << std::endl;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN) {
            std::cout << "Mouse right button down" << std::endl;
        }
        break;
    defalut:
        break;
    }
    glutPostRedisplay();

}

void myKeyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'r':
        //reset translation &rotation
        thetaX = 0.f;
        thetaY = 0.f;
        thetaZ = 0.f;
        angle = 0.f;
        tx = 0.f;
        ty = 0.f;
        tz = 0.f;
        dot1X = 0;
        dot1Y = 0;
        dot2X = 0;
        dot2Y = 0;
        std::cout << "r";
        cx = 0.f;
        cy = 0.f;
        cz = 10.f;
        look_x = 0.f;
        look_y = 0.f;
        look_z = 0.f;

        break;
    case 'a':
        //change the rotation angle thetaX along x-axis
        thetaY += 3;
        std::cout << "a";
        break;
    case 'd':
        //change the rotation angle thetaX along x-axis
        thetaY -= 3;
        std::cout << "d";
        break;
    case 'w':
        //change the rotation angle thetaY along y-axis

        thetaX += 3;
        std::cout << "w";
        break;
    case 's':
        //change the rotation angle thetaY along y-axis

        thetaX -= 3;
        std::cout << "s";
        break;
    case 'z':
        //change the rotation angle thetaZ along z-axis
        thetaZ -= 3;
        std::cout << "z";
        break;
    case 'x':
        //change the rotation angle thetaZ along z-axis
        thetaZ += 3;
        std::cout << "x";
        break;

    case 'p':
        //Rotate your object along the arbitrary axis
        angle += 3;
        std::cout << "p";
        break;
    case 'o':
        //Rotate your object along the arbitrary axis
        angle -= 3;
        std::cout << "o";
        break;

        //camera control 
    case 'A':
        //change the camera location cx along x-axis
        cx += 1;
        std::cout << "A" << " - lookat eye: " << cx << ", " << cy << ", " << cz << endl;
        break;
    case 'D':
        //change the camera location cx along x-axis
        cx -= 1;
        std::cout << "D" << " - lookat eye: " << cx << ", " << cy << ", " << cz << endl;
        break;
    case 'W':
        //change the camera location cy along y-axis
        cy -= 1;
        std::cout << "W" << " - lookat eye: " << cx << ", " << cy << ", " << cz << endl;
        break;
    case 'S':
        //change the camera location cy along y-axis
        cy += 1;
        std::cout << "S" << " - lookat eye: " << cx << ", " << cy << ", " << cz << endl;
        break;

    case 'Z':
        //change the  camera location zx along z-axis
        cz -= 3;
        std::cout << "Z" << " - lookat eye: " << cx << ", " << cy << ", " << cz << endl;
        break;
    case 'X':
        //change the  camera location cz along z-axis
        cz += 3;
        std::cout << "X" << " - lookat eye: " << cx << ", " << cy << ", " << cz << endl;
        break;


        //camera look at control 
    case 'J':
        //change the camera location look at look_x along x-axis
        look_x += 0.1;
        std::cout << "J" << " - lookat center: " << look_x << ", " << look_y << ", " << look_z << endl;
        break;
    case 'L':
        //change the camera location look at look_x along x-axis
        look_x -= 0.1;
        std::cout << "L" << " - lookat center: " << look_x << ", " << look_y << ", " << look_z << endl;
        break;
    case 'I':
        //change the camera location look at look_y along y-axis
        look_y -= 0.1;
        std::cout << "I" << " - lookat center: " << look_x << ", " << look_y << ", " << look_z << endl;
        break;
    case 'K':
        //change the camera location look at look_y along y-axis
        look_y += 0.1;
        std::cout << "K" << " - lookat center: " << look_x << ", " << look_y << ", " << look_z << endl;
        break;

    case 'N':
        //change the camera location look at look_z   look_z along z-axis
        look_z -= 0.1;
        std::cout << "N" << " - lookat center: " << look_x << ", " << look_y << ", " << look_z << endl;
        break;
    case 'M':
        //change the camera location look at  look_z along z-axis
        look_z += 0.1;
    
        std::cout << "M" << " - lookat center: " << look_x << ", " << look_y << ", " << look_z << endl;
        break;

    default:
        break;
    }
    glutPostRedisplay();
}

void mySpecialKey(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_LEFT:
        //change the translation along x-axis
        tx -= 0.01f;
        std::cout << "←";
        break;
    case GLUT_KEY_RIGHT:
        tx += 0.01f;
        std::cout << "→";
        break;
    case GLUT_KEY_UP:
        ty += 0.01f;
        std::cout << "↑";
        break;
    case GLUT_KEY_DOWN:
        ty -= 0.01f;
        std::cout << "↓";
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void mainMenu(int index)
{
    switch (index)
    {
    case 1:
        break;
    default:
        break;
    }
}

//定義按鈕功能
void fileMenu1(int index)
{
    switch (index)
    {
    case 1:
       // scaledRatio = scaledRatioSet[0];
        obj = ObjLoader("teapot.obj");
       // currentfile = 1;
        break;
    case 2:
        //scaledRatio = scaledRatioSet[1];
        obj = ObjLoader("teddy.obj");
       // currentfile = 2;
        break;
    case 3:
        //scaledRatio = scaledRatioSet[2];
        obj = ObjLoader("gourd.obj");
       // currentfile = 3;
        break;
    case 4:
        //scaledRatio = scaledRatioSet[3];
        obj = ObjLoader("octahedron.obj");
        //currentfile = 4;
        break;
    }
   
    initObjModel();

    glutPostRedisplay();
}

void renderMenu2(int index)
{

    switch (index)
    {
    case 1:  //point mode
        showFace = false;
        showLine = false;
        showPoint = true;
        break;
    case 2: //line mode
        showFace = false;
        showLine = true;
        showPoint = false;
        break;
    case 3: //face mode
        showFace = true;
        showLine = false;
        showPoint = false;
        break;
    }
    glutPostRedisplay();
}

void colorMenu3(int index)
{
    switch (index)
    {
    case 1:
        glShadeModel(GL_FLAT);
        colorMode = 0;
        break;
    case 2:
        glShadeModel(GL_SMOOTH);
        colorMode = 1;
        break; 
    }
    setGenList();
    glutPostRedisplay();
}

void BuildPopupMenu()
{
    ///setting all the submenus
       //create sub-menu 1
    int filemenu1 = glutCreateMenu(fileMenu1);
    glutAddMenuEntry("teapot.obj", 1);
    glutAddMenuEntry("teddy.obj", 2);
    glutAddMenuEntry("gourd.obj", 3);
    glutAddMenuEntry("octahedron.obj", 4);
    //create sub-menu 2
    int rendermenu2 = glutCreateMenu(renderMenu2);
    glutAddMenuEntry("Point", 1);//add option of sub manu 2
    glutAddMenuEntry("Line", 2);
    glutAddMenuEntry("Face", 3);

    //create sub-menu 3
    int colormenu3 = glutCreateMenu(colorMenu3);
    glutAddMenuEntry("single color", 1);//add option of sub manu 3
    glutAddMenuEntry("random colors", 2);


    int menu = glutCreateMenu(mainMenu);
    glutAddSubMenu("obj files", filemenu1);//add submenu to main menu
    glutAddSubMenu("Render mode", rendermenu2);
    glutAddSubMenu("Color mode", colormenu3);


    glutAttachMenu(GLUT_RIGHT_BUTTON);
}