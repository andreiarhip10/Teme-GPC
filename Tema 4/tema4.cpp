#define _CRT_SECURE_NO_WARNINGS

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
		double beginx = -0.8 + begin_column * (1.6 / col);
		double beginy = 0.8 - begin_line * (1.6 / lin);
		double endx = -0.8 + end_column * (1.6 / col);
		double endy = 0.8 - end_line * (1.6 / lin);
		char sensx[10], sensy[10];
		if (begin_column < end_column)
		{
			strcpy(sensx, "dreapta");
		}
		else
		{
			strcpy(sensx, "stanga");
		}
		if (begin_line < end_line)
		{
			strcpy(sensy, "jos");
		}
		else
		{
			strcpy(sensy, "sus");
		}
		// panta: m
		double slope = (endy - beginy) / (endx - beginx);
		// ecuatia dreptei: y - beginy = m (x - beginx) ( x va fi incrementat la fiecare coloana strabatuta , in cazul in care unghiul < 45d)

		glColor3f(1.0, 0.1, 0.1);
		glBegin(GL_LINE_STRIP);
			glVertex2f(beginx, beginy);
			glVertex2f(endx, endy);
		glEnd();
		writePixel(begin_line, begin_column);
		writePixel(end_line, end_column);
		double cosine = (abs(endx - beginx)) / (sqrt(pow(endx - beginx, 2) + pow(endy - beginy, 2)));
		//double verifCosine = cos(PI/4);
		// if 0.707 - 45 degrees angle, if > - iterate through columns, if < - iterate through lines
		//cout << verifCosine - cosine;
		if (cosine >= 0.707106 && cosine <= 0.707107)
		{
			cout << "45 degrees angle" << endl;
			//cout << sensx << " " << sensy;
			if (!strcmp(sensx, "dreapta") && !strcmp(sensy, "sus"))
			{
				int line = begin_line - 1;
				for (int column = begin_column + 1; column < end_column; column++)
				{
					cout << "column: " << column << " line: " << line << endl;
					//cout << passingPoint(slope, beginx, beginy, (-0.8 + column * (1.6 / col)), "dreapta sus") << endl;
					writePixel(line, column);
					line--;
				}
			}
			if (!strcmp(sensx, "dreapta") && !strcmp(sensy, "jos"))
			{
				int line = begin_line + 1;
				for (int column = begin_column + 1; column < end_column; column++)
				{
					cout << "column: " << column << " line: " << line << endl;
					//cout << passingPoint(slope, beginx, beginy, (-0.8 + column * (1.6 / col)), "dreapta sus") << endl;
					writePixel(line, column);
					line++;
				}
			}
			if (!strcmp(sensx, "stanga") && !strcmp(sensy, "jos"))
			{
				int line = begin_line + 1;
				for (int column = begin_column - 1; column > end_column; column--)
				{
					cout << "column: " << column << " line: " << line << endl;
					//cout << passingPoint(slope, beginx, beginy, (-0.8 + column * (1.6 / col)), "dreapta sus") << endl;
					writePixel(line, column);
					line++;
				}
			}
			if (!strcmp(sensx, "stanga") && !strcmp(sensy, "sus"))
			{
				int line = begin_line - 1;
				for (int column = begin_column - 1; column > end_column; column--)
				{
					cout << "column: " << column << " line: " << line << endl;
					//cout << passingPoint(slope, beginx, beginy, (-0.8 + column * (1.6 / col)), "dreapta sus") << endl;
					writePixel(line, column);
					line--;
				}
			}
		}
		else if (cosine > 0.707107)
		{
			cout << "angle lower than 45 degrees - segment intersects columns" << endl;
			if (!strcmp(sensx, "dreapta") && !strcmp(sensy, "sus"))
			{
				// drawing intersecting points
				for (int column = begin_column + 1; column < end_column; column++)
				{
					double passing_point = passingPoint(slope, beginx, beginy, (-0.8 + column * (1.6 / col)), "< 45");
					cout << "column: " << column << " intersected in point: " << passing_point << endl;
					findClosestPixel(passing_point, column, "dreapta sus", "< 45");
				}
			}
			if (!strcmp(sensx, "dreapta") && !strcmp(sensy, "jos"))
			{
				for (int column = begin_column + 1; column < end_column; column++)
				{
					double passing_point = passingPoint(slope, beginx, beginy, (-0.8 + column * (1.6 / col)), "< 45");
					//cout << "column: " << column << " intersected in point: " << passing_point << endl;
					findClosestPixel(passing_point, column, "dreapta jos", "< 45");
				}
			}
		}
		else if (cosine < 0.707106)
		{
			cout << "angle higher than 45 degrees - segment intersects lines" << endl;
			if (!strcmp(sensx, "dreapta") && !strcmp(sensy, "sus"))
			{
				for (int line = begin_line - 1; line > end_line; line--)
				{
					double passing_point = passingPoint(slope, beginx, beginy, (0.8 - line * (1.6 / lin)), "> 45");
					cout << "line: " << line << " intersected in point: " << passing_point << endl;
					findClosestPixel(passing_point, line, "dreapta sus", "> 45");
				}
			}
			if (!strcmp(sensx, "dreapta") && !strcmp(sensy, "jos"))
			{
				for (int line = begin_line + 1; line < end_line; line++)
				{
					double passing_point = passingPoint(slope, beginx, beginy, (0.8 - line * (1.6 / lin)), "> 45");
					cout << "line: " << line << " intersected in point: " << passing_point << endl;
					/*glColor3f(0.1, 0.5, 1.0);
					glBegin(GL_POINTS);
					glVertex2f(passing_point, (0.8 - line * (1.6 / lin)));
					glEnd();*/
					findClosestPixel(passing_point, line, "dreapta jos", "> 45");
				}
			}
		}
	}

private:
	float lin,col;

	double passingPoint(double m, double x1, double y1, double known_point, char angle[10])
	{
		if (!strcmp(angle, "> 45"))
		{
			return (known_point - y1) / m + x1;
		}
		else if (!strcmp(angle, "< 45"))
		{
			return m * (known_point - x1) + y1;
		}
	}

	void findClosestPixel(double passing_point, int decision_line, char directie[20], char unghi[10])
	{
		if (!strcmp(directie, "dreapta sus"))
		{
			if (!strcmp(unghi, "< 45"))
			{
				for (double y = 0.8; y > -0.8; y -= 1.6 / lin)
				{
					// we have reached the intersection point
					if (passing_point < y && passing_point > y - 1.6 / lin)
					{
						int chosen_line = 0;
						double pointy = y;
						if (y - passing_point < passing_point - (y - 1.6 / lin))
						{
							// double beginx = -0.8 + begin_column * (1.6 / col);
							while (pointy < 0.8)
							{
								chosen_line++;
								pointy += 1.6 / lin;
							}
							cout << "chosen line: " << chosen_line << endl;
						}
						else
						{
							while (pointy - 1.6 / lin < 0.8)
							{
								chosen_line++;
								pointy += 1.6 / lin;
							}
							cout << "chosen line: " << chosen_line << endl;
						}
						if (y - passing_point == passing_point - (y - 1.6 / lin))
						{
							while (pointy - 1.6 / lin < 0.8)
							{
								chosen_line++;
								pointy += 1.6 / lin;
							}
							cout << "chosen line: " << chosen_line << endl;
						}
						writePixel(chosen_line, decision_line);
					}
				}
			}
			else if (!strcmp(unghi, "> 45"))
			{
				for (double x = -0.8; x < 0.8; x += 1.6 / col)	//	for angles > 45
				{
					// we have reached the intersection point
					if (passing_point > x && passing_point < x + 1.6 / col)
					{
						int chosen_line = 0;
						double pointx = x;
						if (passing_point - x < (x + 1.6 / col) - passing_point)
						{
							// double beginx = -0.8 + begin_column * (1.6 / col);
							while (pointx > -0.8)
							{
								chosen_line++;
								pointx -= 1.6 / col;
							}
							cout << "chosen line: " << chosen_line << endl;
						}
						else
						{
							while (pointx + 1.6 / col > -0.8)
							{
								chosen_line++;
								pointx -= 1.6 / col;
							}
							cout << "chosen line: " << chosen_line << endl;
						}
						writePixel(decision_line, chosen_line);
					}
				}
			}
		}

		if (!strcmp(directie, "dreapta jos"))
		{
			if (!strcmp(unghi, "< 45"))
			{
				for (double y = 0.8; y > -0.8; y -= 1.6 / lin)
				{
					// we have reached the intersection point
					if (passing_point < y && passing_point > y - 1.6 / lin)
					{
						int chosen_line = 0;
						double pointy = y;
						if (y - passing_point < passing_point - (y - 1.6 / lin))
						{
							// double beginx = -0.8 + begin_column * (1.6 / col);
							while (pointy < 0.8)
							{
								chosen_line++;
								pointy += 1.6 / lin;
							}
							cout << "chosen line: " << chosen_line << endl;
						}
						else
						{
							while (pointy - 1.6 / lin < 0.8)
							{
								chosen_line++;
								pointy += 1.6 / lin;
							}
							cout << "chosen line: " << chosen_line << endl;
						}
						writePixel(chosen_line, decision_line);
					}
				}
			}
			if (!strcmp(unghi, "> 45"))
			{
				for (double x = -0.8; x < 0.8; x += 1.6 / col)	//	for angles > 45
				{
					// we have reached the intersection point
					if (passing_point > x && passing_point < x + 1.6 / col)
					{
						int chosen_line = 0;
						double pointx = x;
						if (passing_point - x < (x + 1.6 / col) - passing_point)
						{
							// double beginx = -0.8 + begin_column * (1.6 / col);
							while (pointx > -0.8)
							{
								chosen_line++;
								pointx -= 1.6 / col;
							}
							cout << "chosen line: " << chosen_line << endl;
						}
						else
						{
							while (pointx + 1.6 / col > -0.8)
							{
								chosen_line++;
								pointx -= 1.6 / col;
							}
							cout << "chosen line: " << chosen_line << endl;
						}
						writePixel(decision_line, chosen_line);
					}
				}
			}

		}

	}
};



void Display1(){
	GrilaCarteziana grila =	GrilaCarteziana(15, 15);
	grila.deseneazaGrila();
	//grila.writePixel(4, 14);
	grila.drawSegment(3, 3, 15, 13);
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


