#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <iostream>

#include "glut.h"

using namespace std;

// dimensiunea ferestrei in pixeli
#define dim 300
unsigned char prevKey;

double PI = 4 * atan(1.0);

class GrilaCarteziana {

public:
	GrilaCarteziana() : lin(0),col(0) {}
	GrilaCarteziana(float lin1,float col1) : lin(lin1),col(col1) {}
	~GrilaCarteziana() {}

	double getLin() { return lin; }
	void setLin(float lin1) { lin = lin1; }

	double getCol() { return col; }
	void setCol(float col1) { col = col1; }

	void deseneazaGrila() {
		
		//pt scalare
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		double w = glutGet(GLUT_WINDOW_WIDTH);
		double h = glutGet(GLUT_WINDOW_HEIGHT);
		double ar = w / h;
		glOrtho(-1 * ar, 1 * ar, -1, 1, -1, 1);		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		glColor3f(0.0, 0.0, 0.0);

		
		//desenez linii
		float y = 0.8;
		for (int i = 0; i <= lin; i++) {
				glBegin(GL_LINES);
					glVertex2f(-0.8, y);
					glVertex2f(0.8, y);
				glEnd();
				y -= 1.6/lin;
		}

		//desenez coloane
		
		float x = -0.8;
		for (int i = 0; i <= col; i++) {
			glBegin(GL_LINES);
				glVertex2f(x, 0.8);
				glVertex2f(x,-0.8);
			glEnd();
			x += 1.6/col;
		}


	}

	void writePixel(int line, int column) {
		glColor3f(0.5, 0.5, 0.5);
		float x = -0.8 + column * (1.6/col);
		float y = 0.8 - line * (1.6/lin);
		float radius = 0.03;
		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y);
		for (int i = 0; i <= 100; i++)
		{
			glVertex2f((x + (radius * cos(i * 2 * PI / 100))), (y + (radius * sin(i * 2 * PI / 100))));
		}
		glEnd();
	}

	void drawSegment(int begin_line, int begin_column, int end_line, int end_column) {
		float beginx = -0.8 + begin_column * (1.6 / col);
		float beginy = 0.8 - begin_line * (1.6 / lin);
		float endx = -0.8 + end_column * (1.6 / col);
		float endy = 0.8 - end_line * (1.6 / lin);
		glColor3f(1.0, 0.1, 0.1);
		glBegin(GL_LINE_STRIP);
			glVertex2f(beginx, beginy);
			glVertex2f(endx, endy);
		glEnd();
		writePixel(begin_line, begin_column);
		writePixel(end_line, end_column);
		float cosine = (endx - beginx) / (sqrt(pow(endx - beginx, 2) + pow(endy - beginy, 2)));
		cout << cosine;
		// if 0.707 - 45 degrees angle, if > - iterate through columns, if < - iterate through lines

	}

private:
	float lin,col;
};



void Display1(){
	GrilaCarteziana grila =	GrilaCarteziana(15, 15);
	grila.deseneazaGrila();
	//grila.writePixel(4, 14);
	grila.drawSegment(15, 0, 0, 10);
};

void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);

	glPointSize(3);

	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
	switch (prevKey) {
	case '1':
		glClear(GL_COLOR_BUFFER_BIT);
		Display1();
		break;
	default:
		break;
	}

	glFlush();
}

void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitWindowSize(dim, dim);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}


