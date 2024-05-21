#include <iostream>
#include <vector>
#include <GL/freeglut.h>

const int windowWidth = 800;
const int windowHeight = 600;
const int numRows = 5;
const int numCols = 10;
const int brickWidth = 70;
const int brickHeight = 20;
const int paddleWidth = 100;
const int paddleHeight = 20;
const float ballRadius = 10.0f;
const float ballSpeed = 0.5f;

struct Brick {
    int x, y;
    bool destroyed;
};

std::vector<Brick> bricks;
int paddleX = windowWidth / 2 - paddleWidth / 2;
int paddleY = windowHeight - 50;
float ballX = windowWidth / 2;
float ballY = windowHeight / 2;
float ballDirX = ballSpeed;
float ballDirY = -ballSpeed;

void initBricks() {
    bricks.clear();
    for (int i = 0; i < numRows; ++i) {
        for (int j = 0; j < numCols; ++j) {
            bricks.push_back({ j * (brickWidth + 10) + 50, i * (brickHeight + 10) + 50, false });
        }
    }
}

void drawBrick(const Brick& brick) {
    if (brick.destroyed) return;
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(brick.x, brick.y);
    glVertex2f(brick.x + brickWidth, brick.y);
    glVertex2f(brick.x + brickWidth, brick.y + brickHeight);
    glVertex2f(brick.x, brick.y + brickHeight);
    glEnd();
}

void drawPaddle() {
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(paddleX, paddleY);
    glVertex2f(paddleX + paddleWidth, paddleY);
    glVertex2f(paddleX + paddleWidth, paddleY + paddleHeight);
    glVertex2f(paddleX, paddleY + paddleHeight);
    glEnd();
}

void drawBall() {
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ballX, ballY);
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(ballX + cos(angle) * ballRadius, ballY + sin(angle) * ballRadius);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (const auto& brick : bricks) {
        drawBrick(brick);
    }
    drawPaddle();
    drawBall();
    glutSwapBuffers();
}

void update(int value) {
    ballX += ballDirX;
    ballY += ballDirY;

    if (ballX - ballRadius < 0 || ballX + ballRadius > windowWidth) {
        ballDirX = -ballDirX;
    }
    if (ballY - ballRadius < 0) {
        ballDirY = -ballDirY;
    }
    if (ballY + ballRadius > windowHeight) {
        ballX = windowWidth / 2;
        ballY = windowHeight / 2;
        ballDirY = -ballSpeed;
    }

    if (ballX > paddleX && ballX < paddleX + paddleWidth && ballY + ballRadius > paddleY && ballY - ballRadius < paddleY + paddleHeight) {
        ballDirY = -ballDirY;
        ballY = paddleY - ballRadius;
    }

    for (auto& brick : bricks) {
        if (!brick.destroyed && ballX > brick.x && ballX < brick.x + brickWidth && ballY - ballRadius < brick.y + brickHeight && ballY + ballRadius > brick.y) {
            ballDirY = -ballDirY;
            brick.destroyed = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(int key, int, int) {
    if (key == GLUT_KEY_LEFT && paddleX > 0) {
        paddleX -= 20;
    }
    if (key == GLUT_KEY_RIGHT && paddleX + paddleWidth < windowWidth) {
        paddleX += 20;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Brick Breaker");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    initBricks();

    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}


