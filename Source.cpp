#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

static int ArmR = 0, ArmRF = 0, ArmLF = 0, ArmL = 0, elbowR = 0, elbowL = 0;
static int LegR = 0, LegL = 0, LegRM=0, LegLM=0, KneeR=0,KneeL=0;
static int fingerBaseR1 = 0, fingerBaseR2 = 0, fingerBaseR3 = 0, fingerBase4R = 0, fingerBase5R = 0;
static int fingerBaseL1 = 0, fingerBaseL2 = 0, fingerBaseL3 = 0, fingerBase4L = 0, fingerBase5L = 0;
static int Body = 0, head = 0;

static int moving, startx, starty;
GLfloat angle = 0.0;   /* in degrees */
GLfloat angle2 = 0.0;   /* in degrees */

double vector[] = {0.0,0.0,0.0};
double eye[] = { 0.0, 0.0, 7.0 };
double center[] = { 0.0, 0.0, 0.0};
double look[] = { eye[0]-center[0], eye[1]-center[1],eye[2]-center[2] };
double up[] = { 0.0, 1.0, 0.0 };


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

	temp[0] += (1 - cos(theta))*(a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
	temp[1] += (1 - cos(theta))*(a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
	temp[2] += (1 - cos(theta))*(a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

	temp[0] += cos(theta)*p[0];
	temp[1] += cos(theta)*p[1];
	temp[2] += cos(theta)*p[2];

	p[0] = temp[0];
	p[1] = temp[1];
	p[2] = temp[2];
}

void Left()
{
	// implement camera rotation arround vertical window screen axis to the left
	// used by mouse and left arrow
	double angle = 0.05;
	double direction[] = { 0, 1, 0 };
	rotatePoint(direction, angle, eye);


}

void Right()
{
	// implement camera rotation arround vertical window screen axis to the right
	// used by mouse and right arrow

	double angle = -0.05;
	double direction[] = { 0, 1, 0 };
	rotatePoint(direction, angle, eye);
	

}

void Up()
{
	// implement camera rotation arround horizontal window screen axis +ve
	// used by up arrow

	double angle = -0.05;
	double look[] = { eye[0] - center[0], eye[1] - center[1],eye[2] - center[2] };

	crossProduct(up, look, vector);
	normalize(vector);
	rotatePoint(vector, angle, eye);
	rotatePoint(vector, angle, up);

}

void Down()
{
	// implement camera rotation arround horizontal window screen axis 
	// used by down arrow

	double angle = 0.05 ;
	double look[] = { eye[0] - center[0], eye[1] - center[1],eye[2] - center[2] };


	crossProduct(up, look, vector);
	normalize(vector);
	rotatePoint(vector, angle, eye);
	rotatePoint(vector, angle, up);
}

void moveForward()
{

	double speed = 0.05;
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	normalize(direction);

	eye[0] += direction[0] * speed;
	eye[2] += direction[2] * speed;

	center[0] += direction[0] * speed;
	center[2] += direction[2] * speed;

}

void moveBack()
{

	double speed = -0.05;
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];

	normalize(direction);


	eye[0] += direction[0] * speed;
	eye[2] += direction[2] * speed;

	center[0] += direction[0] * speed;
	center[2] += direction[2] * speed;

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

void LegLeft(void)
{
	glPushMatrix();

	glTranslatef(0.0, -0.5, 0.0);
	glRotatef((GLfloat)LegL, 1.0, 0.0, 0.0);
	glRotatef((GLfloat)LegLM, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -0.5, 0.0);


	glPushMatrix();
	glTranslatef(-0.3, 0.0, 0.0);   //LegRight
	glScalef(0.4, 1.0, 0.3);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0.0, -0.5, 0.0);
	glRotatef((GLfloat)KneeL, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -0.5, 0.0);

	glPushMatrix();
	glTranslatef(-0.3, -0.7, 0.0);   //FootRight
	glScalef(0.4, 0.4, 0.3);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3, 0.0, 0.0);   //KneeRigth
	glScalef(0.4, 1.0, 0.3);
	glutWireCube(1);
	glPopMatrix();

	glPopMatrix();

}
void ArmLeft(void)
{
	glPushMatrix();


	glTranslatef(-0.75, 1.5, 0.0);
	glRotatef((GLfloat)ArmL, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)ArmLF, 0.0, 1.0, 0.0);
	glTranslatef(0.75, -1.5, 0.0);

	glPushMatrix();
	glTranslatef(-0.75, 1.0, 0.0);   //ShoulderLeft
	glScalef(0.3, 1.0, 0.3);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0.0, 0.5, 0.0);
	glRotatef((GLfloat)elbowL, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.5, 0.0);

	glPushMatrix();
	glTranslatef(-0.75, -1,0.0);   //Elbowleft
	glScalef(0.3, 1.0, 0.3);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8705, -1.6, -0.1);                   //fingerbase5rLeft
	glScalef(0.075, 0.2, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8705, -1.6, 0.1);                   //fingerbase4rLeft
	glScalef(0.075, 0.2, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7945, -1.6, 0.1);                   //fingerbase3rLeft
	glScalef(0.075, 0.2, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7215, -1.6, 0.1);                   //fingerbase2rLeft
	glScalef(0.075, 0.2, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();

	glTranslatef(-0.6475, -1.6, 0.1);   
	                                                                                                         //fingerbase1Left
	glScalef(0.075, 0.2, 0.1);
	glutWireCube(1);
	glPopMatrix();

	/////////////////////

	glPushMatrix();
	glTranslatef(-0.8705, -1.75, -0.1);                   //finger5rLeft
	glScalef(0.075, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.8705, -1.75, 0.1);                   //finger4rLeft
	glScalef(0.075, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7945, -1.75, 0.1);                   //finger3Left
	glScalef(0.075, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.7215, -1.75, 0.1);                   //finger2Left
	glScalef(0.075, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6475, -1.75, 0.1);                   //finger1Left
	glScalef(0.075, 0.1, 0.1);
	glutWireCube(1);
	glPopMatrix();

	glPopMatrix();


}
////////////////////////////////////////////////////////Right/////////////////////////////////////

void LegRight(void)
{
	
	glPushMatrix();
	
	glTranslatef(0.0, -0.5, 0.0);
	glRotatef((GLfloat)LegR, 1.0, 0.0, 0.0);
	glRotatef((GLfloat)LegRM, 0.0, 0.0, 1.0);
	glTranslatef(0.0, -0.5, 0.0);


	glPushMatrix();
	glTranslatef(0.3, 0.0, 0.0);   //LegRight
	glScalef(0.4, 1.0, 0.3);
	glutWireCube(1);
	glPopMatrix();

	glTranslatef(0.0, -0.5, 0.0);
	glRotatef((GLfloat)KneeR, 1.0, 0.0, 0.0);
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
void ArmRight(void)
{
	glPushMatrix();

	    glTranslatef(0.75, 1.5, 0.0);	
	    glRotatef((GLfloat)ArmR, 0.0, 0.0, 1.0);
		glRotatef((GLfloat)ArmRF, 0.0, 1.0, 0.0);
		glTranslatef(-0.75, -1.5, 0.0);

		glPushMatrix();
		glTranslatef(0.75, 1.0, 0.0);   //ShoulderRight
		glScalef(0.3, 1.0, 0.3);
		glutWireCube(1);
		glPopMatrix();


		glTranslatef(0.0, 0.5, 0.0);
		glRotatef((GLfloat)elbowR, 1.0, 0.0, 0.0);
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
		glTranslatef(0.0, 0.5, 0.0);   //Chest
		glScalef(1.0, 2.0, 0.5);
		glutWireCube(1);
		glPopMatrix();

	}
	void Head(void)
	{
		glPushMatrix();
		glRotatef((GLfloat)head, 0.0, 1.0, 0.0);

		glPushMatrix();
		glTranslatef(0.0, 2.0, 0.0);          
		glutWireSphere(0.3, 16, 16);
		glPopMatrix();

		glPopMatrix();

	}

	void display(void)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

		glPushMatrix();
		glRotatef(angle2, 0.0, 1.0, 0.0);
		glRotatef(angle, 0.0, 1.0, 0.0);

		glRotatef((GLfloat)Body, 0.0, 1.0, 0.0);

		Chest();

		glPushMatrix();
		ArmRight();
		glPopMatrix();

		glPushMatrix();
		ArmLeft();
		glPopMatrix();

		LegRight();
		LegLeft();
		Head();
		glutSwapBuffers();

		glPopMatrix();
	}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
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
		if (LegR < -90 || LegR<= 90 ) {

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
		if ( LegRM <= 90) {
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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}