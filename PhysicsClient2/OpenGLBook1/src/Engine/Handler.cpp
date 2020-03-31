#include "Handler.h"


Handler::Handler(int count)
	:keys{}, leftClick(false) {
	connect = new Connection();
	connect->theMode = Serverless;
	mouseCoords = glm::vec2(0,0);
	player = nullptr;
	Shader* polygonShader = new Shader("src/shaders/sandbox.vert",
									"src/shaders/sandbox.frag");//delete in desctructor

	Shader* imageShader = new Shader("src/shaders/image.vert",
							   	  "src/shaders/image.frag");//delete in desctructor

	renderer = new Renderer(polygonShader, imageShader);
	
	for (int i = 0; i < count; i++) {
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0f - 1;
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0f - 1;
		objs.push_back(new GameObject(r1, r2, false));
	}
	currentPage = new IntroPage();
	keyTypeCounter = 0;
	letterKey = false;
}

Handler::~Handler() {
	if (player != nullptr) {
		delete player;
	}
	delete renderer;
	for (GameObject* p: objs) {
		delete p;
	}
	objs.clear();
}

void Handler::initGame() {
	player = new GameObject(0, 0, true);

}


void Handler::tick(GLfloat dt) {
	//do connections
	int size;
	float tempData[1024];
	if (connect->theMode != Serverless) {
		if (player != nullptr) {
			float temp[3] = { 1, player->position.x, player->position.y };
			connect->sendData(temp, 3 * sizeof(float));
		}
		connect->getData(tempData, size);

		for (int i = 0; i < size; i += 2) {
			float x1 = tempData[i];
			float y1 = tempData[i + 1];
			renderer->renderCircleColor(glm::vec3(x1, y1, 0), glm::vec3(1,1,1), .04, 10);//render in update for now
		}

		if (leftClick) {
			float toSend[3] = { -2, mouseCoords.x, mouseCoords.y };
			connect->sendData(toSend, 3 * sizeof(float));
			leftClick = false;
		}
	}


	if (currentPage->type == Game) {
		for (int i = 0; i < objs.size(); i++) {
			objs[i]->tick(dt, keys);
		}
		if (player != nullptr) {
			player->tick(dt, keys);
		}

		//Do gravity and in-bounds (can be do in obj?)
		for (int i = 0; i < objs.size(); i++) {
			GameObject* obj = objs[i];

			if (obj->position.y > -1.0f && obj->position.y < 1) {
				obj->velocity -= glm::vec2(0, .981) * dt;
			}
			if (obj->position.x <= -1) {
				obj->velocity.x *= -0.9f;
				obj->position.x = -1;
			}
			if (obj->position.x >= 1) {
				obj->velocity.x *= -0.9f;
				obj->position.x = 1;
			}
			if (obj->position.y <= -1) {
				obj->velocity.y *= -1;
				obj->position.y = -1;
			}
			if (obj->position.y >= 1) {
				obj->velocity.y *= -0.9f;
				obj->position.y = 1;
			}
		}
		//do obj colisions
		for (int i = 0; i < objs.size(); i++) {
			for (int j = i + 1; j < objs.size(); j++) {
				collide(objs[i], objs[j]);
			}
		}
		//player-obj collisions
		for (int i = 0; i < objs.size(); i++) {
			if (player != nullptr) {
				collide(player, objs[i]);
			}
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
					//connect->connectionProtocol();
					//Assume TCP
					if (mode._Equal("TCP")) {
						if (connect->TCPCon.TCPConnect()) {
							connect->theMode = TCP;
							initGame();
							currentPage = currentPage->nextPage;
						} else {
							((ConnectPage*)currentPage)->connectStatus->text = "Failed to connect to TCP Server";
							((ConnectPage*)currentPage)->doConnect = false;
						}
					} else if (mode._Equal("UDP")) {
						//weird bug where if you get the id already taken error,
						//you seem to always get it with ever new id, coulndt replicate on
						//second attempt
						connect->UDPCon.userId = std::stoi(id);
						int connectCode = connect->UDPCon.UDPConnect();
						if (connectCode == 1) {
							connect->theMode = UDP;
							initGame();
							currentPage = currentPage->nextPage;
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


			} else if (currentPage->nextPage->type == Game && player == nullptr) {
				//not sure what this block is for...
				currentPage = currentPage->nextPage;
				initGame();
			} else {
				currentPage = currentPage->nextPage;
			}

		}
	}

	if (leftClick && connect->theMode == Serverless) {
		if (currentPage->type == Game) {
			addObj(new GameObject(mouseCoords.x, mouseCoords.y, false));
			leftClick = false;
		}
	}
	leftClick = false;

	if (currentPage->type == Connect) {
		//if last key = this key then increment key counter, else set to 0
		//if keyCounter > 60 then add key
		// Very convoluded logic, will replace later:
		// Goal is to allow typing allow typing keys (currently A-Z),
		// keyTypeCounter and lastKey is to prevent typing the same
		// multiple times in a short time frame. Last key lets
		// the system know that a different key is pressed and the key can be typed
		// before the timer resets to 0
		if (letterKey) {
			//bool typed = false;
			//iterate possible key presses from the input key map
			for (char i = '.'; i <= 'z'; i++) {
				if (keys[i]) {
					if (keyTypeCounter == 0 || lastKey != i) {
						((ConnectPage*)currentPage)->KeyType(i);
						lastKey = i;
						keyTypeCounter = 1;
					}
					//prevents 2 keys at once bug
					break;
				}
			}

		} else {
			//backspace
			if (keys[GLFW_KEY_BACKSPACE]) {
				if (keyTypeCounter == 0 || lastKey != GLFW_KEY_BACKSPACE) {
					((ConnectPage*)currentPage)->KeyType(GLFW_KEY_BACKSPACE);
					lastKey = GLFW_KEY_BACKSPACE;
					keyTypeCounter = 1;
				}
			} else if (keys[GLFW_KEY_SPACE]) {
				if (keyTypeCounter == 0 || lastKey != GLFW_KEY_SPACE) {
					((ConnectPage*)currentPage)->KeyType(GLFW_KEY_SPACE);
					lastKey = GLFW_KEY_SPACE;
					keyTypeCounter = 1;
				}
			}
		}

		if (keyTypeCounter != 0 && keyTypeCounter < 15) {
			keyTypeCounter = (keyTypeCounter + 1) % 15;
			std::cout << "keyTypeCounter: " << keyTypeCounter << std::endl;
		}

	}

}

void Handler::render() {
	//Add all the objects shape to the render
	// dont render these if serverless?
	if (currentPage->type == Game) {	
		for (int i = 0; i < objs.size(); i++) {
			objs[i]->render(renderer);
		}
		if (connect->theMode == Serverless) {
			if (player != nullptr) {
				player->render(renderer);
			}
		}
	}
	if (currentPage != nullptr) {
		currentPage->render(renderer);
	}


	//Do the actual draw
	renderer->Draw();
}


void Handler::addObj(GameObject* obj) {
	objs.push_back(obj);
}

void Handler::collide(GameObject* obj1, GameObject* obj2) {
	//nullptry here, obj1
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