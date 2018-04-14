#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265f

float x[] = {-600, -600, -100}, y[] = {-250, -150, -350}, X[3], Y[3];
float a[] = {600, 600, 100}, b[] = {-250, -150, -350}, A[3], B[3];

float ballX = 700, ballY = -300;
float xMin = -750, xMax = 750, yMin = -375, yMax = 375;
int rotationFlag = 0, ballFlag = 0, rotCountUp = 0, rotCountDown = 0, bounceCount = 0;
float xSpeed = 7, ySpeed = 4;
float angle = 0, theta1, theta2;
bool atMax = false;
int score = 0;

void launchBall(int);

void drawPoint(float x, float y){
    glPointSize(1.0);
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

void midPointCircleAlgo(float r, float X, float Y){
	float x = 0;
	float y = r;
	float decision = 5/4 - r;
	drawPoint(x+X, y+Y);

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
		drawPoint(x+X, y+Y);
		drawPoint(x+X, Y-y);
		drawPoint(X-x, y+Y);
		drawPoint(X-x, Y-y);
		drawPoint(y+X, x+Y);
		drawPoint(X-y, x+Y);
		drawPoint(y+X, Y-x);
		drawPoint(X-y, Y-x);
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


void resetFlippers(){
    rotationFlag = 0;
    atMax = false;
    angle = 0;
    rotCountUp = 0;
    rotCountDown = 0;
}

void resetBall(){
    ballFlag = 0;
    bounceCount = 0;
    ballX = 700;
    ballY = -300;
}

void bounceBall(){
    midPointCircleAlgo(30, ballX, ballY);
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
    }

    if(ballX + 30 > xMax){
        ballX = xMax - 30;
        xSpeed = -xSpeed;
    }
    else if(ballX - 30 < xMin){
        ballX = xMin + 30;
        xSpeed = -xSpeed;
    }
    if(ballY + 30 > yMax){
        ballY = yMax - 30;
        ySpeed = -ySpeed;
    }
    else if(ballY - 30 < yMin){
        ballY = yMin + 30;
        ySpeed = -ySpeed;
    }

    if(ballY - 30 < -350 && ballX > -100 && ballX < 100){
        resetBall();
        score += 100;
        printf("Score: %d", score);
    }
}

void launchBall(){
    if(ballFlag == 1){
        midPointCircleAlgo(30, ballX, ballY);
        if(ballY <= 300){
            ballY += ySpeed;
        }
    }
    else{
        midPointCircleAlgo(30, ballX, ballY);
        score = 0;
    }
}

void drawFlippers(){
    lineDDA(x[0], y[0], x[1], y[1]);
    lineDDA(x[1], y[1], x[2], y[2]);
    lineDDA(x[2], y[2], x[0], y[0]);

    lineDDA(a[0], b[0], a[1], b[1]);
    lineDDA(a[1], b[1], a[2], b[2]);
    lineDDA(a[2], b[2], a[0], b[0]);
}

void flipperRotation(){
    if(rotationFlag == 1){
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

        if(bounceCount){
            if((ballX - 30 <= X[2] && ballX - 30 >= X[0]) || (ballX + 30 >= A[2] && ballX + 30 <= A[0])){
                if((ballY - 30 <= Y[2] && ballY - 30 >= Y[0]) || (ballY - 30 <= B[2] && ballY - 30 >= B[0])){
                    xSpeed = -xSpeed;
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
        if(bounceCount){
            if((ballX - 30 <= x[2] && ballX - 30 >= x[0]) || (ballX + 30 >= a[2] && ballX + 30 <= a[0])){
                if((ballY - 30 <= y[2] && ballY - 30 >= y[0]) || (ballY - 30 <= b[2] && ballY - 30 >= b[0])){
                    xSpeed = -xSpeed;
                    ySpeed = -ySpeed;
                }
            }
        }
    }
}
