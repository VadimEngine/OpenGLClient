#include "Handler.h"


Handler::Handler()
	:keys{}, leftClick(false) {
	gameHandler = nullptr;
	connect = new Connection();
	connect->setMode(ConnectionMode::SERVERLESS);
	mouseCoords = glm::vec2(0,0);
	//player = nullptr;
	Shader* polygonShader = new Shader("src/shaders/sandbox.vert",
									"src/shaders/sandbox.frag");//delete in destructor

	Shader* imageShader = new Shader("src/shaders/image.vert",
							   	  "src/shaders/image.frag");//delete in destructor

	renderer = new Renderer(polygonShader, imageShader);
	
	currentPage = new IntroPage();
	rightClick = false;
}

Handler::~Handler() {
	delete currentPage;
	delete renderer;

	if (gameHandler != nullptr) {
		delete gameHandler;
	}
	connect->close();
	delete connect;
}

void Handler::initGame() {
	//GameHandler
	if (gameHandler != nullptr) {
		delete gameHandler;
	}
	gameHandler = new GameHandler(this);
}


void Handler::tick(GLfloat dt) {
	//do connections
	int size;
	float tempData[1024];
	if (connect->getMode() != ConnectionMode::SERVERLESS) {
		if (gameHandler->getPlayer() != nullptr) {
			float temp[3] = { ConnectionConstants::CLIENT_PARTICLE_PULL, gameHandler->getPlayer()->position.x, gameHandler->getPlayer()->position.y };
			connect->sendData(temp, 3 * sizeof(float));
		}
		connect->getData(tempData, size);


		if (tempData[0] == ConnectionConstants::SERVER_PARTICLE_POST) {
			//clear old particles from game
			if (gameHandler != nullptr) {
				gameHandler->clearServerObjs();
			}

			for (int i = 1; i < size; i += 7) {
				int theId = tempData[i];
				float theRad = tempData[i + 1];
				float theX = tempData[i + 2];
				float theY = tempData[i + 3];
				float theCR = tempData[i + 4];
				float theCG = tempData[i + 5];
				float theCB = tempData[i + 6];

				//add particles
				GameObject* toAdd = new GameObject(theX, theY, false, glm::vec3(theCR, theCG, theCB));
				toAdd->setId(theId);
				gameHandler->addParticleServer(toAdd, theId);
			}
		} else if (tempData[0] == ConnectionConstants::SERVER_INFO) {
			//set server info fields
			if (currentPage->type == PageType::GAME && ((GamePage*)currentPage)->gameHUD != nullptr) {
				std::cout << "Update server info: " << std::endl;
				std::string serverName;

				for (int i = 4; i < 255 && ((char*)tempData)[i] != '\0'; i++) {
					serverName+=((char*)tempData)[i];
				}
				std:: cout << "ServerName: " << serverName <<std::endl;
				((GamePage*)currentPage)->gameHUD->setServerName(serverName);// ->text = "Server Name: " + serverName;
			}
		}		

		if (leftClick) {
			float toSend[3] = { ConnectionConstants::CLIENT_PARTICLE_ADD, mouseCoords.x, mouseCoords.y };
			connect->sendData(toSend, 3 * sizeof(float));
		}
	}

	if (currentPage->type == PageType::GAME) {
		if (gameHandler != nullptr) {
			gameHandler->tick(dt, leftClick, keys);
		}
	}

	if (currentPage != nullptr) {
		currentPage->tick();
		currentPage->mouseHover(mouseCoords);
		if (leftClick) {
			currentPage->mouseClick(mouseCoords);
		}
		if (currentPage->nextPage != nullptr) {
			//if going from connect to game
			if (currentPage->type == PageType::CONNECT && currentPage->nextPage->type == PageType::GAME) {
				std::string mode = ((ConnectPage*)currentPage)->connectModeInput->selectedElement->text;
				std::string ip = ((ConnectPage*)currentPage)->ipInput->text;
				std::string id = ((ConnectPage*)currentPage)->idInput->text;
				//connect, if failed then set status and dont switch to game page
				//if connect success then set next switch to game page
				if (((ConnectPage*)currentPage)->doConnect) {
					std::cout << "Connectmode: " << mode << std::endl;
					if (mode._Equal("TCP")) {
						if (connect->getTCPConnect()->TCPConnect(54000, ip)) {
							connect->setMode(ConnectionMode::TCP);
							setCurrentPage(currentPage->nextPage);
						} else {
							((ConnectPage*)currentPage)->connectStatus->text = "Failed to connect to TCP Server";
							((ConnectPage*)currentPage)->doConnect = false;
						}
					} else if (mode._Equal("UDP")) {
						//weird bug where if you get the id already taken error,
						//you seem to always get it with ever new id, couldn't replicate on
						//second attempt
						connect->getUDPConnect()->setUserID(std::stoi(id));
						int connectCode = connect->getUDPConnect()->UDPConnect(54000, ip);
						if (connectCode == ConnectionConstants::SERVER_INFO) {// == 1 ? UPDATE HERE
							connect->setMode(ConnectionMode::UDP);
							setCurrentPage(currentPage->nextPage);
							//get server name?
						} else if (connectCode == ConnectionConstants::SERVER_CLIENTID_INVALID) {
							((ConnectPage*)currentPage)->connectStatus->text = "UserId already taken";
							((ConnectPage*)currentPage)->doConnect = false;
						} else {
							((ConnectPage*)currentPage)->connectStatus->text = "Failed to connect to UDP Server";
							((ConnectPage*)currentPage)->doConnect = false;
						}
					} else {
						((ConnectPage*)currentPage)->doConnect = false;
					}	
				}


			} else if (currentPage->nextPage->type == PageType::GAME) {
				//not sure what this block is for...
				setCurrentPage(currentPage->nextPage);
			} else {
				setCurrentPage(currentPage->nextPage);
			}

		}
	}

	if (leftClick && connect->getMode() == ConnectionMode::SERVERLESS) {
		if (currentPage != nullptr && currentPage->type == PageType::GAME) {
			gameHandler->addObj(new GameObject(mouseCoords.x, mouseCoords.y, false));
		}
	}

	if (rightClick && gameHandler != nullptr) {
		gameHandler->mouseClick(mouseCoords, 0);
	}

	bool keyTyped = false;
	for (char i = '.'; i <= 'z'; i++) {
		if (keys[i]) {
			currentPage->keyPress(i);
			keyTyped = true;
			break;
		}
	}
	//backspace
	if (keys[GLFW_KEY_BACKSPACE] && !keyTyped) {
		currentPage->keyPress(GLFW_KEY_BACKSPACE);
	}
	//space
	if (keys[GLFW_KEY_SPACE] && !keyTyped) {
		currentPage->keyPress(GLFW_KEY_SPACE);
	}

	
	if (currentPage != nullptr && currentPage->type != PageType::GAME && gameHandler != nullptr) {
		delete gameHandler;
		gameHandler = nullptr;
		connect->close();
		connect->setMode(ConnectionMode::SERVERLESS);
		//disconnect from server
	}
	//do not carry the left click into the next tick
	leftClick = false;
	rightClick = false;
}

void Handler::render() {
	//Add all the objects shape to the render
	// dont render these if serverless?
	if (currentPage->type == PageType::GAME) {
		gameHandler->render(renderer);
	}
	if (currentPage != nullptr) {
		currentPage->render(renderer);
	}
	//Do the actual draw
	renderer->Draw();
}


void Handler::addObj(GameObject* obj) {
	gameHandler->addObj(obj);
}

void Handler::collide(GameObject* obj1, GameObject* obj2) {
	//nullptr here, obj1
	float dist = sqrt(pow(obj1->position.x - obj2->position.x, 2)
				       + pow(obj1->position.y - obj2->position.y, 2));
	if (dist <= obj1->getRadius()) {
		glm::vec2 temp = glm::vec2(obj1->velocity.x, obj1->velocity.y);
		obj1->velocity.x = obj2->velocity.x * 1.0f;
		obj1->velocity.y = obj2->velocity.y * 1.0f;

		obj2->velocity.x = temp.x * 1.0f;
		obj2->velocity.y = temp.y * 1.0f;
	}
}

void Handler::setCurrentPage(Page* nextPage) {
	if (currentPage != nullptr) {
		//would this delete the nextPage? Should nextPage be deleted in page destructor?
		delete currentPage;
	}
	currentPage = nextPage;
	if (currentPage->type == PageType::GAME && ((GamePage*)currentPage)->gameHUD == nullptr) {
		initGame();
		((GamePage*)currentPage)->gameHUD = new GameHUD(this);
	}
}

void Handler::setMousePosition(glm::vec2 position) {
	this->mouseCoords = position;
}

void Handler::setMouseX(GLfloat xPosition) {
	this->mouseCoords.x = xPosition;
}

void Handler::setMouseY(GLfloat yPosition) {
	this->mouseCoords.y = yPosition;
}

void Handler::setRightClick(GLboolean rightClick) {
	this->rightClick = rightClick;
}

void Handler::setLeftClick(GLboolean leftClick) {
	this->leftClick = leftClick;
}

Connection* Handler::getConnection() {
	return connect;
}

bool Handler::keyPressed(int keyCode) {
	if (keyCode >=0 && keyCode < 1024) {
		return keys[keyCode];
	} else {
		return false;
	}
}

void Handler::setKeyPress(int keyCode, GLboolean value) {
	if (keyCode >= 0 && keyCode < 1024) {
		keys[keyCode] = value;
	}
}

GameHandler* Handler::getGameHander() {
	return gameHandler;
}