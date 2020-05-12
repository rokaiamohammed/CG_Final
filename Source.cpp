#include <GL/glut.h>
#include <stdlib.h>
//#include "glm.h"
//#include "imageloader.h"
#include <math.h>
#include <stdio.h> 
#include <windows.h>

static int ArmR = 0, ArmRF = 0, ArmLF = 0, ArmL = 0, elbowR = 0, elbowL = 0;
static int LegR = 0, LegL = 0, LegRM = 0, LegLM = 0, KneeR = 0, KneeL = 0;
static int fingerBaseR1 = 0, fingerBaseR2 = 0, fingerBaseR3 = 0, fingerBase4R = 0, fingerBase5R = 0;
static int fingerBaseL1 = 0, fingerBaseL2 = 0, fingerBaseL3 = 0, fingerBase4L = 0, fingerBase5L = 0;
static int Body = 0, head = 0;

static int window;
static int menu_id;
static int submenu1, submenu2, submenu3, submenu4, submenu5, submenu6, submenu7, submenu8;
static int value = 0;

static int moving, startx, starty;
GLfloat angle = 0.0;   /* in degrees */
GLfloat angle2 = 0.0;   /* in degrees */

double speed = 0.2;

double eye[] = { 0, 0,-10 };
double center[] = { 0, 0, 1 };
double up[] = { 0, 1, 0 };

double direction[3];
GLfloat lightZeroPosition[] = { 10.0, 14.0, 10.0, 1.0 };
GLfloat lightZeroColor[] = { 0.8, 1.0, 0.8, 1.0 }; /* green-tinted */
GLfloat lightOnePosition[] = { -1.0, 1.0, 1.0, 0.0 };
GLfloat lightOneColor[] = { 0.6, 0.3, 0.2, 1.0 }; /* red-tinted */
GLdouble bodyWidth = 3.0;
int useStencil = 0;  /* Initially, allow the artifacts. */
/////////////////////////////////////

// RGBA
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5,1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// x , y, z, w
GLfloat light_position[] = { 0.5,5.0, 0.0, 1.0 };
GLfloat lightPos1[] = { -0.5,-5.0,-2.0, 1.0 };
// Material Properties
GLfloat mat_amb_diff[] = { 0.643, 0.753, 0.934, 1.0 };
GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat shininess[] = { 100.0 };

//GLuint texture; //the array for our texture

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void crossProduct(double a[], double b[], double c[])
{
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double a[])
{
	double norm;
	norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
	norm = sqrt(norm);
	a[0] /= norm;
	a[1] /= norm;
	a[2] /= norm;
}

void rotatePoint(double a[], double theta, double p[])
{

	double temp[3];
	temp[0] = p[0];
	temp[1] = p[1];
	temp[2] = p[2];

	temp[0] = -a[2] * p[1] + a[1] * p[2];
	temp[1] = a[2] * p[0] - a[0] * p[2];
	temp[2] = -a[1] * p[0] + a[0] * p[1];

	temp[0] *= sin(theta);
	temp[1] *= sin(theta);
	temp[2] *= sin(theta);

	temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
	temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
	temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

	temp[0] += cos(theta) * p[0];
	temp[1] += cos(theta) * p[1];
	temp[2] += cos(theta) * p[2];

	p[0] = temp[0];
	p[1] = temp[1];
	p[2] = temp[2];
}

void SetDirection(double d[])
{
	d[0] = center[0] - eye[0];
	d[1] = center[1] - eye[1];
	d[2] = center[2] - eye[2];
}

void Left()
{
	// implement camera rotation arround vertical window screen axis to the left
	// used by mouse and left arrow
	rotatePoint(up, 0.05, eye);
}

void Right()
{
	// implement camera rotation arround vertical window screen axis to the right
	// used by mouse and right arrow
	rotatePoint(up, -0.05, eye);

}

void Up()
{
	// implement camera rotation arround horizontal window screen axis +ve
	// used by up arrow

	double sub[3];
	for (int i = 0; i < 3; i++)
	{
		sub[i] = center[i] - eye[i];
	}
	double cross[3];
	crossProduct(sub, up, cross);
	normalize(cross);
	rotatePoint(cross, 0.05, eye);
	rotatePoint(cross, 0.05, up);

}

void Down()
{
	// implement camera rotation arround horizontal window screen axis 
	// used by down arrow

	double sub[3];
	for (int i = 0; i < 3; i++)
	{
		sub[i] = center[i] - eye[i];
	}
	double cross[3];
	crossProduct(sub, up, cross);
	normalize(cross);
	rotatePoint(cross, -0.05, eye);
	rotatePoint(cross, -0.05, up);
}

void moveForward()
{

	SetDirection(direction);

	eye[0] += direction[0] * speed;
	eye[1] += direction[1] * speed;
	eye[2] += direction[2] * speed;

	center[0] += direction[0] * speed;
	center[1] += direction[1] * speed;
	center[2] += direction[2] * speed;

}

void moveBack()
{

	SetDirection(direction);

	eye[0] -= direction[0] * speed;
	eye[1] -= direction[1] * speed;
	eye[2] -= direction[2] * speed;

	center[0] -= direction[0] * speed;
	center[1] -= direction[1] * speed;
	center[2] -= direction[2] * speed;
}

static void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			moving = 1;
			startx = x;
			starty = y;
		}
		if (state == GLUT_UP) {
			moving = 0;
		}
	}
}


static void motion(int x, int y)
{
	if (moving) {
		angle = angle + (x - startx);
		angle2 = angle2 + (y - starty);
		startx = x;
		starty = y;
		glutPostRedisplay();
	}
}

void Leg(static int Leg, static int Knee, static int Side, int i)
{
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.6f);
	glTranslatef(0.0, -0.5, 0.0);
	if (i == 0)             // for right leg
	{
		glRotatef((GLfloat)Leg, -1.0, 0.0, 0.0);
		glRotatef((GLfloat)Side, 0.0, 0.0, 1.0);
	}
	else if (i == 1)       // for left leg
	{
		glRotatef((GLfloat)Leg, 1.0, 0.0, 0.0);
		glRotatef((GLfloat)Side, 0.0, 0.0, -1.0);
	}

	glTranslatef(0.0, -0.5, 0.0);

	glPushMatrix();
	glTranslatef(0.3, 0.0, 0.0);   //LegRight
	glScalef(0.4, 1.0, 0.3);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0.0, -0.5, 0.0);
	if (i == 0)             // for right leg knee
	{
		glRotatef((GLfloat)Knee, 1.0, 0.0, 0.0);
	}
	else if (i == 1)       // for left leg knee
	{
		glRotatef((GLfloat)Knee, -1.0, 0.0, 0.0);
	}
	glTranslatef(0.0, -0.5, 0.0);

	glPushMatrix();
	glTranslatef(0.3, -0.7, 0.0);   //FootRight
	glScalef(0.4, 0.4, 0.3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3, 0.0, 0.0);   //KneeRigth
	glScalef(0.4, 1.0, 0.3);
	glutWireCube(1);
	glPopMatrix();

	glPopMatrix();
}
void Arm(static int Shoulder, static int Elbow, static int ShoulderF, int i)
{
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.75, 1.5, 0.0);
	if (i == 0)                   // 0 For the Right Arm
	{
		glRotatef((GLfloat)Shoulder, 0.0, 0.0, 1.0);
		glRotatef((GLfloat)ShoulderF, -1.0, 0.0, 0.0);
	}
	else if (i == 1)            // 1 For the Left Arm 
	{
		glRotatef((GLfloat)Shoulder, 0.0, 0.0, -1.0);
		glRotatef((GLfloat)ShoulderF, -1.0, 0.0, 0.0);
	}
	glTranslatef(-0.75, -1.5, 0.0);

	glPushMatrix();
	glTranslatef(0.75, 1.0, 0.0);   //ShoulderRight
	glScalef(0.3, 1.0, 0.3);
	glutWireCube(1);
	glPopMatrix();


	glTranslatef(0.0, 0.5, 0.0);
	if (i == 0)
	{
		glRotatef((GLfloat)Elbow, 1.0, 0.0, 0.0);
	}
	else if (i == 1)
	{
		glRotatef((GLfloat)Elbow, -1.0, 0.0, 0.0);
	}

	glTranslatef(0.0, 0.5, 0.0);

	glPushMatrix();
	glTranslatef(0.75, -1, 0.0);   //Elbowright
	glScalef(0.3, 1.0, 0.3);
	glutWireCube(1);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.8705, -1.6, -0.1);                   //fingerbase5rRight
	glScalef(0.075, 0.2, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.8705, -1.6, 0.1);                   //fingerbaserRight
	glScalef(0.075, 0.2, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7945, -1.6, 0.1);                   //fingerbase3rRight
	glScalef(0.075, 0.2, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7215, -1.6, 0.1);                   //fingerbase2rRight
	glScalef(0.075, 0.2, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.685, -0.515, 0.0);
	glRotatef((GLfloat)fingerBaseR1, 1.0, 0.0, 0.0);
	glTranslatef(-0.685, 0.515, 0.0);

	glPushMatrix();
	glTranslatef(0.6475, -1.6, 0.1);   //fingerbase1Right
	glScalef(0.075, 0.2, 0.1);
	glutWireCube(1);
	glPopMatrix();


	glTranslatef(0.6475, -1.75, 0.1);                   //finger1Right
	glScalef(0.075, 0.1, 0.1);
	glutWireCube(1);

	glPopMatrix();

	/////////////////////

	glPushMatrix();
	glTranslatef(0.8705, -1.75, -0.1);                   //finger5rRight
	glScalef(0.075, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.8705, -1.75, 0.1);                   //finger4Right
	glScalef(0.075, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7945, -1.75, 0.1);                   //finger3Right
	glScalef(0.075, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7215, -1.75, 0.1);                   //finger2Right
	glScalef(0.075, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();


	glPopMatrix();
}
void Chest(void)
{
	glPushMatrix();
	glColor3f(1.0f, 0.5f, 0.0f);
	glTranslatef(0.0, 0.5, 0.0);   //Chest
	glScalef(1.0, 2.0, 0.5);
	glutSolidCube(1);
	glPopMatrix();

}
void Head(void)
{
	glPushMatrix();
	glColor3f(3.0f, 0.65, 2.0f);
	glRotatef((GLfloat)head, 0.0, 1.0, 0.0);

	glPushMatrix();
	glTranslatef(0.0, 2.0, 0.0);
	glutWireSphere(0.3, 16, 16);
	glPopMatrix();

	glPopMatrix();

}
void drawRoom(void)
{

	/////////////////////////
	//floor
	
	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.7f, 0.7f);
	glVertex3f(-10.0f, -2.9f, -10.0f);
	glVertex3f(-10.0f, -2.9f, 10.0f);
	glVertex3f(10.0f, -2.9f, 10.0f);
	glVertex3f(10.0f, -2.9f, -10.0f);
	glEnd();

	/////////////////////////////

	//wall
	glBegin(GL_QUADS);
	glColor3f(0.9294f, 0.9216f, 0.8353f);
	
	glVertex3f(-10.0f, -2.9f, -10.0f);
	glVertex3f(-10.0f, 7.0f, -10.0f);
	glVertex3f(10.0f, 7.0f, -10.0f);
	glVertex3f(10.0f, -2.9f, -10.0f);
	glEnd();

	//wall
	
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.851f, 0.702f);
	glVertex3f(-10.0f, -2.9f, -10.0f);
	glVertex3f(-10.0f, 7.0f, -10.0f);
	glVertex3f(-10.0f, 7.0f, 10.0f);
	glVertex3f(-10.0f, -2.9f, 10.0f);
	glEnd();

	//wall with door
	
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.851f, 0.702f);
	glVertex3f(-10.0f, -2.9f, 10.0f);
	glVertex3f(-10.0f, 7.0f, 10.0f);
	glVertex3f(-6.0f, 7.0f, 10.0f);
	glVertex3f(-6.0f, -2.9f, 10.0f);
	glEnd();

	
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.851f, 0.702f);
	glVertex3f(-3.0f, -2.9f, 10.0f);
	glVertex3f(-3.0f, 7.0f, 10.0f);
	glVertex3f(10.0f, 7.0f, 10.0f);
	glVertex3f(10.0f, -2.9f, 10.0f);
	glEnd();

	
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.851f, 0.702f);
	glVertex3f(-6.0f, 7.0f, 10.0f);
	glVertex3f(-6.0f, 5.0f, 10.0f);
	glVertex3f(-3.0f, 5.0f, 10.0f);
	glVertex3f(-3.0f, 7.0f, 10.0f);
	glEnd();

	glLineWidth(10.0f);
	glBegin(GL_LINES);
	glColor3f(0.4f, 0.2f, 0.0f);
	
	glVertex3f(-6.0f, 5.0f, 10.01f);
	glVertex3f(-3.0f, 5.0f, 10.01f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(-6.0f, 5.0f, 10.01f);
	glVertex3f(-6.0f, -2.9f, 10.01f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(-3.0f, -2.9f, 10.01f);
	glVertex3f(-3.0f, 5.0f, 10.01f);
	glEnd();
	//wall
	
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.851f, 0.702f);
	glVertex3f(10.0f, -2.9f, -10.0f);
	glVertex3f(10.0f, 7.0f, -10.0f);
	glVertex3f(10.0f, 7.0f, 10.0f);
	glVertex3f(10.0f, -2.9f, 10.0f);
	glEnd();

	//ceiling
	
	glBegin(GL_QUADS);
	glColor3f(0.95f, 0.95f, 0.95f);
	glVertex3f(-10.0f, 7.0f, -10.0f);
	glVertex3f(10.0f, 7.0f, -10.0f);
	glVertex3f(10.0f, 7.0f, 10.0f);
	glVertex3f(-10.0f, 7.0f, 10.0f);
	glEnd();

	//Floor pattern
	glPushMatrix();
	
	glColor3f(0.149f, 0.149f, 0.149f);
	glLineWidth(3.0f);
	for (int i = 0; i < 20; i += 2)
	{
		glBegin(GL_LINES);
		glVertex3f(-10.0f + i, -2.9f, -10.01f);
		glVertex3f(-10.0f + i, -2.9f, 10.01f);
		glEnd();
	}
	for (int i = 0; i < 20; i += 2)
	{
		glBegin(GL_LINES);
		
		glVertex3f(-10.0f, -2.9f, -10.01f + i);
		glVertex3f(10.0f, -2.9f, -10.01f + i);
		glEnd();
	}
	glPopMatrix();

	
	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.7333f, 1.0f);
	glVertex3f(-9.99f, 2.0f, 6.0f);
	glVertex3f(-9.99f, 5.0f, 6.0f);
	glVertex3f(-9.99f, 5.0f, 2.0f);
	glVertex3f(-9.99f, 2.0f, 2.0f);
	glEnd();
	glLineWidth(10.0f);
	glPushMatrix();
	
	glBegin(GL_LINES);
	glColor3f(0.149f, 0.149f, 0.149f);
	
	//left
	glVertex3f(-9.98f, 2.0f, 6.0f);
	glVertex3f(-9.98f, 5.0f, 6.0f);
	//top
	glVertex3f(-9.98f, 5.0f, 6.0f);
	glVertex3f(-9.98f, 5.0f, 2.0f);
	//right
	glVertex3f(-9.98f, 2.0f, 2.0f);
	glVertex3f(-9.98f, 5.0f, 2.0f);
	//bottom
	glVertex3f(-9.98f, 2.0f, 6.0f);
	glVertex3f(-9.98f, 2.0f, 2.0f);
	glEnd();
	glPopMatrix();
	//Cross
	glPushMatrix();

	glLineWidth(3.0f);
	glBegin(GL_LINES);
	
	glVertex3f(-9.98f, 2.0f, 4.0f);
	glVertex3f(-9.98f, 5.0f, 4.0f);

	glVertex3f(-9.98f, 3.5f, 6.0f);
	glVertex3f(-9.98f, 3.5f, 2.0f);
	glEnd();
	glPopMatrix();

	//////////////////////////////////////////////
	//glPushAttrib(GL_TEXTURE_BIT);

	//glDisable(GL_TEXTURE_2D);
	////glColor3f(0.6f, 0.6f, 0.6f);
	//glBegin(GL_QUADS);
	///* Floor */
	//glPushMatrix();
	//glColor3f(0.6f, 0.6f, 0.6f);
	//glNormal3f(10.0f, 2.9f, 10.0f);
	//glVertex3f(-10.0f, -2.9f, -15.0f);
	//glVertex3f(-10.0f, -2.9f, 15.0f);
	//glVertex3f(10.0f, -2.9f, 15.0f);
	//glVertex3f(10.0f, -2.9f, -15.0f);
	//glPopMatrix();
	///* Ceiling */
	//glPushMatrix();
	//glColor3f(0.4f, 0.6f, 0.6f);
	//glNormal3f(10.0f, -1.0f, 10.0f);
	//glVertex3f(-10.0f, 7.0f, 15.0f);
	//glVertex3f(-10.0f, 7.0f, -15.0f);
	//glVertex3f(10.0f, 7.0f, -15.0f);
	//glVertex3f(10.0f, 7.0f, 15.0f);
	//glPopMatrix();
	///* Back Wall */
	//glPushMatrix();
	//glColor3f(0.6f, 0.4f, 0.6f);
	//glNormal3f(0.0f, 0.0f, 1.0f);
	//glVertex3f(-10.0f, 7.0f, 15.0f);
	//glVertex3f(-10.0f, -2.9f, 15.0f);
	//glVertex3f(10.0f, -2.9f, 15.0f);
	//glVertex3f(10.0f, 7.0f, 15.0f);
	//glPopMatrix();
	///* Left Wall */
	//glPushMatrix();
	//glColor3f(0.6f, 0.6f, 0.4f);
	//glNormal3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(-10.0f, 7.0f, 15.0f);
	//glVertex3f(-10.0f, -2.9f, 15.0f);
	//glVertex3f(-10.0f, -2.9f, -15.0f);
	//glVertex3f(-10.0f, 7.0f, -15.0f);
	//glPopMatrix();
	///* Right Wall */
	//glPushMatrix();
	//glColor3f(0.6f, 0.6f, -0.6f);
	//glNormal3f(-1.0f, 0.0f, 0.0f);
	//glVertex3f(10.0f, 7.0f, -15.0f);
	//glVertex3f(10.0f, -2.9f, -15.0f);
	//glVertex3f(10.0f, -2.9f, 15.0f);
	//glVertex3f(10.0f, 7.0f, 15.0f);
	//glPopMatrix();
	//glEnd();

	//
	/*glEnable(GL_LIGHTING);
	//glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
	glVertex3f(-18.0, 0.0, 27.0);
	glVertex3f(27.0, 0.0, 27.0);
	glVertex3f(27.0, 0.0, -18.0);
	glVertex3f(-18.0, 0.0, -18.0);
	glEnd();*/
	//glEnable(GL_LIGHTING);
}
//void drawFloor(GLuint texture, float f)
//{
//	float f = 5.0;
//	glEnable(GL_TEXTURE_2D);
//	//glBindTexture(GL_TEXTURE_2D, texture);
//
//
//	glPushMatrix();
//
//	for (float a = -10.0; a <= 170.0; a = a + 10.0)
//	{
//		glBegin(GL_QUADS);
//		glTexCoord2f(0.0, 0.0);
//		glVertex3f(0.0 + a, -2.9, 10.0);
//		glTexCoord2f(0.0, f);
//		glVertex3f(0.0 + a, -2.9, 0.0);
//		glTexCoord2f(f, f);
//		glVertex3f(10.0 + a, -2.9, 0.0);
//		glTexCoord2f(f, 0.0);
//		glVertex3f(10.0 + a, -2.9, 10.0);
//		glEnd();
//	}
//
//	for (float b = -10.0; b <= 320.0; b += 10.0)
//	{
//		for (float a = -10.0; a <= 170.0; a = a + 10.0)
//		{
//			glBegin(GL_QUADS);
//			glTexCoord2f(0.0, 0.0);
//			glVertex3f(0.0 + a, 7.0, -(b - 10.0));
//			glTexCoord2f(0.0, f);
//			glVertex3f(0.0 + a, 7.0, -b);
//			glTexCoord2f(f, f);
//			glVertex3f(10.0 + a, 7.0, -b);
//			glTexCoord2f(f, 0.0);
//			glVertex3f(10.0 + a, 7.0, -(b - 10.0));
//			glEnd();
//
//		}
//	}
//
//	glPopMatrix();
//
//	glDisable(GL_TEXTURE_2D);
//
//	//LoadTexture::FreeCreatedTexture(texture);
//}


void menu(int num) {
	if (num == 0) {
		glutDestroyWindow(window);
		exit(0);
	}
	else {
		value = num;
	}
	glutPostRedisplay();
}
void createMenu(void) {

	submenu1 = glutCreateMenu(menu);
	glutAddMenuEntry("Football field", 1);
	glutAddMenuEntry("Yellow Sand", 2);
	glutAddMenuEntry("Red Sand", 3);
	glutAddMenuEntry("Track", 4);

	menu_id = glutCreateMenu(menu);

	glutAddSubMenu("Change Floor Texture", submenu1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//GLuint loadTexture(Image* image) {
//	GLuint textureId;
//	glGenTextures(1, &textureId); //Make room for our texture
//	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
//	//Map the image to the texture
//	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
//		0,                            //0 for now
//		GL_RGB,                       //Format OpenGL uses for image
//		image->width, image->height,  //Width and height
//		0,                            //The border of the image
//		GL_RGB, //GL_RGB, because pixels are stored in RGB format
//		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
//						  //as unsigned numbers
//		image->pixels);               //The actual pixel data
//	return textureId; //Returns the id of the texture
//}
//
//GLuint _textureId; //The id of the texture
//GLuint _textureId1; //The id of the texture
//
//void initRendering() {
//	Image* image = loadBMP("wood.bmp");
//	_textureId = loadTexture(image);
//	delete image;
//	// Turn on the power
//	glEnable(GL_LIGHTING);
//	// Flip light switch
//	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
//	// assign light parameters
//	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
//	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
//	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
//	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
//	// Material Properties         
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
//	GLfloat lightColor1[] = { 1.0f, 1.0f,  1.0f, 1.0f };
//	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
//	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
//	glEnable(GL_NORMALIZE);
//	//Enable smooth shading
//	glShadeModel(GL_SMOOTH);
//	// Enable Depth buffer
//	glEnable(GL_DEPTH_TEST);
//
//	//     	startList = glGenLists(4);
//	// glNewList(startList, GL_COMPILE);
//	// glRotatef(90, 0, 1, 0);
//	// 	glScalef(1, 1.2, 1);
//	// 	glTranslatef(1.7, -0.05, -.3);
//	// pmodel1 = pmodel4;
//	// drawmodel();
//	// glEndList();
//
//	// glNewList(startList + 1, GL_COMPILE);
//	// 	glRotatef(270, 0, 1, 0);
//	// 	glScalef(1, 1.2, 1);
//	// 	glTranslatef(-1.7, -0.05, -.3);
//	// pmodel1 = pmodel4;
//	// drawmodel();
//	// glEndList();
//
//	// glNewList(startList + 2, GL_COMPILE);
//	// glTranslatef(0.3, -.1, 0.075);
//	// pmodel1 = pmodel2;
//	// drawmodel();
//	// glEndList();
//
//	// glNewList(startList + 3, GL_COMPILE);
//	// glTranslatef(-0.6, 0, 0.0);
//	// pmodel1 = pmodel2;
//	// drawmodel();	
//	// glEndList();
//
//
//}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	//glFrustum(-2, 2, -1.5, 1.5, 1, 40);

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
	
	drawRoom();
	glPushMatrix();
	glLineWidth(1.0f);
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	glRotatef((GLfloat)Body, 0.0, 1.0, 0.0);

	Chest();

	glPushMatrix();                // For The Right Arm
	Arm(ArmR, elbowR, ArmRF, 0);
	glPopMatrix();

	glPushMatrix();             // For The Left Arm
	glRotatef(180, 0.0, 1.0, 0.0);
	Arm(ArmL, elbowL, ArmLF, 1);
	glPopMatrix();

	glPushMatrix();                // For The Right Leg
	Leg(LegR, KneeR, LegRM, 0);
	glPopMatrix();

	glPushMatrix();             // For The Left Leg
	glRotatef(180, 0.0, 1.0, 0.0);
	Leg(LegL, KneeL, LegLM, 1);
	glPopMatrix();

	Head();

	//Neck
	glPushMatrix();
	glTranslatef(0.0, 1.65, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.25, 0.25, 0.25);
	glutWireCube(1.0);
	glPopMatrix();

	/////////////////////////////////////////////
	////floor
	//glPushMatrix();
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, _textureId);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glBegin(GL_QUADS);

	//glNormal3f(0.0, -1.0, 0.0);
	//glTexCoord2f(0.0f, 0.0f);
	//glVertex3f(-0.5, -0.25, 2);
	//glTexCoord2f(5.0f, 0.0f);
	//glVertex3f(0.5, -0.25, 2);
	//glTexCoord2f(5.0f, 20.0f);
	//glVertex3f(0.5, -0.25, -2);
	//glTexCoord2f(0.0f, 20.0f);
	//glVertex3f(-0.5, -0.25, -2);
	//glEnd();
	//glDisable(GL_TEXTURE_2D);

	//glPopMatrix();


	///////////////////////////////


	glutSwapBuffers();


	if (value == 1) {
		printf("Football field\n");
		value = 0;

	}
	else if (value == 2) {
		printf("Yellow Sand\n");
		value = 0;

	}
	else if (value == 3) {
		printf("Red Sand\n");
		value = 0;
	}
	else if (value == 4) {
		printf("Track\n");
		value = 0;
	}

	glPopMatrix();
	glFlush();

}



void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
	//gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(0.0, 0.0, -5.0);
}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Left();
		break;
	case GLUT_KEY_RIGHT:
		Right();
		break;
	case GLUT_KEY_UP:
		Up(); break;
	case GLUT_KEY_DOWN:
		Down(); break;
	}

	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'b':
		Body = (Body + 2) % 360;
		glutPostRedisplay();

		break;
	case 'B':
		Body = (Body - 2) % 360;
		glutPostRedisplay();
		break;

	case 's':
		if (LegR < -90 || LegR <= 90) {

			LegR = (LegR + 5) % 360;
			glutPostRedisplay();
		}

		break;
	case 'S':
		if (LegR >= -90) {

			LegR = (LegR - 5) % 360;
			glutPostRedisplay();
		}
		break;

	case 'q':
		if (KneeR <= 90) {
			KneeR = (KneeR + 5) % 360;
			glutPostRedisplay();
		}
		break;
	case 'Q':
		if (KneeR >= 0) {
			KneeR = (KneeR - 5) % 360;
			glutPostRedisplay();
		}
		break;
	case 'd':
		if (LegRM <= 90) {
			LegRM = (LegRM + 5) % 360;
			glutPostRedisplay();
		}
		break;
	case'D':
		if (LegRM >= 1) {

			LegRM = (LegRM - 5) % 360;
			glutPostRedisplay();
		}
		break;

	case 'c':

		if (ArmR <= 180) {
			ArmR = (ArmR + 5) % 360;
			glutPostRedisplay();
		}
		break;
	case 'C':
		if (ArmR >= -20) {
			ArmR = (ArmR - 5) % 360;
			glutPostRedisplay();
		}
		break;

	case 'f':

		if (ArmRF <= 90) {
			ArmRF = (ArmRF + 5) % 360;
			glutPostRedisplay();                      //RotatetoFlexR
		}
		break;
	case 'F':
		if (ArmRF >= 1) {
			ArmRF = (ArmRF - 5) % 360;
			glutPostRedisplay();
		}
		break;

	case 'v':

		if (elbowR > -120) {
			elbowR = (elbowR - 5) % 360;
			glutPostRedisplay();
		}
		break;
	case 'V':
		if (elbowR <= 20) {
			elbowR = (elbowR + 5) % 360;
			glutPostRedisplay();
		}
		break;

	case 'h':
		if (head <= 90) {
			head = (head + 5) % 360;
			glutPostRedisplay();
		}
		break;
	case 'H':
		if (head >= -90) {
			head = (head - 5) % 360;
			glutPostRedisplay();
		}
		break;


		//////////////////////////////////////////

	case 'a':
		if (LegL < -90 || LegL <= 90) {

			LegL = (LegL + 5) % 360;
			glutPostRedisplay();
		}
		break;
	case 'A':
		if (LegL >= -90) {

			LegL = (LegL - 5) % 360;
			glutPostRedisplay();
		}
		break;
	case 'z':
		if (LegLM >= -90) {
			LegLM = (LegLM - 5) % 360;
			glutPostRedisplay();
		}
		break;
	case'Z':
		if (LegLM <= -1) {

			LegLM = (LegLM + 5) % 360;
			glutPostRedisplay();
		}
		break;
	case 'x':
		if (KneeL <= 90) {
			KneeL = (KneeL + 5) % 360;
			glutPostRedisplay();
		}
		break;
	case'X':
		if (KneeL >= 0) {

			KneeL = (KneeL - 5) % 360;
			glutPostRedisplay();
		}

		break;

	case 'n':
		if (ArmL >= -180) {

			ArmL = (ArmL - 5) % 360;
			glutPostRedisplay();
		}
		break;
	case 'N':
		if (ArmL < 20) {

			ArmL = (ArmL + 5) % 360;
			glutPostRedisplay();
		}
		break;

	case 'l':

		if (ArmLF >= -90) {
			ArmLF = (ArmLF - 5) % 360;
			glutPostRedisplay();                      //RotatetoFlexLeft
		}
		break;
	case 'L':
		if (ArmLF <= -1) {
			ArmLF = (ArmLF + 5) % 360;
			glutPostRedisplay();
		}
		break;

	case 'm':
		if (elbowL > -120) {
			elbowL = (elbowL - 5) % 360;
			glutPostRedisplay();
		}
		break;
	case 'M':
		if (elbowL <= 20) {
			elbowL = (elbowL + 5) % 360;
			glutPostRedisplay();
		}
		break;

		/////////////////////////////Camera Movement////////////////////////////////

	case 'k':
		moveForward();
		glutPostRedisplay();
		break;

	case 'K':
		moveBack();
		glutPostRedisplay();
		break;

	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	//glutDisplayFunc(redraw);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboard);
	createMenu();




	glutMainLoop();
	return 0;
}