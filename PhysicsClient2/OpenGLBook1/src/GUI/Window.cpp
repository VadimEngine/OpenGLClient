#include "Window.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);//mouse_move
void mouse_enter_callback(GLFWwindow* window, int entered);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

Window::Window(GLuint width, GLuint height, GLuint count) {
	//initilize glfw (graphics library framework)
	if (!glfwInit()) {
		std::cout << "ERROR" << std::endl;
	} else {
		std::cout << "SUCCESS" << std::endl;
	}

	glfwInit();
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(width, height, "OpenGLSandbox", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwSetWindowUserPointer(window, this);
	//window->set

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetCursorEnterCallback(window, mouse_enter_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initalize GLEW" << std::endl;
		glfwTerminate();
	}

	int tempWidth, tempHeight;
	glfwGetFramebufferSize(window, &tempWidth, &tempHeight);
	glViewport(0, 0, tempWidth, tempHeight);//specify lower left corner

	handler = new Handler(count);
	handler->isServer = serverMode;
}


Window::~Window() {
	//delete window;//Handled by glfwTerminate
	glfwDestroyWindow(window);
	delete handler;
}


GLboolean Window::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Window::getWidthandHeight(GLuint& width, GLuint height) {}

void Window::update() {
	//Save these upper variables ^
	if (!serverMode) {
		if (leftClick) {
			addServerlessP(mouseX, mouseY);
			leftClick = false;
		}
	}

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

//render client coords
void Window::drawCoords(float x, float y) {
	float radius = 0.05f;
	float sides = 20;
	double pi = 3.14159265358979323846;

	for (int j = 0; j < sides; j++) {
		GLfloat x1 = x;
		GLfloat y1 = y;

		GLfloat x2 = x + cos((2 * pi / (float)sides) * (float)j) * radius;
		GLfloat y2 = y + sin((2 * pi / (float)sides) * (float)j) * radius;

		GLfloat x3 = x + cos((2 * pi / (float)sides) * (float)(j + 1)) * radius;
		GLfloat y3 = y + sin((2 * pi / (float)sides) * (float)(j + 1)) * radius;

		handler->renderer->addVertices(x1, y1);
		handler->renderer->addVertices(x2, y2);
		handler->renderer->addVertices(x3, y3);
	}
}

void Window::addServerlessP(float x, float y) {
	handler->addObj(new GameObject(x, y, false));//GLfloat x, GLfloat y, glm::vec3 color,
	//SandboxShader* myShader, GLboolean isPlayer
}

void Window::setServer(bool isServer) {
	handler->isServer = isServer;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//set pointer and get pointer of handler for reference
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	if (action == GLFW_PRESS) {
		((Window*)glfwGetWindowUserPointer(window))->handler->keys[key] = true;
	} else if (action == GLFW_RELEASE) {
		((Window*)glfwGetWindowUserPointer(window))->handler->keys[key] = false;
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	//get width and height
	int winHeight;
	int winWidth;

	glfwGetWindowSize(window, &winWidth, &winHeight);

	((Window*)glfwGetWindowUserPointer(window))->mouseX = ((2.0 * xpos / winWidth) - 1.0);
	((Window*)glfwGetWindowUserPointer(window))->mouseY = (1.0 - (2.0 * ypos / winHeight));
}

void mouse_enter_callback(GLFWwindow* window, int entered) {}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		((Window*)glfwGetWindowUserPointer(window))->leftClick = true;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		((Window*)glfwGetWindowUserPointer(window))->leftClick = false;
	}
}
