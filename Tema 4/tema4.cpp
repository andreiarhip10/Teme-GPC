#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "glut.h"

// dimensiunea ferestrei in pixeli
#define dim 300
unsigned char prevKey;

class GrilaCarteziana {

public:
	GrilaCarteziana() : lin(0),col(0) {}
	GrilaCarteziana(float lin1,float col1) : lin(lin1),col(col1) {}
	~GrilaCarteziana() {}

	double getLin() { return lin; }
	void setLin(float lin1) { lin = lin1; }

	double getCol() { return col; }
	void setCol(float col1) { col = col1; }

	void deseneazaGrila(float lin,float col) {
		
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

private:
	float lin,col;
};



void Display1(){
	GrilaCarteziana grila;
	grila.deseneazaGrila(20,20);
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


