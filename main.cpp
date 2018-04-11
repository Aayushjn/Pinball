#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include<iostream>
#include <time.h>
using namespace std;
inline GLint round (const GLfloat a) {return GLint (a+0.5);}

int counter=0;


void drawPoint(int x, int y)
{
    glPointSize(7.0);
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}
void lineDDA(GLint x0, GLint y0, GLint xEnd, GLint yEnd)
{
 GLint dx = xEnd - x0;
 GLint dy = yEnd - y0;
 GLint steps, k;
 GLfloat xIncrement, yIncrement, x=x0, y=y0;
 if(fabs(dx) > fabs(dy))
 steps = fabs(dx);
 else
 steps = fabs(dy);
 xIncrement = GLfloat (dx) / GLfloat (steps);
 yIncrement = GLfloat (dy) / GLfloat (steps);
 drawPoint(round (x), round(y));

 for(k=0; k<steps; k++)
 {
 x += xIncrement;
 y += yIncrement;
 drawPoint(round(x) , round(y));
 }


}



float x[]={-100,-100,-35},y[]={-100,-50,-150}, X[3],Y[3];
float a[]={100,100,35},b[]={-100,-50,-150}, A[3],B[3];
float angle1,theta1;
float angle2,theta2;
void rotateCustom()
{

    glClear(GL_COLOR_BUFFER_BIT);

    theta1=angle1* 3.14 / 180;
    theta2=angle2* 3.14 / 180;

    int i=0;

    for(i=0;i<3;i++){
        x[i]+=100;
        y[i]+=75;
        X[i] =  ((float)(x[i]) * cos(theta1))
             - ((float)(y[i]) * sin(theta1));
        Y[i] =  ((float)(x[i]) * sin(theta1))
             + ((float)(y[i]) * cos(theta1));
    x[i]-=100;
    y[i]-=75;
    X[i]-=100;
    Y[i]-=75;

    a[i]-=100;
      b[i]+=75;
        A[i] =  ((float)(a[i]) * cos(theta2))
             - ((float)(b[i]) * sin(theta2));
        B[i] =  ((float)(a[i]) * sin(theta2))
             + ((float)(b[i]) * cos(theta2));
    a[i]+=100;
     b[i]-=75;
    A[i]+=100;
     B[i]-=75;
    }
        lineDDA(X[0],Y[0],X[1],Y[1]);
        lineDDA(X[1],Y[1],X[2],Y[2]);
        lineDDA(X[2],Y[2],X[0],Y[0]);

        lineDDA(A[0],B[0],A[1],B[1]);
        lineDDA(A[1],B[1],A[2],B[2]);
        lineDDA(A[2],B[2],A[0],B[0]);


    glFlush();


}

void init()
{
    glClearColor(0.0,0.0,0.0,0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-500.0,500.0,-500.0,500.0);
}
void display2(int x)
{
    counter++;
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0,1.0,1.0);
    if(angle1>0 && angle2<0)
    {rotateCustom();
    angle1-=0.1;
    angle2+=0.1;}

    glFlush();
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0,1.0,1.0);

    if(angle1<=60 && counter==0 && angle2>=-60)
    {rotateCustom();
    angle1+=0.1;
    angle2-=0.1;}

    glutTimerFunc(500,display2,0);
    glFlush();


}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 97)
		{
		printf("99999");

		}
}





int main(int argc,char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(1500,750);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Rotation");
    glutDisplayFunc(display);
    glutIdleFunc(display);


    init();
    glutMainLoop();
}
