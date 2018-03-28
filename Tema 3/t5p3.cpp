#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "glut.h"

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;
int nivel = 0;


class C2coord
{
public:
  C2coord() 
  {
    m.x = m.y = 0;
  }

  C2coord(double x, double y) 
  {
    m.x = x;
    m.y = y;
  }

  C2coord(const C2coord &p) 
  {
    m.x = p.m.x;
    m.y = p.m.y;
  }

  C2coord &operator=(C2coord &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  int operator==(C2coord &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

protected:
  struct SDate
  {
    double x,y;
  } m;
};

class CPunct : public C2coord
{
public:
  CPunct() : C2coord(0.0, 0.0)
  {}

  CPunct(double x, double y) : C2coord(x, y)
  {}

  CPunct &operator=(const CPunct &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  void getxy(double &x, double &y)
  {
    x = m.x;
    y = m.y;
  }

  int operator==(CPunct &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

  void marcheaza()
  {
    glBegin(GL_POINTS);
      glVertex2d(m.x, m.y);
    glEnd();
  }

  void print(FILE *fis)
  {
    fprintf(fis, "(%+f,%+f)", m.x, m.y);
  }

};

class CVector : public C2coord
{
public:
  CVector() : C2coord(0.0, 0.0)
  {
    normalizare();
  }

  CVector(double x, double y) : C2coord(x, y)
  {
    normalizare();
  }

  CVector &operator=(CVector &p)
  {
    m.x = p.m.x;
    m.y = p.m.y;
    return *this;
  }

  int operator==(CVector &p)
  {
    return ((m.x == p.m.x) && (m.y == p.m.y));
  }

  CPunct getDest(CPunct &orig, double lungime)
  {
    double x, y;
    orig.getxy(x, y);
    CPunct p(x + m.x * lungime, y + m.y * lungime);
    return p;
  }

  void rotatie(double grade)
  {
    double x = m.x;
    double y = m.y;
    double t = 2 * (4.0 * atan(1.0)) * grade / 360.0;
    m.x = x * cos(t) - y * sin(t);
    m.y = x * sin(t) + y * cos(t);
    normalizare();
  }

  void deseneaza(CPunct p, double lungime) 
  {
    double x, y;
    p.getxy(x, y);
    glColor3f(1.0, 0.1, 0.1);
    glBegin(GL_LINE_STRIP);
      glVertex2d(x, y);
      glVertex2d(x + m.x * lungime, y + m.y * lungime);
    glEnd();
  }

  void deseneazaPatrat(CPunct p, double lungime)
  {
	  double x, y;
	  p.getxy(x, y);
	  glColor3f(1.0, 0.1, 0.1);
	  glBegin(GL_POLYGON);
		glVertex2d(x, y);
		glVertex2d(x + lungime, y);
		glVertex2d(x + lungime, y + lungime);
		glVertex2d(x, y + lungime);
	  glEnd();

  }

  void print(FILE *fis)
  {
    fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
  }

private:
  void normalizare()
  {
    double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
    if (d != 0.0) 
    {
      C2coord::m.x = C2coord::m.x * 1.0 / d;
      C2coord::m.y = C2coord::m.y * 1.0 / d;
    }
  }
};

class CCurbaKoch
{
public:
  void segmentKoch(double lungime, int nivel, CPunct &p, CVector v)
  {
    CPunct p1;
    if (nivel == 0) 
    {
      v.deseneaza(p, lungime);
    }
    else
    {
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p, v);
      p1 = v.getDest(p, lungime / 3.0);
      v.rotatie(60);
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p1, v);
      p1 = v.getDest(p1, lungime / 3.0);
      v.rotatie(-120);
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p1, v);
      p1 = v.getDest(p1, lungime / 3.0);
      v.rotatie(60);
  //    v.print(stderr);
  //    fprintf(stderr, "\n");
      segmentKoch(lungime / 3.0, nivel - 1, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v1(sqrt(3.0)/2.0, 0.5);
    CPunct p1(-1.0, 0.0);

    CVector v2(0.0, -1.0);
    CPunct p2(0.5, sqrt(3.0)/2.0);

    CVector v3(-sqrt(3.0)/2.0, 0.5);
    CPunct p3(0.5, -sqrt(3.0)/2.0);

    segmentKoch(lungime, nivel, p1, v1);
    segmentKoch(lungime, nivel, p2, v2);
    segmentKoch(lungime, nivel, p3, v3);
  }
};

class CArboreBinar
{
public:
  void arboreBinar(double lungime, int nivel, CPunct &p, CVector v)
  {
    CPunct p1;
    if (nivel == 0) 
    {
      v.deseneaza(p, lungime);
    }
    else
    {
      arboreBinar(lungime, nivel - 1, p, v);
      p1 = v.getDest(p, lungime);

      v.rotatie(-45);
      arboreBinar(lungime / 2.0, nivel - 1, p1, v);

      v.rotatie(90);
      arboreBinar(lungime / 2.0, nivel - 1, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, -1.0);
    CPunct p(0.0, 1.0);

    arboreBinar(lungime, nivel, p, v);
  }
};

class CArborePerron
{
public:
  void arborePerron(double lungime, 
                    int nivel, 
                    double factordiviziune, 
                    CPunct p, 
                    CVector v)
  {
    assert(factordiviziune != 0);
    CPunct p1, p2;
    if (nivel == 0) 
    {
    }
    else
    {
      v.rotatie(30);
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      v.rotatie(-90);
      v.deseneaza(p, lungime);
      p1 = v.getDest(p, lungime);
      p2 = p1;

      v.rotatie(-30);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      p1 = p2;
      v.rotatie(90);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      p2 = p1;

      v.rotatie(30);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

      p1 = p2;
      v.rotatie(-90);
      v.deseneaza(p1, lungime);
      p1 = v.getDest(p1, lungime);
      arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, 1.0);
    CPunct p(0.0, -1.0);

    v.deseneaza(p, 0.25);
    p = v.getDest(p, 0.25);
    arborePerron(lungime, nivel, 0.4, p, v);
  }
};



class CCurbaHilbert
{
public:
  void curbaHilbert(double lungime, int nivel, CPunct &p, CVector &v, int d)
  {
    if (nivel == 0) 
    {
    }
    else
    {
      v.rotatie(d * 90);
      curbaHilbert(lungime, nivel - 1, p, v, -d);

      v.deseneaza(p, lungime);
      p = v.getDest(p, lungime);

      v.rotatie(-d * 90);
      curbaHilbert(lungime, nivel - 1, p, v, d);

      v.deseneaza(p, lungime);
      p = v.getDest(p, lungime);

      curbaHilbert(lungime, nivel - 1, p, v, d);

      v.rotatie(-d * 90);
      v.deseneaza(p, lungime);
      p = v.getDest(p, lungime);
      
      curbaHilbert(lungime, nivel - 1, p, v, -d);

      v.rotatie(d * 90);
    }
  }

  void afisare(double lungime, int nivel)
  {
    CVector v(0.0, 1.0);
    CPunct p(0.0, 0.0);

    curbaHilbert(lungime, nivel, p, v, 1);
  }
};

class Imagine1
{
	public:
		void imagine1(double lungime, int nivel, CPunct &p, CVector &v, int d)
		{
			CPunct punct;
			double lungime_nivel_superior;
			if (nivel == 0)
			{
				double x, y;
				p.getxy(x, y);

				CPunct centru(x + lungime, y - lungime);
				//v.rotatie(90 * d);
				v.deseneaza(centru, lungime);

				punct = v.getDest(centru, lungime);
				v.rotatie(-90 * d);
				v.deseneaza(punct, lungime);

				punct = v.getDest(punct, lungime);
				v.rotatie(-90 * d);
				v.deseneaza(punct, lungime);

				punct = v.getDest(punct, lungime);
				v.rotatie(-90 * d);
				v.deseneaza(punct, lungime);

				punct = v.getDest(punct, lungime);
				v.rotatie(-90 * d);

			}
			else
			{
				double x, y;
				p.getxy(x, y);
				lungime_nivel_superior = lungime / 3;

				CPunct sus(x + lungime, y + 2 * lungime_nivel_superior);
				imagine1(lungime_nivel_superior, nivel - 1, sus, v, 1);

				CPunct dr_sus(x + 2 * lungime, y + 2 * lungime_nivel_superior);
				imagine1(lungime_nivel_superior, nivel - 1, dr_sus, v, 1);

				CPunct dr(x + 2 * lungime, y - lungime_nivel_superior);
				imagine1(lungime_nivel_superior, nivel - 1, dr, v, 1);

				CPunct dr_jos(x + 2 * lungime, y - lungime - lungime_nivel_superior);
				imagine1(lungime_nivel_superior, nivel - 1, dr_jos, v, 1);

				CPunct jos(x + lungime, y - lungime - lungime_nivel_superior);
				imagine1(lungime_nivel_superior, nivel - 1, jos, v, 1);

				CPunct st_jos(x, y - lungime - lungime_nivel_superior);
				imagine1(lungime_nivel_superior, nivel - 1, st_jos, v, 1);

				CPunct st(x, y - lungime + 2 * lungime_nivel_superior);
				imagine1(lungime_nivel_superior, nivel - 1, st, v, 1);

				CPunct st_sus(x, y + 2 * lungime_nivel_superior);
				imagine1(lungime_nivel_superior, nivel - 1, st_sus, v, 1);

				imagine1(lungime, nivel - 1, p, v, 1);

			
			}
		}

		void afisare(double lungime, int nivel)
		{
			CVector v(0.0, 0.1);
			CPunct p(-0.75, 0.25);

			imagine1(lungime, nivel, p, v, 1);
		}
};

class Imagine2
{
public:
	void imagine2(double lungime,
		int nivel,
		double factordiviziune,
		CPunct p,
		CVector v)
	{
		assert(factordiviziune != 0);
		CPunct p1, p2;
		if (nivel == 0)
		{
		}
		else
		{
			v.deseneaza(p, 0.3 * lungime);
			p = v.getDest(p, 0.3 * lungime);

			v.rotatie(45);
			v.deseneaza(p, lungime);
			p1 = v.getDest(p, lungime);

			v.rotatie(-90);
			v.deseneaza(p, lungime * 0.8);
			p1 = v.getDest(p, lungime * 0.8);
			imagine2(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
			p1 = v.getDest(p1, lungime * 0.2);


			v.rotatie(135);
			p1 = v.getDest(p1, lungime * 1.4);
			v.rotatie(-90);
			v.deseneaza(p1, lungime);
			p2 = v.getDest(p1, lungime);

			v.rotatie(-90);
			v.deseneaza(p2, 0.4 * lungime);
			p2 = v.getDest(p2, 0.4 * lungime);
			imagine2(lungime * factordiviziune, nivel - 1, factordiviziune, p2, v);

			v.rotatie(180);
			p2 = v.getDest(p2, 0.4 * lungime);
			v.rotatie(-60);
			v.deseneaza(p2, 0.4 * lungime);
			p2 = v.getDest(p2, 0.4 * lungime);
			imagine2(lungime * factordiviziune, nivel - 1, factordiviziune, p2, v);
			v.rotatie(-30);


			v.rotatie(60);
			v.deseneaza(p1, lungime);
			p1 = v.getDest(p1, lungime);
			imagine2(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
		}
	}

	void afisare(double lungime, int nivel)
	{
		CVector v(0.0, 1.0);
		CPunct p(-0.25, 1.0);

		v.rotatie(180);
		imagine2(lungime, nivel, 0.35, p, v);
	}
};

class Imagine3
{
public:
	void imagine3(double lungime, int nivel, double factordiviziune, CPunct &p, CVector &v, int d, int directie, double orientare)
	{
		if (nivel == 0)
		{
			v.deseneaza(p, lungime);
			p = v.getDest(p, lungime);
		}
		else
		{
			if (directie == 0)
			{
				imagine3(lungime*factordiviziune, nivel - 1, factordiviziune, p, v, d, (directie + 1) % 2, orientare);
				v.rotatie(60 * d * orientare);

				imagine3(lungime*factordiviziune, nivel - 1, factordiviziune, p, v, d, directie, orientare);
				v.rotatie(60 * d * orientare);

				imagine3(lungime*factordiviziune, nivel - 1, factordiviziune, p, v, d, (directie + 1) % 2, orientare);
			}
			else
			{
				imagine3(lungime*factordiviziune, nivel - 1, factordiviziune, p, v, d, (directie + 1) % 2, orientare);
				v.rotatie(-60 * d * orientare);

				imagine3(lungime*factordiviziune, nivel - 1, factordiviziune, p, v, d, directie, orientare);
				v.rotatie(-60 * d * orientare);

				imagine3(lungime*factordiviziune, nivel - 1, factordiviziune, p, v, d, (directie + 1) % 2, orientare);
			}

		}
	}

	void afisare(double lungime, int nivel)
	{
		CVector v(1.0, 0.0);
		CPunct p(-0.98, 0.6);
		v.rotatie(-90);

		if (nivel % 2 == 0)
		{
			imagine3(lungime, nivel, 0.53, p, v, 1, 0, -1);
		}
		else
		{
			imagine3(lungime, nivel, 0.53, p, v, 1, 0, 1);
		}
	}
};



// afisare curba lui Koch "fulg de zapada"
void Display1() {
  CCurbaKoch cck;
  cck.afisare(sqrt(3.0), nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'K');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'h');

  nivel++;
}

// afisare arbore binar
void Display2() {
  CArboreBinar cab;
  cab.afisare(1, nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');

  nivel++;
}

// afisare arborele lui Perron
void Display3() {
  CArborePerron cap;

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,-0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');

  glPushMatrix();
  glLoadIdentity();
  glScaled(0.4, 0.4, 1);
  glTranslated(-0.5, -0.5, 0.0);
  cap.afisare(1, nivel);
  glPopMatrix();
  nivel++;
}

// afisare curba lui Hilbert
void Display4() {
  CCurbaHilbert cch;
  cch.afisare(0.05, nivel);

  char c[3];
  sprintf(c, "%2d", nivel);
  glRasterPos2d(-0.98,-0.98);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

  glRasterPos2d(-1.0,-0.9);
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'H');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
  glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');

  nivel++;
}

void Display5() {
	Imagine1 img1;
	img1.afisare(0.5, nivel);

	nivel++;
}

void Display6() {
	Imagine2 img2;
	img2.afisare(0.45, nivel);

	nivel++;
}

void Display7() {
	Imagine3 img3;
	img3.afisare(0.8, nivel);

	nivel++;
}

void Init(void) {

   glClearColor(1.0,1.0,1.0,1.0);

   glLineWidth(1);

   glPointSize(3);

   glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) 
{
  switch(prevKey) 
  {
    case '0':
      glClear(GL_COLOR_BUFFER_BIT);
      nivel = 0;
      fprintf(stderr, "nivel = %d\n", nivel);
      break;
    case '1':
      glClear(GL_COLOR_BUFFER_BIT);
      Display1();
      break;
    case '2':
      glClear(GL_COLOR_BUFFER_BIT);
      Display2();
      break;
    case '3':
      glClear(GL_COLOR_BUFFER_BIT);
      Display3();
      break;
    case '4':
      glClear(GL_COLOR_BUFFER_BIT);
      Display4();
      break;
	case '5':
		glClear(GL_COLOR_BUFFER_BIT);
		Display5();
		break;
	case '6':
		glClear(GL_COLOR_BUFFER_BIT);
		Display6();
		break;
	case '7':
		glClear(GL_COLOR_BUFFER_BIT);
		Display7();
		break;
    default:
      break;
  }

  glFlush();
}

void Reshape(int w, int h) 
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void KeyboardFunc(unsigned char key, int x, int y) 
{
   prevKey = key;
   if (key == 27) // escape
      exit(0);
   glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) 
{
}

int main(int argc, char** argv) 
{
  glutInit(&argc, argv);

  glutInitWindowSize(dim, dim);

  glutInitWindowPosition(100, 100);

  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

  glutCreateWindow (argv[0]);

  Init();

  glutReshapeFunc(Reshape);

  glutKeyboardFunc(KeyboardFunc);

  glutMouseFunc(MouseFunc);

  glutDisplayFunc(Display);

  glutMainLoop();

  return 0;
}

