#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

// Macros
#define PI 3.14159625f

// Global variables

GLfloat radius = 0.05f;
GLfloat ballX = 1.8f;
GLfloat ballY = -0.8f;
GLfloat ballXMax, ballXMin, ballYMax, ballYMin;
GLfloat xSpeed = 0.0005f;
GLfloat ySpeed = 0.0007f;
GLdouble clipAreaLeft, clipAreaRight, clipAreaBottom, clipAreaTop;
GLfloat alpha;
int refreshMillis = 25;
bool finished = false;

// OpenGL functions
void initOpenGL();
void reshape(GLsizei, GLsizei);
void keyPressed(unsigned char, int, int);
void renderScene();

// Helper functions
void timer(int);
void launchBall();
void bounceBall();
bool isFinished(GLfloat, GLfloat);

int main(int argc, char **argv){
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1500, 750);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Pinball");
    initOpenGL();

    glutDisplayFunc(renderScene);
    glutTimerFunc(0, timer, 0);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyPressed);
    glutMainLoop();

    return 0;
}

// Function definitions

void initOpenGL(){
    glClearColor(0.0, 0.0, 0.0, 1.0);
    gluOrtho2D(-750.0, 750.0, -375.0, 375.0);
    glColor4f(0.0f, 0.0f, 1.0f, alpha);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void reshape(GLsizei width, GLsizei height){
    if(height == 0){
        height = 1;
    }
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(width >= height){
        clipAreaLeft = -1.0 * aspect;
        clipAreaRight = 1.0 * aspect;
        clipAreaBottom = -1.0;
        clipAreaTop = 1.0;
    }
    else{
        clipAreaLeft = -1.0;
        clipAreaRight = 1.0;
        clipAreaBottom = -1.0 * aspect;
        clipAreaTop = 1.0 * aspect;
    }
    gluOrtho2D(clipAreaLeft, clipAreaRight, clipAreaBottom, clipAreaTop);
    ballXMin = clipAreaLeft + radius;
    ballXMax = clipAreaRight - radius;
    ballYMin = clipAreaBottom + radius;
    ballYMax = clipAreaTop - radius;
}

void keyPressed(unsigned char c, int x, int y){
    if(c == 32){
        launchBall();
        bounceBall();
    }
}

void DDA(int x0,int y0,int x1,int y1){
    int dx=x1-x0;
    int dy=y1-y0;
    int steps=abs(dx)>abs(dy)?abs(dx):abs(dy);

    float XInc=dx/float(steps);
    float YInc=dy/float(steps);
    float x=x0;
    float y=y0;
    glBegin(GL_POINTS);
    for(int i=0;i<steps;i++){
        glVertex2f(x,y);
        x+=XInc;
        y+=YInc;
    }
    glEnd();
}

void draw_pixel(int x,int y)
{
    glColor3f(0.5,0.4,0.7);
    glPointSize(1.0);
    glBegin(GL_POINTS);
    glVertex2i(x,y);
    glEnd();
}

void edgedetect(float x1,float y1,float x2,float y2,int *le,int *re)
{
    float temp,x,mx;
    int i;

    if(y1>y2)
    {
        temp=x1,x1=x2,x2=temp;
        temp=y1,y1=y2,y2=temp;
    }

    if(y1==y2)
        mx=x2-x1;
    else
        mx=(x2-x1)/(y2-y1);

    x=x1;

    for(i=int(y1);i<=(int)y2;i++)
    {
        if(x<(float)le[i]) le[i]=(int)x;
        if(x>(float)re[i]) re[i]=(int)x;
        x+=mx;
    }
}

void scanfill(float x1,float y1,float x2,float y2,float x3,float y3)
{
    int le[700],re[700],i,j;

    for(i=0;i<700;i++)
        le[i]=700,re[i]=0;

    edgedetect(x1,y1,x2,y2,le,re);
    edgedetect(x2,y2,x3,y3,le,re);
    edgedetect(x3,y3,x1,y1,le,re);




    for(j=0;j<700;j++)
    {
        if(le[j]<=re[j])
            for(i=le[j];i<re[j];i++)
                draw_pixel(i,j);
    }
}

void renderScene(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO: Draw all the colliding objects
    glColor3b(1.0,1.0,0);
    DDA(-200,-200,-450,-150);
    DDA(-450,-150,-400,0);
    DDA(-400,0,-200,-200);
    scanfill(-450,-150,-400,0,-200,-200);

    DDA(200,-200,450,-150);
    DDA(450,-150,400,0);
    DDA(400,0,200,-200);

    scanfill(200,-200,400,0,450,-150);
}

void timer(int value){
    glutPostRedisplay();
    glutTimerFunc(refreshMillis, timer, 0);
}

void launchBall(){
    alpha = 1.0f;
    ballX = 1.8f;
    ballY = -0.8f;
    xSpeed = fabs(xSpeed);
    ySpeed = fabs(ySpeed);
    do{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPushMatrix();
        glTranslatef(ballX, ballY, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
            glColor4f(0.0f, 0.0f, 1.0f, alpha);
            glVertex2f(0.0f, 0.0f);
            int segments = 100;
            GLfloat angle;
            for(int i = 0;i <= segments;i++){
                angle = i * 2.0f * PI / segments;
                glVertex2f(cos(angle) * radius, sin(angle) * radius);
            }
        glEnd();
        glPopMatrix();

        glutSwapBuffers();

        ballY += ySpeed;
    }while(ballY < 0.8f);
}

void bounceBall(){
    int quadrant = (rand() % 4) + 1;
    do{
        finished = isFinished(ballX, ballY);
        if(finished){
            alpha = 0.0f;
            ballX = 1.77f;
            ballY = 0.77f;
        }
        else{
            alpha = 1.0f;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPushMatrix();
        glTranslatef(ballX, ballY, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
            glColor4f(0.0f, 0.0f, 1.0f, alpha);
            glVertex2f(0.0f, 0.0f);
            int segments = 100;
            GLfloat angle;
            for(int i = 0;i <= segments;i++){
                angle = i * 2.0f * PI / segments;
                glVertex2f(cos(angle) * radius, sin(angle) * radius);
            }
        glEnd();
        glPopMatrix();

        glutSwapBuffers();

        switch(quadrant){
            case 1:
                ballX += xSpeed;
                ballY += ySpeed;
            break;
            case 2:
                ballX -= xSpeed;
                ballY += ySpeed;
            break;
            case 3:
                ballX -= xSpeed;
                ballY -= ySpeed;
            break;
            case 4:
                ballX += xSpeed;
                ballY -= ySpeed;
            break;
            default:
                printf("rand() error");
        }

        if(ballX > ballXMax){
            ballX = ballXMax;
            xSpeed = -xSpeed;
        }
        else if(ballX < ballXMin){
            ballX = ballXMin;
            xSpeed = -xSpeed;
        }
        if(ballY > ballYMax){
            ballY = ballYMax;
            ySpeed = -ySpeed;
        }
        else if(ballY < ballYMin){
            ballY = ballYMin;
            ySpeed = -ySpeed;
        }
    }while(!finished);
}

bool isFinished(GLfloat x, GLfloat y){
    if(x >= ballXMin / 3 && x <= ballXMax / 3 && y == ballYMin){
        return true;
    }
    return false;
}
