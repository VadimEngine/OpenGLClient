#pragma once

#include <iostream>

#include "../Engine/Handler.h"


class Window {
	
public:

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	GLfloat tempFrame = 0;
	GLfloat tempTotal = 0;

	GLboolean leftClick;

	GLfloat mouseX;//-1.0 to 1.0
	GLfloat mouseY;//Vector?

	GLFWwindow* window;
	Handler* handler;
	//Connection connection;
	bool serverMode;
	bool TCP;

	Window(GLuint width, GLuint height, GLuint count);
	~Window();

	GLboolean shouldClose();

	void getWidthandHeight(GLuint& width, GLuint height);

	void update();

	void render();

	void drawCoords(float x, float y);

	void addServerlessP(float x, float y);

	void setServer(bool isServer);
};