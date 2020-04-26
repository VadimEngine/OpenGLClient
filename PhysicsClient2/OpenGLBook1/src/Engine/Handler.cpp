#include "Handler.h"


Handler::Handler()
	:keys{}, leftClick(false) {
	connect = new Connection();
	connect->theMode = ConnectionMode::Serverless;
	mouseCoords = glm::vec2(0,0);
	//player = nullptr;
	Shader* polygonShader = new Shader("src/shaders/sandbox.vert",
									"src/shaders/sandbox.frag");//delete in desctructor

	Shader* imageShader = new Shader("src/shaders/image.vert",
							   	  "src/shaders/image.frag");//delete in desctructor

	renderer = new Renderer(polygonShader, imageShader);
	
	currentPage = new IntroPage();
	rightClick = false;
}

Handler::~Handler() {
	delete renderer;
}

void Handler::initGame() {
	//GameHandler
	if (gameHandler != nullptr) {
		delete gameHandler;
	}
	gameHandler = new GameHandler(connect->theMode != ConnectionMode::Serverless, connect);
}


void Handler::tick(GLfloat dt) {
	//do connections
	int size;
	float tempData[1024];
	if (connect->theMode != Serverless) {
		if (gameHandler->player != nullptr) {
			float temp[3] = { ConnectionConstants::CLIENT_PARTICLE_PULL, gameHandler->player->position.x, gameHandler->player->position.y };
			connect->sendData(temp, 3 * sizeof(float));
		}
		connect->getData(tempData, size);

		//if (float)tempdata[0] == ::server_connet, then set HUD server info

		if (tempData[0] == ConnectionConstants::SERVER_PARTICLE_POST) {
			if (gameHandler != nullptr) {
				/*
				for (int i = 0; i < gameHandler->objs.size(); i++) {
					delete gameHandler->objs[i];
				}
				gameHandler->objs.clear();
				*/
				std::map<int, GameObject*>::iterator itr = gameHandler->serverObjs.begin();
				while (itr != gameHandler->serverObjs.end()) {
					delete itr->second;
					itr++;
				}
				gameHandler->serverObjs.clear();
			}


			for (int i = 1; i < size; i += 7) {
				/*
				float x1 = tempData[i];
				float y1 = tempData[i + 1];
				renderer->renderCircleColor(glm::vec3(x1, y1, 0), glm::vec3(1,1,1), .04, 10);//render in update for now might be why there is flicker
				*/
				int theId = tempData[i];
				float theRad = tempData[i + 1];
				float theX = tempData[i + 2];
				float theY = tempData[i + 3];
				float theCR = tempData[i + 4];
				float theCG = tempData[i + 5];
				float theCB = tempData[i + 6];

				//clear particles from game
				//add particles
				GameObject* toAdd = new GameObject(theX, theY, false, glm::vec3(theCR, theCG, theCB));
				toAdd->id = theId;
				//gameHandler->addObj(toAdd);
				gameHandler->addParticleServer(toAdd, theId);
				//renderer->renderCircleColor(glm::vec3(theX, theY, 0), glm::vec3(theCR, theCG, theCB), theRad, 10);//render in update for now might be why there is flicker
			}
		} else if (tempData[0] == ConnectionConstants::SERVER_INFO) {
			//set server info fields
			if (currentPage->type == PageType::Game && ((GamePage*)currentPage)->gameHUD != nullptr) {
				std::cout << "Update server info: " << std::endl;

				std::string serverName;

				for (int i = 4; i < 255 && ((char*)tempData)[i] != '\0'; i++) {
					serverName+=((char*)tempData)[i];
				}
				std:: cout << "ServerName: " << serverName <<std::endl;
				((GamePage*)currentPage)->gameHUD->serverNameInfo->text = "Server Name: " + serverName;

			}
		}		

		if (leftClick) {
			float toSend[3] = { ConnectionConstants::CLIENT_PARTICLE_ADD, mouseCoords.x, mouseCoords.y };
			connect->sendData(toSend, 3 * sizeof(float));
		}
		//send server info pull
		//float toSend[1] = { ConnectionConstants::CLIENT_CONNECT };
		//connect->sendData(toSend, 1 * sizeof(float));
	}


	if (currentPage->type == Game) {
		if (gameHandler != nullptr) {
			gameHandler->tick(dt, leftClick, keys);
		}

		if (((GamePage*)currentPage)->gameHUD->toMenu) {
			setCurrentPage(new MenuPage());
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
			if (currentPage->type == Connect && currentPage->nextPage->type == Game) {
				std::string mode = ((ConnectPage*)currentPage)->connectModeInput->selectedElement->text;
				std::string ip = ((ConnectPage*)currentPage)->ipInput->text;
				std::string id = ((ConnectPage*)currentPage)->idInput->text;
				//connect, if failed then set status and dont swtich to game page
				//if connect success then set next swith to game page
				if (((ConnectPage*)currentPage)->doConnect) {
					std::cout << "Connectmode: " << mode << std::endl;
					if (mode._Equal("TCP")) {
						if (connect->TCPCon.TCPConnect(54000, ip)) {
							connect->theMode = TCP;
							//initGame();
							setCurrentPage(currentPage->nextPage);
							//currentPage = currentPage->nextPage;
						} else {
							((ConnectPage*)currentPage)->connectStatus->text = "Failed to connect to TCP Server";
							((ConnectPage*)currentPage)->doConnect = false;
						}
					} else if (mode._Equal("UDP")) {
						//weird bug where if you get the id already taken error,
						//you seem to always get it with ever new id, coulndt replicate on
						//second attempt
						connect->UDPCon.userId = std::stoi(id);
						int connectCode = connect->UDPCon.UDPConnect(54000, ip);
						if (connectCode == 1) {
							connect->theMode = UDP;
							//initGame();
							setCurrentPage(currentPage->nextPage);
							//currentPage = currentPage->nextPage;
						} else if (connectCode == -2) {
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


			} else if (currentPage->nextPage->type == Game) {
				//not sure what this block is for...
				setCurrentPage(currentPage->nextPage);
				//currentPage = currentPage->nextPage;
				//initGame();
			} else {
				setCurrentPage(currentPage->nextPage);
				//currentPage = currentPage->nextPage;
			}

		}
	}

	if (leftClick && connect->theMode == ConnectionMode::Serverless) {
		if (currentPage->type == Game) {
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

	
	if (currentPage != nullptr && currentPage->type != PageType::Game && gameHandler != nullptr) {
		delete gameHandler;
		gameHandler = nullptr;
		connect->close();
		connect->theMode = ConnectionMode::Serverless;
		//disconnect from server
	}
	//do not carry the left click into the next tick
	leftClick = false;
	rightClick = false;

}

void Handler::render() {
	//Add all the objects shape to the render
	// dont render these if serverless?
	if (currentPage->type == Game) {
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
	if (dist <= obj1->radius) {
		glm::vec2 temp = glm::vec2(obj1->velocity.x, obj1->velocity.y);
		obj1->velocity.x = obj2->velocity.x * 1.0f;
		obj1->velocity.y = obj2->velocity.y * 1.0f;

		obj2->velocity.x = temp.x * 1.0f;
		obj2->velocity.y = temp.y * 1.0f;
	}
}

bool Handler::connectionProtocol() {
	//do this on connect button?
	connect->connectionProtocol();
	return -1;
}

void Handler::setCurrentPage(Page* nextPage) {
	if (currentPage != nullptr) {
		//would this delete the nextapge? Should nextPage be deleted in page destructor?
		delete currentPage;
	}
	currentPage = nextPage;
	if (currentPage->type == PageType::Game && ((GamePage*)currentPage)->gameHUD == nullptr) {
		initGame();
		((GamePage*)currentPage)->gameHUD = new GameHUD(connect, &(gameHandler->objs), gameHandler->player, &(gameHandler->selectedObj), gameHandler);
	}
}