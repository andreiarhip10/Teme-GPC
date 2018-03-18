#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "glut.h"

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

// concoida lui Nicomede (concoida dreptei)
// $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. sau
// $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. unde
// $t \in (-\pi / 2, \pi / 2)$
void Display1() {
	double xmax, ymax, xmin, ymin;
	double a = 1, b = 2;
	double pi = 4 * atan(1.0);
	double ratia = 0.05;
	double t;

	// calculul valorilor maxime/minime ptr. x si y
	// aceste valori vor fi folosite ulterior la scalare
	xmax = a - b - 1;
	xmin = a + b + 1;
	ymax = ymin = 0;
	for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = a + b * cos(t);
		xmax = (xmax < x1) ? x1 : xmax;
		xmin = (xmin > x1) ? x1 : xmin;

		x2 = a - b * cos(t);
		xmax = (xmax < x2) ? x2 : xmax;
		xmin = (xmin > x2) ? x2 : xmin;

		y1 = a * tan(t) + b * sin(t);
		ymax = (ymax < y1) ? y1 : ymax;
		ymin = (ymin > y1) ? y1 : ymin;

		y2 = a * tan(t) - b * sin(t);
		ymax = (ymax < y2) ? y2 : ymax;
		ymin = (ymin > y2) ? y2 : ymin;
	}

	xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
	ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = (a + b * cos(t)) / xmax;
		x2 = (a - b * cos(t)) / xmax;
		y1 = (a * tan(t) + b * sin(t)) / ymax;
		y2 = (a * tan(t) - b * sin(t)) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = (a + b * cos(t)) / xmax;
		x2 = (a - b * cos(t)) / xmax;
		y1 = (a * tan(t) + b * sin(t)) / ymax;
		y2 = (a * tan(t) - b * sin(t)) / ymax;

		glVertex2f(x2, y2);
	}
	glEnd();
}

// graficul functiei 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2() {
	double pi = 4 * atan(1.0);
	double xmax = 8 * pi;
	double ymax = exp(1.1);
	double ratia = 0.05;

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double x = 0; x < xmax; x += ratia) {
		double x1, y1;
		x1 = x / xmax;
		y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();
}


//graficul functiei f(x) = {1, x = 0 si d(x)/x, 0 < x <= 100}
void Display3() {
	double xmax, xmin;
	double x, y, x1;
	double ratia = 0.05;
	double ymax, ymin;

	// calculul valorilor maxime/minime ptr. x si y
	// aceste valori vor fi folosite ulterior la scalare
	xmax = 25;	//	daca xmax = 100, liniile vor fi desenate prea apropiate
	xmin = 0;
	ymax = 1, ymin = 0;
	for (x1 = xmin; x1 < xmax; x1 += ratia) {
		y = fabs(x1 - round(x1)) / x1;
		if (x1 == 0) y = 1;
		ymax = (ymax < y) ? y : ymax;
		ymin = (ymin > y) ? y : ymin;
	}
	ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (x1 = xmin; x1 < xmax; x1 += ratia) {
		x = x1 / xmax;
		y = fabs(x1 - round(x1)) / x1 / ymax;
		if (x1 == 0) y = 1;
		glVertex2f(x, y);
	}
	glEnd();
}


//melcul lui Pascal
//x = 2 * (a * cos(t) + b) * cos (t)
//y = 2 * (a * cos(t) + b) * sin (t)
void Display4() {
	double a = 0.3, b = 0.2;
	double pi = 4 * atan(1.0);
	double ratia = 0.00001;
	double t;
	double x, y;

	// afisarea punctelor propriu-zise
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = -pi + ratia; t < pi; t += ratia) {
		x = 2 * (a* cos(t) + b)*cos(t);
		y = 2 * (a* cos(t) + b)*sin(t);
		glVertex2f(x - 0.1, y);
	}
	glEnd();


}


//trisectoarea lui Longchamps
void Display5() {
	double a = 0.2;
	double pi = 4 * atan(1.0);
	double ratia = 0.007;
	double t;
	double x, y;

	// afisarea punctelor propriu-zise 
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = -pi / 2; t <  - pi / 6; t += ratia) {
		x = a / (4 * cos(t)*cos(t) - 3);
		y = (a *tan(t)) / (4 * cos(t)*cos(t) - 3);
		glVertex2f(x, y);
	}
	glEnd();
	glBegin(GL_TRIANGLES);
	for (t = -pi / 6; t > -pi / 2; t -= ratia) {
		if (t < -pi / 6 && (t > -pi/4.5 || t < -pi/2.5) )
		{
			x = a / (4 * cos(t)*cos(t) - 3);
			y = (a *tan(t)) / (4 * cos(t)*cos(t) - 3);
			glVertex2f(x, y);
			glVertex2f(-1, 1);
		}
	}
	glEnd();
}


//cicloida
void Display6() {
	double a = 0.1, b = 0.2;
	double pi = 4 * atan(1.0);
	double ratia = 0.05;
	double t;
	double x, y;

	// afisarea punctelor propriu-zise 
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = -3 * pi + ratia; t < 3 * pi; t += ratia) {
		x = a * t - b * sin(t);
		y = a - b * cos(t);
		glVertex2f(x, y);
	}
	glEnd();

}


//epicicloida
void Display7() {
	double pi = 4 * atan(1.0);
	double ratia = 0.05;
	double t;
	double R = 0.1;
	double r = 0.3;
	double x, y;

	// afisarea punctelor propriu-zise
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = 0; t <= 2 * pi; t += ratia) {
		x = (R + r) * cos(r / R * t) - r * cos(t + r / R * t);
		y = (R + r) * sin(r / R * t) - r * sin(t + r / R * t);;
		glVertex2f(x, y);
	}
	glEnd();
}


//hipocicloida
void Display8() {
	double pi = 4 * atan(1.0);
	double ratia = 0.05;
	double t;
	double R = 0.1;
	double r = 0.3;
	double x, y;

	// afisarea punctelor propriu-zise 
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = 0; t <= 2 * pi; t += ratia) {
		x = (R - r) * cos(r / R * t) - r * cos(t - r / R * t);
		y = (R - r) * sin(r / R * t) - r * sin(t - r / R * t);
		glVertex2f(x, y);
	}
	glEnd();
}


//lemniscata lui Bernoulli
void Display9() {
	double a = 0.4;
	double pi = 4 * atan(1.0);
	double ratia = 0.0001;
	double t;
	double r;
	double x, y;

	// afisarea punctelor propriu-zise 
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = -pi / 4 + ratia; t < pi / 4; t += ratia) {
		r = a * sqrt(2 * cos(2 * t));
		x = r * cos(t);
		y = r * sin(t);
		glVertex2f(x, y);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (t = -pi / 4 + ratia; t < pi / 4; t += ratia) {
		r = -a * sqrt(2 * cos(2 * t));
		x = r * cos(t);
		y = r * sin(t);
		glVertex2f(x, y);
	}
	glEnd();
}


//spirala logaritmica
void Display0() {
	double a = 0.02;
	double pi = 4 * atan(1.0);
	double ratia = 0.05;
	double t;
	double r;
	double x, y;

	// afisarea punctelor propriu-zise
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = 0 + ratia; t < 5 * pi; t += ratia) {
		r = a * exp(1 + t);
		x = r * cos(t);
		y = r * sin(t);
		glVertex2f(x, y);
	}
	glEnd();

}



void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);

	//   glPointSize(4);

	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey) {
	case '1':
		Display1();
		break;
	case '2':
		Display2();
		break;
	case '3':
		Display3();
		break;
	case '4':
		Display4();
		break;
	case '5':
		Display5();
		break;
	case '6':
		Display6();
		break;
	case '7':
		Display7();
		break;
	case '8':
		Display8();
		break;
	case '9':
		Display9();
		break;
	case '0':
		Display0();
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