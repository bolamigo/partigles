// Application.cpp�: d�finit le point d'entr�e pour l'application console.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <ctime>
#include <string>

#include "Quantum.h"
#include "GxWorld.h"
#include "GxParticle.h"
#include "GxUpdater.h"
#include "QmDrag.h"
#include "QmForceRegistry.h"
#include <QmMagnetism.h>

constexpr int ESC_KEY = 27;

constexpr glm::vec3 WHITE_VEC3 = glm::vec3(1.f, 1.f, 1.f);
constexpr glm::vec3 BLACK_VEC3 = glm::vec3(0.f, 0.f, 0.f);
constexpr glm::vec3 RED_VEC3 = glm::vec3(1.f, 0.f, 0.f);
constexpr glm::vec3 GREEN_VEC3 = glm::vec3(0.f, 1.f, 0.f);
constexpr glm::vec3 BLUE_VEC3 = glm::vec3(0.f, 0.f, 1.f);

using namespace std;
using namespace Quantum;

GxWorld graphicWorld;
QmWorld physicsWorld;

glm::vec3* mousePointer;

int scene = 0;

// ********************** GLUT 
// Variables globales

int SCREEN_X = 800;
int SCREEN_Y = 800;
int VIEWPORT_X = 5; int VIEWPORT_Y = 5; int VIEWPORT_Z = 200;

GLuint DrawListSphere;

int buttons = -1;
float cameraRotate = 90.f;
float cameraDistance = 30.f;
float cameraHeight = 0.f;
float mouseX = 0.f;
float mouseY = 0.f;

// Variables du calcul de framerate 
int timeold = 0;
int frame = 0;
float accTime = 0.0;
bool paused = false;

GLfloat light_pos[] = { 0.0, 6.0, 1.0, 1.0 };

// ********************************************


glm::vec3 randomVector(float min, float max)
{
	float x = min + (max - min)*((rand() % 100) / 100.f);
	float y = min + (max - min)*((rand() % 100) / 100.f);
	float z = min + (max - min)*((rand() % 100) / 100.f);
	return glm::vec3(x, y, z);
}

glm::vec3 randomFountainVelocityVector()
{
	float x, y, z;
	glm::vec3 location, center = glm::vec3(-12, 4, 0);
	do {
		x = -16 + 8 * ((rand() % 100) / 100.f);
		y = 8 * ((rand() % 100) / 100.f);
		z = -4 + 8 * ((rand() % 100) / 100.f);
		location = glm::vec3(x, y, z);
	} while (distance(center, location) > 4); // sphere instead of cube
	return location;
}

float randomFloat(float min, float max) {
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

QmParticle* createParticle(bool fountain = false)
{
    glm::vec3 pos = *mousePointer;
	glm::vec3 vel = fountain ? randomFountainVelocityVector() : randomVector(-8, 8);
    
    QmParticle* particle = new QmParticle(pos, vel, glm::vec3(0));
	float radius = randomFloat(.1, .4);
	particle->setMass(radius*radius);
	GxParticle* graphicsParticle = new GxParticle(randomVector(0, 1), radius, pos);
    particle->setUpdater(new GxUpdater(graphicsParticle));
    
    graphicWorld.addParticle(graphicsParticle);
    physicsWorld.addBody(particle);

	// Random drag force
	float K1 = randomFloat(.005f, .02f);
	float K2 = randomFloat(.01f, .06f);
	QmDrag* dragForce = new QmDrag(K1, K2);
	QmForceRegistry* dragRegistry = new QmForceRegistry(particle, dragForce);
	physicsWorld.addForceRegistry(dragRegistry);

    return particle;
}

void initScene1()
{
	scene = 1;
	printf("Scene 1: Particles\n");
	mousePointer = new glm::vec3(0, 0, 0);
	for (int i = 0; i < 100; i++)
		createParticle();
}

void initScene2()
{
	scene = 2;
	printf("Scene 2: Magnetic particles\n");
	mousePointer = new glm::vec3(0, 0, 0);

	const int numParticles = 100;
	for (int i = 0; i < numParticles; ++i) {
		glm::vec3 position = randomVector(-5, 5);
		glm::vec3 velocity = randomVector(-2, 2);

		QmParticle* p = new QmParticle(position, velocity, glm::vec3(0));

		if (i < numParticles / 2) {
			p->setCharge(1.f); // Positive charge
			GxParticle* g = new GxParticle(RED_VEC3, .2f, position);
			p->setUpdater(new GxUpdater(g));
			graphicWorld.addParticle(g);
		}
		else {
			p->setCharge(-1.f); // Negative charge
			GxParticle* g = new GxParticle(BLUE_VEC3, .2f, position);
			p->setUpdater(new GxUpdater(g));
			graphicWorld.addParticle(g);
		}

		physicsWorld.addBody(p);

		QmMagnetism* magnetism = new QmMagnetism();
		QmForceRegistry* magneticForceRegistry = new QmForceRegistry(p, magnetism);
		physicsWorld.addForceRegistry(magneticForceRegistry);
	}
}

// ***************************** GLUT methods

void resetView()
{
	cameraRotate = 90.f;
	cameraDistance = 30.f;
	cameraHeight = 0.f;
	mouseX = 0.f;
	mouseY = 0.f;
	if (mousePointer) {
		delete mousePointer;
		mousePointer = NULL;
	}
}

void createDisplayListSphere()
{
	DrawListSphere = glGenLists(1);
	glNewList(DrawListSphere, GL_COMPILE);
	GLUquadricObj* sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, 1, 12, 8);
	glEndList();
}

void initLight()
{
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);

	GLfloat ambient_light[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuse_light[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);
}

void calculateFPS(float dt)
{
	accTime += dt;

	if (accTime > 0.5)
	{
		char t[200];
		sprintf_s(t, "Quantum Physics Engine: Scene %d, %.2f FPS", scene, frame / accTime);
		glutSetWindowTitle(t);
		accTime = 0.0;
		frame = 0;
	}
}

void idleFunc()
{
	int timer = glutGet(GLUT_ELAPSED_TIME);
	float dt = (float)(timer - timeold) / 1000.f;
	timeold = timer;

	calculateFPS(dt);

	if (buttons == GLUT_LEFT_BUTTON) {
		createParticle(true); // fountain = true
	}

	if (!paused) physicsWorld.simulate(dt);

	glutPostRedisplay();
}

static void renderText(float x, float y, const std::string& text, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f)) {
	glColor3f(color.r, color.g, color.b);
	glRasterPos2f(x, y);

	for (char c : text)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}

void drawFunc()
{
	frame++;
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraDistance * cos(cameraRotate), cameraHeight, cameraDistance * sin(cameraRotate), 0, cameraHeight, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

	for (GxParticle* p : graphicWorld.getParticles())
	{
		glPushMatrix();
		glm::vec3 color = p->getColor();
		glColor3f(color.x, color.y, color.z);
		glTranslatef(p->getPos().x, p->getPos().y, p->getPos().z);
		glScaled(p->getRadius(), p->getRadius(), p->getRadius());
		glCallList(DrawListSphere);
		glPopMatrix();
	}
	if (mousePointer) {
		glPushMatrix();
		glColor3f(1.f, 1.f, 1.f);
		glTranslatef(mousePointer->x, mousePointer->y, mousePointer->z);
		glScaled(.1f, .1f, .1f);
		glCallList(DrawListSphere);
		glPopMatrix();
	}

	// Controls
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_X, 0, SCREEN_Y);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	renderText(8, SCREEN_Y - 20, "ESC - Exit", WHITE_VEC3);
	renderText(8, SCREEN_Y - 40, "Toggles: (P)ause, (G)ravity, (D)rag", WHITE_VEC3);

	// Status (togglable options)
	glm::vec3 pauseColor = paused ? GREEN_VEC3 : RED_VEC3;
	renderText(8, SCREEN_Y - 60, "PAUSE", pauseColor);
	glm::vec3 gravityColor = physicsWorld.isGravityOn() ? GREEN_VEC3 : RED_VEC3;
	renderText(100, SCREEN_Y - 60, "GRAVITY", gravityColor);
	glm::vec3 dragColor = QmDrag::isOn() ? GREEN_VEC3 : RED_VEC3;
	renderText(210, SCREEN_Y - 60, "DRAG", dragColor);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glutSwapBuffers();
}

void locateMouseCursor(int x, int y)
{
	*mousePointer = glm::vec3((x - SCREEN_X / 2.f) / 50.f, -(y - SCREEN_Y / 2.f) / 50.f, 0);
}

void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		locateMouseCursor(x, y);
		buttons = GLUT_LEFT_BUTTON;
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) buttons = GLUT_MIDDLE_BUTTON;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) buttons = GLUT_RIGHT_BUTTON;
	if (button == 3) cameraDistance /= 1.07f;
	if (button == 4) cameraDistance *= 1.07f;
	if (state == GLUT_UP) buttons = -1;

	mouseX = (float)x;
	mouseY = (float)y;
}

void motionFunc(int x, int y)
{
	if (buttons == GLUT_LEFT_BUTTON)
		locateMouseCursor(x, y);

	if (buttons == GLUT_RIGHT_BUTTON) {
		cameraRotate += (x - mouseX) / 100.f;
		cameraHeight += (y - mouseY) * cameraDistance / 1000.f;
	}

	mouseX = (float)x;
	mouseY = (float)y;
}

void clearWorld()
{
	graphicWorld.clear();
	physicsWorld.clear();
}

void toggleScene(int sceneNumber)
{
	clearWorld();
	resetView();

	switch (sceneNumber)
	{
	case 1: initScene1(); break;
	case 2: initScene2(); break;
	}
}

void keyFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ESC_KEY:
		clearWorld();
		glDeleteLists(DrawListSphere, 1);
		exit(0);
		break;
	case '1':
		toggleScene(1);
		break;
	case '2':
		toggleScene(2);
		break;
	case 'p': case 'P':
		cout << ((paused = !paused) ? "" :"un") << "pausing" << endl;
		break;
	case 'g': case 'G':
		physicsWorld.toggleGravity();
		break;
	case 'd': case 'D':
		QmDrag::switchState();
		break;
	default:
		break;
	}
}

void initGraphics(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_X, SCREEN_Y);
	glutCreateWindow("Quantum Demo");

	glutIdleFunc(idleFunc);
	glutDisplayFunc(drawFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutKeyboardFunc(keyFunc);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_PROGRAM_POINT_SIZE);
	//glPointSize(5);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect = SCREEN_X / (float)SCREEN_Y;
	gluPerspective(45.0, aspect, 1.0, VIEWPORT_Z);
	glViewport(0, 0, SCREEN_X, SCREEN_Y);
	createDisplayListSphere();
	initLight();
}

// ************************** end GLUT methods

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	initGraphics(argc, argv);

	toggleScene(1);

	glutMainLoop();
	return 0;
}



