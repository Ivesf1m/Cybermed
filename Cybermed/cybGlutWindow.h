#pragma once

#include <gl\glew.h>
#include <gl\freeglut.h>
#include <string>

using std::string;

class CybGlutWindow
{
public:
	CybGlutWindow(unsigned int width = 600, unsigned int height = 600,
		unsigned int windowPositionX = 100, unsigned int windowPostionY = 100,
		string windowName = "");
	~CybGlutWindow();

	void createGLContext(int* argc, char** argv);
	void initialize();
	void display();
	void mainLoop();
	void moveWindow(unsigned int x, unsigned int y);
	void reshapeWindow(int width, int height);

	//getters and setters
	unsigned int getHeight();
	void setHeight(unsigned int height);

	unsigned int getWidth();
	void setWidth(unsigned int width);

	string getWindowName();
	void setWindowName(string name);

	unsigned int getWindowPositionX();
	void setWindowPositionX(unsigned int x);

	unsigned int getWindowPositionY();
	void setWindowPositionY(unsigned int y);

	static void displayFunction();
	static void setCurrentWindow(CybGlutWindow* window);

private:
	static CybGlutWindow* currentWindow;
	unsigned int width;
	unsigned int height;
	unsigned int windowPositionX;
	unsigned int windowPositionY;
	string windowName;
	bool contextCreated;
};
