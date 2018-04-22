#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265f

float x[] = {-600, -600, -100}, y[] = {-250, -150, -350}, X[3], Y[3];
float a[] = {600, 600, 100}, b[] = {-250, -150, -350}, A[3], B[3];

float ballX = 700, ballY = -300;
float xMin = -750, xMax = 750, yMin = -375, yMax = 375;
int rotationFlag = 0, ballFlag = 0, rotCountUp = 0, rotCountDown = 0,
    bounceCount = 0;
float xSpeed = 7, ySpeed = 4;
float angle = 0, theta1, theta2;
bool atMax = false;
int score = 0;

// Algorithms for drawing primitives and filling
void drawPoint();
void midPointCircleFill(float, float, float);
void lineDDA(float, float, float, float);
void edge_detect(float, float, float, float, int*, int*);
void scanFillTriangle(float, float, float, float, float, float);
void scanFill(float, float, float, float, float, float, float, float);

// Ball functions
void resetBall();
void launchBall();
void bounceBall();

//Flipper functions
void resetFlippers();
void drawFlippers();
void flipperRotation();

// For delayed objects
void drawBoundaries();

/* <----Function definitions---->*/
void drawPoint(float x, float y){
    glPointSize(1.0);
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

void midPointCircleFill(float r, float X, float Y){
    glColor3f(0.621, 0.628, 0.636);
	float x = 0;
	float y = r;
	float decision = 5/4 - r;

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
        for(float i = -x;i <= x;i++){
            for(float j = -y;j <=y;j++){
                drawPoint(i+X, j+Y);
                drawPoint(j+X, i+Y);
            }
		}
	}
}

void lineDDA(float x0, float y0, float xEnd, float yEnd){
    float dx = xEnd - x0;
    float dy = yEnd - y0;
    float steps, xIncrement, yIncrement, x = x0, y = y0;

    if(fabs(dx) > fabs(dy)){
        steps = fabs(dx);
    }
    else{
        steps = fabs(dy);
    }
    xIncrement = GLfloat (dx) / GLfloat (steps);
    yIncrement = GLfloat (dy) / GLfloat (steps);
    drawPoint(round (x), round(y));

    for(int k = 0; k < steps; k++){
        x += xIncrement;
        y += yIncrement;
        drawPoint(round(x), round(y));
    }
}

void edge_detect(float x1a, float y1a, float x2a, float y2a, int *le, int *re){
    float temp, x, mx;

    if(y1a > y2a){
        temp = x1a, x1a = x2a, x2a = temp;
        temp = y1a, y1a = y2a, y2a = temp;
    }

    if(y1a == y2a)
        mx = x2a - x1a;
    else
        mx = (x2a - x1a) / (y2a - y1a);

    x = x1a;

    for(int i = int(y1a) + abs(yMin);i <= (int)y2a + abs(yMin);i++){
        if(x < (float)le[i])
            le[i] = (int)x;
        if(x > (float)re[i])
            re[i] = (int)x;
        x += mx;
    }
}

void scanFillTriangle(float x1b, float y1b, float x2b, float y2b, float x3b,
                      float y3b){
    int le[abs(yMin) + abs(yMax) + 1], re[abs(yMin) + abs(yMax) + 1];

    for(int i = 0;i < yMax - yMin + 1;i++)
        le[i] = xMax;
    for(int j=0;j < yMax - yMin + 1;j++)
        re[j] = xMin;

    edge_detect(x1b, y1b, x2b, y2b, le, re);
    edge_detect(x2b, y2b, x3b, y3b, le, re);
    edge_detect(x3b, y3b, x1b, y1b, le, re);

    for(int j = 0;j < yMax - yMin + 1;j++){
        if(le[j] <= re[j])
            for(int i = le[j];i < re[j];i++)
                drawPoint(i, j - abs(yMin));
    }
}

void scanFill(float x1b, float y1b, float x2b, float y2b, float x3b, float y3b,
                float x4b, float y4b){
    int le[abs(yMin) + abs(yMax) + 1], re[abs(yMin) + abs(yMax) + 1];

    for(int i = 0;i < yMax - yMin + 1;i++)
        le[i] = xMax;
    for(int j=0;j < yMax - yMin + 1;j++)
        re[j] = xMin;

    edge_detect(x1b, y1b, x2b, y2b, le, re);
    edge_detect(x2b, y2b, x3b, y3b, le, re);
    edge_detect(x3b, y3b, x4b, y4b, le, re);
    edge_detect(x4b, y4b, x1b, y1b, le, re);

    for(int j = 0;j < yMax - yMin + 1;j++){
        if(le[j] <= re[j])
            for(int i = le[j];i < re[j];i++)
                drawPoint(i, j - abs(yMin));
    }
}

void resetBall(){
    ballFlag = 0;
    bounceCount = 0;
    ballX = 700;
    ballY = -300;
    xSpeed = fabs(xSpeed);
    ySpeed = fabs(ySpeed);
}

void launchBall(){
    if(ballFlag == 1){
        midPointCircleFill(30, ballX, ballY);
        if(ballY <= 300){
            ballY += ySpeed;
        }
    }
    else{
        midPointCircleFill(30, ballX, ballY);
        score = 0;
    }
}

void bounceBall(){
    midPointCircleFill(30, ballX, ballY);
    if(bounceCount == 0){
        if(ballX == 700 && ballY >= 300){
            ballX -= xSpeed;
            ballY -= ySpeed;
            ++bounceCount;
        }
    }
    else{
        ballX += xSpeed;
        ballY += ySpeed;
        ++bounceCount;
    }

    // Boundary collision
    if(ballX + 30 > xMax){
        ballX = xMax - 30;
        xSpeed = -xSpeed;
        ++bounceCount;
    }
    else if(ballX - 30 < xMin){
        ballX = xMin + 30;
        xSpeed = -xSpeed;
        ++bounceCount;
    }
    if(ballY + 30 > yMax){
        ballY = yMax - 30;
        ySpeed = -ySpeed;
        ++bounceCount;
    }
    else if(ballY - 30 < yMin){
        ballY = yMin + 30;
        ySpeed = -ySpeed;
        ++bounceCount;
    }

    // For blocks beside the flippers
    if(bounceCount){
        if(ballX - 30 <= -600 && ballY - 30 <= -150){
            ySpeed = -ySpeed;
            ++bounceCount;
        }
        if(ballX + 30 >= 600 && ballY - 30 <= -150){
            ySpeed = -ySpeed;
            ++bounceCount;
        }
    }

    // For bumpers in top corners
    if(bounceCount >= 30){
        if(ballX - 30 <= -610 && ballX - 30 >= xMin){
            if(ballY + 30 >= 235 && ballY <= yMax){
                ySpeed = -ySpeed;
                score += 25;
            }
        }
        if(ballX + 30 <= xMax && ballX + 30 >= 610){
            if(ballY + 30 >= 235 && ballY <= yMax){
                ySpeed = -ySpeed;
                score += 25;
            }
        }
    }

    // Game over
    if((ballY - 30 < -350 && ballX - 30 > X[2] && ballX + 30 < A[2]) ||
       (ballY - 30 < -350)){
        resetBall();
        score += 100;
        printf("Score: %d\n", score);
    }
}

void resetFlippers(){
    rotationFlag = 0;
    atMax = false;
    angle = 0;
    rotCountUp = 0;
    rotCountDown = 0;
}

void drawFlippers(){
    glColor3f(0.578, 0, 0.824);

    // Left flipper
    lineDDA(x[0], y[0], x[1], y[1]);
    lineDDA(x[1], y[1], x[2], y[2]);
    lineDDA(x[2], y[2], x[0], y[0]);

    // Right flipper
    lineDDA(a[0], b[0], a[1], b[1]);
    lineDDA(a[1], b[1], a[2], b[2]);
    lineDDA(a[2], b[2], a[0], b[0]);

    scanFillTriangle(x[0], y[0], x[1], y[1], x[2], y[2]);
    scanFillTriangle(a[0], b[0], a[1], b[1], a[2], b[2]);
}

void flipperRotation(){
    if(rotationFlag == 1){
        glColor3f(0.578, 0, 0.824);
        theta1 = angle * PI / 180, theta2 = (-angle) * PI / 180;
        for(int i = 0;i < 3;i++){
            x[i] += 600;
            y[i] += 200;
            X[i] = (x[i] * cos(theta1)) - (y[i] * sin(theta1));
            Y[i] = (x[i] * sin(theta1)) + (y[i] * cos(theta1));
            x[i] -= 600;
            y[i] -= 200;
            X[i] -= 600;
            Y[i] -= 200;

            a[i] -= 600;
            b[i] += 200;
            A[i] = (a[i] * cos(theta2)) - (b[i] * sin(theta2));
            B[i] = (a[i] * sin(theta2)) + (b[i] * cos(theta2));
            a[i] += 600;
            b[i] -= 200;
            A[i] += 600;
            B[i] -= 200;
        }
        lineDDA(X[0], Y[0], X[1], Y[1]);
        lineDDA(X[1], Y[1], X[2], Y[2]);
        lineDDA(X[2], Y[2], X[0], Y[0]);
        scanFillTriangle(X[0], Y[0], X[1], Y[1], X[2], Y[2]);

        lineDDA(A[0], B[0], A[1], B[1]);
        lineDDA(A[1], B[1], A[2], B[2]);
        lineDDA(A[2], B[2], A[0], B[0]);
        scanFillTriangle(A[0], B[0], A[1], B[1], A[2], B[2]);

        // Collision detection
        if(bounceCount){
            if((ballX - 30 <= X[2] && ballX - 30 >= X[0]) ||
               (ballX + 30 >= A[2] && ballX + 30 <= A[0])){
                if((ballY - 30 <= Y[2] && ballY - 30 >= Y[0]) ||
                   (ballY - 30 <= B[2] && ballY - 30 >= B[0])){
                    ySpeed = -ySpeed;
                }
            }
        }

        if(angle <= 30){
            ++rotCountUp;
            if(rotCountUp >= 40){
                atMax = true;
            }
            else{
                angle += 0.75;
            }
        }

        if(atMax){
            theta1 = angle * PI / 180, theta2 = (-angle) * PI / 180;
            for(int i = 0;i < 3;i++){
                x[i] += 600;
                y[i] += 200;
                X[i] = (x[i] * cos(theta1)) - (y[i] * sin(theta1));
                Y[i] = (x[i] * sin(theta1)) + (y[i] * cos(theta1));
                x[i] -= 600;
                y[i] -= 200;
                X[i] -= 600;
                Y[i] -= 200;

                a[i] -= 600;
                b[i] += 200;
                A[i] = (a[i] * cos(theta2)) - (b[i] * sin(theta2));
                B[i] = (a[i] * sin(theta2)) + (b[i] * cos(theta2));
                a[i] += 600;
                b[i] -= 200;
                A[i] += 600;
                B[i] -= 200;
            }
            lineDDA(X[0], Y[0], X[1], Y[1]);
            lineDDA(X[1], Y[1], X[2], Y[2]);
            lineDDA(X[2], Y[2], X[0], Y[0]);

            lineDDA(A[0], B[0], A[1], B[1]);
            lineDDA(A[1], B[1], A[2], B[2]);
            lineDDA(A[2], B[2], A[0], B[0]);

            if(angle >= 0){
                ++rotCountDown;
                if(rotCountDown >= 40){
                    resetFlippers();
                }
                else{
                    angle -= 0.75;
                }
            }
        }
    }
    else{
        drawFlippers();
        // Collision detection
        if(bounceCount){
            if((ballX - 30 <= x[2] && ballX - 30 >= x[0]) ||
               (ballX + 30 >= a[2] && ballX + 30 <= a[0])){
                if((ballY - 30 <= y[2] && ballY - 30 >= y[0]) ||
                   (ballY - 30 <= b[2] && ballY - 30 >= b[0])){
                    ySpeed = -ySpeed;
                }
            }
        }
    }
}

void drawBoundaries(){
    // For blocks beside flippers
    if(bounceCount){
        glColor3f(0.542, 0.269, 0.074);
        // Left box
        lineDDA(-600, -150, xMin, -150);
        lineDDA(-600, -250, xMin, -250);
        // Right box
        lineDDA(600, -150, xMax, -150);
        lineDDA(600, -250, xMax, -250);

        scanFill(-600, -150, xMin, -150, xMin, -250, -600, -250);
        scanFill(600, -150, xMax, -150, xMax, -250, 600, -250);
    }

    // For bumpers in top corners
    if(bounceCount >= 25){
        glColor3f(1.0, 0.0, 0.0);

        // Left bumper
        lineDDA(xMin, 235, -610, 235);
        lineDDA(-610, 235, -610, yMax);

        // Right bumper
        lineDDA(xMax, 235, 610, 235);
        lineDDA(610, 235, 610, yMax);

        scanFill(xMin, yMax, -610, yMax, xMin, 235, -610, 235);
        scanFill(xMax, yMax, 610, yMax, xMax, 235, 610, 235);
    }
}
