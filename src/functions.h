#include <windows.h>
#include <GL/glut.h>
#include <math.h>

//Find sign of given integer
#define sign(x) ((x > 0) ? 1 : ((x < 0) ? -1 : 0))
#define PI 3.14159625f

//Swap two given integer values
void swap(int *x, int *y){
    int temp = *x;
    *x = *y;
    *y = temp;
}

//Place pixel at given (x, y) co-ordinates
void drawPixel(GLint x, GLint y) {
    glPointSize(4.0);
    glBegin(GL_POINTS);
        glVertex2i(x,y);
    glEnd();
}

//Draw line from (x1, y1) to (x2, y2) using Bresenham's algorithm
void drawLine(int x1, int y1, int x2, int y2){
    int dx, dy, x, y, decisionParameter;
    int s1, s2, swapFlag = 0;

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    s1 = sign(x2 - x1);
    s2 = sign(y2 - y1);

    if(dy > dx){
        swap(&dx, &dy);
        swapFlag = 1;
    }

    decisionParameter = 2 * dy - dx;
    x = x1;
    y = y1;

    for(int i = 1;i <= dx;i++){
        drawPixel(x, y);
        while(decisionParameter >= 0){
            if(swapFlag){
                x += s1;
            }
            else{
                y += s2;
                decisionParameter -= 2* dx;
            }
        }
        if(swapFlag){
            y += s2;
        }
        else{
            x += s1;
        }
        decisionParameter += 2 * dy;
    }
}

void midPointCircleAlgo(GLfloat r, GLfloat X, GLfloat Y){
	float x = 0;
	float y = r;
	float decision = 5/4 - r;
	drawPixel(x + X, y + Y);

	while(y > x){
		if(decision < 0){
			x++;
			decision += 2 * x + 1;
		}
		else{
			y--;
			x++;
			decision += 2 * (x - y) + 1;
		}
		drawPixel(x + X, y + Y);
		drawPixel(x + X, Y - y);
		drawPixel(X - x, y + Y);
		drawPixel(X - x, Y - y);
		drawPixel(y + X, x + Y);
		drawPixel(X - y, x + Y);
		drawPixel(y + X, Y - x);
		drawPixel(X - y, Y - x);
	}
}
