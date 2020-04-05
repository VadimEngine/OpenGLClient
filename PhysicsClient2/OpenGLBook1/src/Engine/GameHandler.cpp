#include "GameHandler.h"


GameHandler::GameHandler(bool server) {
	//float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0f - 1;
	this->server = server;
	player = new GameObject(0,0, true);
	selectedObj = nullptr;
	this->gravity = true;
	this->paused = false;
}

GameHandler::~GameHandler() {
	if (player != nullptr) {
		//exception thrown here when setting mode to serverless
		// and deleteing game handler (might have fixed it)
		delete player;
	}

	for (GameObject* p: objs) {
		delete p;
	}
	objs.clear();
}

void GameHandler::tick(GLfloat dt, bool LeftClick, GLboolean* keys) {

	//std::cout << "GameHandler: GameObjs: " << objs.size() << std::endl;

	if (player != nullptr) {
		player->tick(dt, keys);
	}

	if (!server) {
		if (!paused) {
			for (int i = 0; i < objs.size(); i++) {
				objs[i]->tick(dt, keys);
			}

			for (int i = 0; i < objs.size(); i++) {
				GameObject* obj = objs[i];

				if (obj->position.y > -1.0f && obj->position.y < 1) {
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
				if (obj->position.y <= -1) {
					obj->velocity.y *= -1;
					obj->position.y = -1;
				}
				if (obj->position.y >= 1) {
					obj->velocity.y *= -0.9f;
					obj->position.y = 1;
				}
			}

			for (int i = 0; i < objs.size(); i++) {
				for (int j = i + 1; j < objs.size(); j++) {
					collide(objs[i], objs[j]);
				}
			}

		}

		//player-obj collisions
		for (int i = 0; i < objs.size(); i++) {
			if (player != nullptr) {
				collide(player, objs[i]);
			}
		}

	}


}

void GameHandler::render(Renderer* renderer) {
	for (int i = 0; i < objs.size(); i++) {
		if (selectedObj != nullptr && selectedObj == objs[i]) {
			renderer->renderCircleColor(glm::vec3(objs[i]->position, 0), glm::vec3(1,1,0), .05, 10);
			//renderer->renderCircleColor(glm::vec3(position, 0.0), color, .04, 10);
		}
		objs[i]->render(renderer);
	}

	if (player != nullptr && !server) {
		player->render(renderer);
	}
}


void GameHandler::collide(GameObject* obj1, GameObject* obj2) {
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

void GameHandler::addObj(GameObject* obj) {
	objs.push_back(obj);
}

void GameHandler::mouseClick(glm::vec2 mouseCoord, int button) {
	//if right click, iterate all objs, if inbound select, otherwise set select to nullptr
	std::cout << "GameHandler click" << std::endl;

	bool selected = false;

	for (int i = 0; i < objs.size(); i++) {
		float dist = sqrt(pow(mouseCoord.x - objs[i]->position.x, 2)
			+ pow(mouseCoord.y - objs[i]->position.y, 2));
		if (dist <= objs[i]->radius ) {
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
		for (int i = 0; i < objs.size(); i++) {
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
}


void GameHandler::setPause(bool pause) {
	this->paused = pause;
}

void GameHandler::setGravity(bool gravity) {
	this->gravity = gravity;
}