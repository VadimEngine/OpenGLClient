#pragma once

#include <iostream>

#include "RetainedHandler.h"
//#include "Connection.h"


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
	RetainedHandler* handler;
	//Connection connection;
	bool serverMode;
	bool TCP;

	SandboxWindow(GLuint width, GLuint height, GLuint count);
	~SandboxWindow();

	GLboolean shouldClose();

	void getWidthandHeight(GLuint& width, GLuint height);

	void update();

	void render();

	void drawCoords(float arr[20]);

	void drawCoords(float x, float y);

	void addServerlessP(float x, float y);

	void connectionProtocol();

	//set callback, to point to this object
};