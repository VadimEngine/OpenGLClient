#include "GameHandler.h"
#include "Handler.h"

GameHandler::GameHandler(Handler* theHandler) {
	//float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0f - 1;
	player = new GameObject(0,0, true);
	selectedObj = nullptr;
	this->gravity = true;
	this->paused = false;
	this->selectedId = -1;
	this->theHandler = theHandler;
}

GameHandler::~GameHandler() {
	if (player != nullptr) {
		delete player;
	}
	for (GameObject* p: objs) {
		delete p;
	}
	objs.clear();
	clearServerObjs();
}

void GameHandler::tick(GLfloat dt, bool LeftClick, GLboolean* keys) {
	if (player != nullptr) {
		player->tick(dt, keys);
	}
	if (theHandler->getConnection()->getMode() == ConnectionMode::SERVERLESS) {
		if (!paused) {
			for (unsigned int i = 0; i < objs.size(); i++) {
				objs[i]->tick(dt, keys);
			}

			for (unsigned int i = 0; i < objs.size(); i++) {
				GameObject* obj = objs[i];

				if (obj->position.y > -.7f && obj->position.y < 1) {
					if (gravity) {
						obj->velocity -= glm::vec2(0, .981) * dt;
					}
				}
				if (obj->position.x <= -1) {
					obj->velocity.x *= -0.9f;
					obj->position.x = -1;
				}
				if (obj->position.x >= 1) {
					obj->velocity.x *= -0.9f;
					obj->position.x = 1;
				}
				if (obj->position.y <= -.7) {
					obj->velocity.y *= -1.0f;
					obj->position.y = -.7f;
				}
				if (obj->position.y >= 1) {
					obj->velocity.y *= -0.9f;
					obj->position.y = 1;
				}
			}

			for (unsigned int i = 0; i < objs.size(); i++) {
				for (unsigned int j = i + 1; j < objs.size(); j++) {
					collide(objs[i], objs[j]);
				}
			}
		}
		//player-obj collisions
		for (unsigned int i = 0; i < objs.size(); i++) {
			if (player != nullptr) {
				collide(player, objs[i]);
			}
		}
	}
}

void GameHandler::render(Renderer* renderer) {
	if (theHandler->getConnection()->getMode() != ConnectionMode::SERVERLESS) {
		for (const auto& sm_pair: serverObjs) {
			GameObject* theObj = sm_pair.second;
			if (theObj->getId() == selectedId) {
				//render circle
				renderer->renderCircleColor(glm::vec3(theObj->position, 0), glm::vec3(1, 1, 0), .05f, 10.0f);
			}
			theObj->render(renderer);
		}
	}


	for (unsigned i = 0; i < objs.size(); i++) {
		if (selectedObj != nullptr && selectedObj == objs[i]) {
			renderer->renderCircleColor(glm::vec3(objs[i]->position, 0), glm::vec3(1,1,0), .05f, 10.0f);
		}
		objs[i]->render(renderer);
	}

	if (player != nullptr && theHandler->getConnection()->getMode() == ConnectionMode::SERVERLESS) {
		player->render(renderer);
	}
}


void GameHandler::collide(GameObject* obj1, GameObject* obj2) {
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

void GameHandler::addObj(GameObject* obj) {
	objs.push_back(obj);
}

void GameHandler::mouseClick(glm::vec2 mouseCoord, int button) {
	//if right click, iterate all objs, if inbound select, otherwise set select to nullptr
	std::cout << "GameHandler click" << std::endl;

	bool selected = false;

	for (unsigned int i = 0; i < objs.size(); i++) {
		float dist = sqrt(pow(mouseCoord.x - objs[i]->position.x, 2)
			+ pow(mouseCoord.y - objs[i]->position.y, 2));
		if (dist <= objs[i]->getRadius() ) {
			selectedObj = objs[i];
			selected = true;
		}
	}

	if (!selected) {
		selectedObj = nullptr;
	}
}

void GameHandler::selectNext() {
	if (selectedObj == nullptr) {
		//get first
		if (objs.size() != 0) {
			selectedObj = objs[0];
		}
	} else {
		int selectedIndex = -1;
		//find the selected index
		for (unsigned int i = 0; i < objs.size(); i++) {
			if (objs[i] == selectedObj) {
				selectedIndex = i;
			}
		}
		selectedObj = objs[(selectedIndex + 1) % objs.size()];
	}
}

void GameHandler::removeParticle(GameObject* obj) {
	objs.erase(std::remove(objs.begin(), objs.end(), obj), objs.end());
	selectedObj = nullptr;
	delete obj;
}


void GameHandler::setPause(bool pause) {
	this->paused = pause;
}

void GameHandler::setGravity(bool gravity) {
	this->gravity = gravity;
}


void GameHandler::removeParticleServer(int theID) {
	float theData[2] = {ConnectionConstants::CLIENT_PARTICLE_REMOVE, theID};
	theHandler->getConnection()->sendData((void*)theData, 2*sizeof(float));
}


void GameHandler::addParticleServer(GameObject* obj, int Id) {
	serverObjs.insert({Id, obj});
}


void GameHandler::selectNextServer() {
	//always returns -1?
	if (serverObjs.size() == 0) {
		selectedId = -1;
		return;
	}

	if (selectedId == -1) {
		//get first element if there is one
		selectedId = serverObjs.begin()->first;
	} else {
		//get next biggest key, current is largest then get smallest key
		std::map<int, GameObject*>::iterator testIT = serverObjs.find(selectedId);

		if (testIT != serverObjs.end() && std::next(testIT) != serverObjs.end()) {
			selectedId = (++testIT)->first;
			std::cout << "Next: " << selectedId << std::endl;
			
		} else if (testIT != serverObjs.end() && std::next(testIT) == serverObjs.end()) {
			//if last element then return the first element
			std::cout << "First: " << serverObjs.begin()->first << std::endl;
			selectedId = serverObjs.begin()->first;
		} else {
			std::cout << "Not found: First: " << serverObjs.begin()->first << std::endl;
			selectedId = serverObjs.begin()->first;
		}
	}
}

GameObject* GameHandler::getPlayer() {
	return player;
}

GameObject* GameHandler::getSelectedObj() {
	return selectedObj;
}

int GameHandler::getSelectedId() {
	return selectedId;
}

void GameHandler::clearServerObjs() {
	std::map<int, GameObject*>::iterator itr = serverObjs.begin();
	while (itr != serverObjs.end()) {
		delete itr->second;
		itr++;
	}
	serverObjs.clear();
}

bool GameHandler::isPaused() {
	return paused;
}

bool GameHandler::gravityEnabled() {
	return gravity;
}

std::vector<GameObject*>* GameHandler::getObjs() {
	return &objs;
}

std::map<int, GameObject*>* GameHandler::getServerObjs() {
	return &serverObjs;
}