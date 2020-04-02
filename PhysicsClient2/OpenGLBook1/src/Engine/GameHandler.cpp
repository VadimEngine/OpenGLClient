#include "GameHandler.h"


GameHandler::GameHandler(bool server) {
	this->server = server;
	//if (!server) {
		player = new GameObject(0,0, true);
	//}
}

GameHandler::~GameHandler() {
	if (player != nullptr) {
		//exception thrown here when setting mode to serverless
		// and deleteing game handler
		delete player;
	}

	for (GameObject* p: objs) {
		delete p;
	}
	objs.clear();
}

void GameHandler::tick(GLfloat dt, bool LeftClick, GLboolean* keys) {

	if (player != nullptr) {
		player->tick(dt, keys);
	}

	if (!server) {
		for (int i = 0; i < objs.size(); i++) {
			objs[i]->tick(dt, keys);
		}

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


}

void GameHandler::render(Renderer* renderer) {
	for (int i = 0; i < objs.size(); i++) {
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