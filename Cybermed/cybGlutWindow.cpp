#include "cybGlutWindow.h"
#include <iostream>

using std::cerr;
using std::endl;

CybGlutWindow* CybGlutWindow::currentWindow = NULL;

CybGlutWindow::CybGlutWindow(unsigned int width, unsigned int height, 
	unsigned int windowPositionX, unsigned int windowPostionY,
	string windowName)
	: width(width), height(height), windowPositionX(windowPositionX),
	windowPositionY(windowPostionY), windowName(windowName),
	contextCreated(false)
{

}

CybGlutWindow::~CybGlutWindow()
{

}

void CybGlutWindow::createGLContext(int* argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(windowPositionX, windowPositionY);
	glutInitWindowSize(width, height);
	glutCreateWindow(windowName.c_str());

	if (glewInit() != GLEW_OK) {
		cerr << "GLEW context not properly initialized." << endl;
		exit(1);
	}

	contextCreated = true;

	initialize();
	glutDisplayFunc(CybGlutWindow::displayFunction);
}

void CybGlutWindow::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

void CybGlutWindow::displayFunction()
{
	if (CybGlutWindow::currentWindow)
		CybGlutWindow::currentWindow->display();
	else {
		cerr << "Display function callback failed. Window instance is not valid"
			<< endl;
	}
}

void CybGlutWindow::initialize()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void CybGlutWindow::mainLoop()
{
	if(contextCreated)
		glutMainLoop();
	else
		cerr << "Failed to start main loop. Context not created." << endl;
}

void CybGlutWindow::moveWindow(unsigned int x, unsigned int y)
{
	glutPositionWindow(x, y);
}

void CybGlutWindow::reshapeWindow(int width, int height)
{
	glutReshapeWindow(width, height);
}

void CybGlutWindow::setCurrentWindow(CybGlutWindow* window)
{
	CybGlutWindow::currentWindow = window;
}

//getters and setters
unsigned int CybGlutWindow::getHeight()
{
	return height;
}

void CybGlutWindow::setHeight(unsigned int height)
{
	this->height = height;
	reshapeWindow(width, height);
}

unsigned int CybGlutWindow::getWidth()
{
	return width;
}

void CybGlutWindow::setWidth(unsigned int width)
{
	this->width = width;
	reshapeWindow(width, height);
}

string CybGlutWindow::getWindowName()
{
	return windowName;
}

void CybGlutWindow::setWindowName(string name)
{
	this->windowName = name;
	glutSetWindowTitle(name.c_str());
}

unsigned int CybGlutWindow::getWindowPositionX()
{
	return windowPositionX;
}

void CybGlutWindow::setWindowPositionX(unsigned int x)
{
	this->windowPositionX = x;
	moveWindow(windowPositionX, windowPositionY);
}

unsigned int CybGlutWindow::getWindowPositionY()
{
	return windowPositionY;
}

void CybGlutWindow::setWindowPositionY(unsigned int y)
{
	this->windowPositionY = y;
	moveWindow(windowPositionX, windowPositionY);
}