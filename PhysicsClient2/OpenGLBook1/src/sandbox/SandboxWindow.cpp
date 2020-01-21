#include "SandboxWindow.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);//mouse_move
void mouse_enter_callback(GLFWwindow* window, int entered);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

SandboxWindow::SandboxWindow(GLuint width, GLuint height, GLuint count) {
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

	handler = new RetainedHandler(count);
}


SandboxWindow::~SandboxWindow() {
	//delete window;//Handled by glfwTerminate
	glfwDestroyWindow(window);
	delete handler;
}


GLboolean SandboxWindow::shouldClose() {
	return glfwWindowShouldClose(window);
}

void SandboxWindow::getWidthandHeight(GLuint& width, GLuint height) {}

void SandboxWindow::update() {
	//Save these upper variables ^

	/* disable for now to avoid doing when connected to server
	// re add once proper checks are added
	if (leftClick) {
		addServerlessP(mouseX, mouseY);
		leftClick = false;
	}
	*/

	//need to pass in a delta time?
	handler->tick(1/60.0);
}

void SandboxWindow::render() {
	glfwPollEvents();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	handler->render();
	glfwSwapBuffers(window);
}

//render client coords
void SandboxWindow::drawCoords(float x, float y) {
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

		//std::cout << x1 << ", " << y1 << ": " << x2 << ", " << y2 << ": " << x3 << ", " << y3 << std::endl;
		handler->renderer->addVertices(x1, y1);
		handler->renderer->addVertices(x2, y2);
		handler->renderer->addVertices(x3, y3);
	}
}

void  SandboxWindow::addServerlessP(float x, float y) {
	handler->addObj(new RetainedObj(x, y, glm::vec3(0,0,0), NULL, false));//GLfloat x, GLfloat y, glm::vec3 color,
	//SandboxShader* myShader, GLboolean isPlayer
}

void SandboxWindow::connectionProtocol() {

	bool finished = false;
	std::string input;
	//use stage to do a switch
	int stage = 0;

	while (!finished) {
		switch (stage) {
		case 0://Decide if serverless or servermode
			std::cout << "Would you like to connect to server? y/n" << std::endl;
			std::cout << "> ";
			std::cin >> input;
			while (input != "y" && input != "n") {
				std::cout << "Invalid input. Would you like to connect to server? y/n" << std::endl;
				std::cout << "> ";
				std::cin >> input;
			}

			if (input == "y") {//serverless mode chosen
				stage = 2;
			}

			if (input == "n") {//server mode chosen
				stage = 1;
			}

			break;

		case 1://serverless chosen
			std::cout << "Running program serverless." << std::endl;
			finished = true;
			break;


		case 2:
			std::cout << "Connect to server using TCP(1) or UDP(2)?" << std::endl;
			std::cout << "> ";
			std::cin >> input;
			while (input != "1" && input != "2") {
				std::cout << "Invalid input. Connect to server using TCP(1) or UDP(2)?" << std::endl;
				std::cout << "> ";
				std::cin >> input;
			}

			if (input == "1") {//TCP mode chosen
				stage = 3;
			}

			if (input == "2") {//UDP mode chosen
				stage = 4;
			}
			break;

		case 3:
			std::cout << "Attempting to connect to a TCP server..." << std::endl;
			
			//do connection

			//failed to connect
			std::cout << "Failed to connect to a TCP server. Would you like to try again? y/n" << std::endl;
			std::cout << "> ";
			std::cin >> input;
			while (input != "y" && input != "n") {
				std::cout << "Invalid input. Failed to connect to a TCP server. Would you like to try again? y/n" << std::endl;
				std::cout << "> ";
				std::cin >> input;
			}

			if (input == "y") {
				stage = 3;
			}

			if (input == "n") {
				stage = 0;
			}

			break;

		case 4:
			std::cout << "Attempting to connect to a UDP server..." << std::endl;

			//do connection

			//failed to connect
			std::cout << "Failed to connect to a UDP server. Would you like to try again? y/n" << std::endl;
			std::cout << "> ";
			std::cin >> input;
			while (input != "y" && input != "n") {
				std::cout << "Invalid input. Failed to connect to a UDP server. Would you like to try again? y/n" << std::endl;
				std::cout << "> ";
				std::cin >> input;
			}

			if (input == "y") {
				stage = 4;
			}
			if (input == "n") {
				stage = 0;
			}
			break;

		default:
			break;
		}
	}

	/*
	std::cout << "Would you like to connect to server? y/n" << std::endl;

	std::cin >> input;

	while (input != "y" && input != "n") {
		std::cout << "Invalid input. Would you like to connect to server? y/n" << std::endl;
		std::cin >> input;
	}

	if (input == "y") {
		//ask if udp or tcp
		serverMode = true;
		std::cout << "Connect to server using TCP(1) or UDP(2)?" << std::endl;
		std::cin >> input;
		while (input != "1" && input != "2") {
			std::cout << "Invalid input. Connect to server using TCP(1) or UDP(2)?" << std::endl;
			std::cin >> input;
		}

		if (input == "1") {
			TCP = true;
			//Connection con.tcpstuff

			//if fail connection
			std::cout << "Failed to connect to a TCP server. Would you like to try again? y/n";
			std::cin >> input;

			while (input != "y" && input != "n") {
				std::cout << "Invalid input. Failed to connect to a TCP server. Would you like to try connecting again? y/n" << std::endl;
				std::cin >> input;
			}

			if (input == "y") {
				//try again
				//connectionProtocol(); //cons of calling this method and have retrun in next line? 
			}

			if (input == "n") {
				//start process again
			}
		}

		if (input == "2") {
			TCP = false;
			//Connection con...UPD stuff

			//if fail connection
			std::cout << "Failed to connect to a UDP server. Would you like to try again? y/n";
			std::cin >> input;

			while (input != "y" && input != "n") {
				std::cout << "Invalid input. Failed to connect to a UDP server. Would you like to try connecting again? y/n" << std::endl;
				std::cin >> input;
			}

			if (input == "y") {
				//try again
			}

			if (input == "n") {
				//start this process over
			}
		}
		//set connection
	}

	if (input == "n") {
		serverMode = false;
	}
	*/
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//set pointer and get pointer of handler for reference
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	//std::cout << ((SandboxWindow*)glfwGetWindowUserPointer(window))->handler->keys << std::endl;

	if (action == GLFW_PRESS) {
		((SandboxWindow*)glfwGetWindowUserPointer(window))->handler->keys[key] = true;
	} else if (action == GLFW_RELEASE) {
		((SandboxWindow*)glfwGetWindowUserPointer(window))->handler->keys[key] = false;
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	//get width and height
	int winHeight;
	int winWidth;

	glfwGetWindowSize(window, &winWidth, &winHeight);

	//std::cout << "Mouse adj: " << (2.0 * xpos / winWidth) - 1.0 << ", " << 1.0 - (2.0 * ypos / winHeight) << std::endl;
	((SandboxWindow*)glfwGetWindowUserPointer(window))->mouseX = ((2.0 * xpos / winWidth) - 1.0);
	((SandboxWindow*)glfwGetWindowUserPointer(window))->mouseY = (1.0 - (2.0 * ypos / winHeight));
}

void mouse_enter_callback(GLFWwindow* window, int entered) {}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		((SandboxWindow*)glfwGetWindowUserPointer(window))->leftClick = true;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		((SandboxWindow*)glfwGetWindowUserPointer(window))->leftClick = false;
	}
}

