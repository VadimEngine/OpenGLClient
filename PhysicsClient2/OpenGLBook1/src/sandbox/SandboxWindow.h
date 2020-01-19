#pragma once
#define windowMode 1 // 0 = immediate, 1 = retained

#ifndef windowMode
#define THEHANDLER ImmediateHandler
#elif windowMode==0
#define THEHANDLER ImmediateHandler
#elif windowMode==1
#define THEHANDLER RetainedHandler
#endif

#include <iostream>

#ifndef windowMode
#include "ImmediateHandler.h"
#elif windowMode==0
#include "Immediate/ImmediateHandler.h"
#elif windowMode==1
#include "RetainedHandler.h"
#endif


class SandboxWindow {
	
public:

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	GLfloat tempFrame = 0;
	GLfloat tempTotal = 0;

	GLboolean leftClick;

	GLfloat mouseX;//-1.0 to 1.0
	GLfloat mouseY;//Vector?

	GLFWwindow* window;
	THEHANDLER* handler;

	SandboxWindow(GLuint width, GLuint height, GLuint count);
	~SandboxWindow();

	GLboolean shouldClose();

	void getWidthandHeight(GLuint& width, GLuint height);

	void update();

	void render();

	void drawCoords(float arr[20]);

	void drawCoords(float x, float y);

	void addServerlessP(float x, float y);

	//set callback, to point to this object
};