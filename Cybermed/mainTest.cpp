#include "cybGlutWindow.h"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;


int main(int argc, char** argv)
{
	CybGlutWindow window;
	window.createGLContext(&argc, argv);
	CybGlutWindow::setCurrentWindow(&window);
	window.mainLoop();

	return 0;
}