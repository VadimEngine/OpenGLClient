#include "Window.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_enter_callback(GLFWwindow* window, int entered);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);


Window::Window(GLuint width, GLuint height, GLuint count) {
	//initialize glfw (graphics library framework)
	if (!glfwInit()) {
		std::cout << "ERROR" << std::endl;
	} else {
		std::cout << "SUCCESS" << std::endl;
	}

	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(width, height, "OpenGLSandbox", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwSetWindowUserPointer(window, this);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetCursorEnterCallback(window, mouse_enter_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
	}

	int tempWidth, tempHeight;
	glfwGetFramebufferSize(window, &tempWidth, &tempHeight);
	glViewport(0, 0, tempWidth, tempHeight);//specify lower left corner

	handler = new Handler();
}

Window::~Window() {
	//delete window;//Handled by glfwTerminate
	glfwDestroyWindow(window);
	delete handler;
}

GLboolean Window::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Window::update() {
	//need to pass in an accurate delta time?
	handler->tick(1/60.0);
}

void Window::render() {
	glfwPollEvents();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	handler->render();
	glfwSwapBuffers(window);
}

void Window::closeConnection() {
	if (handler->getConnection()->getMode() != ConnectionMode::SERVERLESS) {
		handler->getConnection()->close();
	}
}

Handler* Window::getHandler() {
	return handler;
}


//Call back functions***********************************************************************************

/// <summary>
/// Key callback that sets the boolean array in handler that represents keys.
///	weird GLWF issue where it detects only capital keys, to set the lower case,
///	need to confirm shift and/or caplock is not pressed?
/// </summary>
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			//((Window*)glfwGetWindowUserPointer(window))->getHandler()->keys[key] = true;
			((Window*)glfwGetWindowUserPointer(window))->getHandler()->setKeyPress(key, true);

		} else if (action == GLFW_RELEASE) {
			//((Window*)glfwGetWindowUserPointer(window))->getHandler()->keys[key] = false;
			((Window*)glfwGetWindowUserPointer(window))->getHandler()->setKeyPress(key, false);

		}
	}
}

/// <summary>
/// Call back action that sets mousePosition of this window
/// </summary
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	//get width and height
	int winHeight;
	int winWidth;

	glfwGetWindowSize(window, &winWidth, &winHeight);

	GLfloat mx = ((2.0f * xpos / winWidth) - 1.0f);
	GLfloat my = (1.0 - (2.0 * ypos / winHeight));
	//((Window*)glfwGetWindowUserPointer(window))->getHandler()->mouseCoords = glm::vec2(mx, my);
	((Window*)glfwGetWindowUserPointer(window))->getHandler()->setMousePosition(glm::vec2(mx, my));

}

/// <summary>
/// Mouse enter callback. Not used at the moment but can be added to 
/// future functionality that require knowing if mouse is in frame or not
/// </summary>
void mouse_enter_callback(GLFWwindow* window, int entered) {}

/// <summary>
///  Mouse click call back, sets left click in window and handler
/// </summary>
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//((Window*)glfwGetWindowUserPointer(window))->getHandler()->leftClick = true;
		((Window*)glfwGetWindowUserPointer(window))->getHandler()->setLeftClick(true);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		//((Window*)glfwGetWindowUserPointer(window))->getHandler()->leftClick = false;
		((Window*)glfwGetWindowUserPointer(window))->getHandler()->setLeftClick(false);

	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		//((Window*)glfwGetWindowUserPointer(window))->getHandler()->rightClick = true;
		((Window*)glfwGetWindowUserPointer(window))->getHandler()->setRightClick(true);

	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		//((Window*)glfwGetWindowUserPointer(window))->getHandler()->rightClick = false;
		((Window*)glfwGetWindowUserPointer(window))->getHandler()->setRightClick(false);

	}
}