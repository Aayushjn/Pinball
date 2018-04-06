#include <windows.h>
#include <GL/glut.h>

#include "functions.h"

// Global variables

GLfloat radius = 0.05f;
GLfloat ballX = 1.77f;
GLfloat ballY = 0.77f;
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
void bounceBall();
bool isFinished(GLfloat, GLfloat);

int main(int argc, char **argv){
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
    glClearColor(1.0, 1.0, 1.0, 1.0);
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
        bounceBall();
    }
}

void renderScene(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // TODO: Draw all the colliding objects
    // TODO: Launch ball function
}

void timer(int value){
    glutPostRedisplay();
    glutTimerFunc(refreshMillis, timer, 0);
}

void bounceBall(){
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

        ballX -= xSpeed;
        ballY -= ySpeed;
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
    if(x >= ballXMin/3 && x <= ballXMax/3 && y == ballYMin){
        return true;
    }
    return false;
}
