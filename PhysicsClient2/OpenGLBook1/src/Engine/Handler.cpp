#include "Handler.h"


Handler::Handler(int count)
	:keys{}, leftClick(false) {
	connect = new Connection();
	mouseCoords = glm::vec2(0,0);
	Shader* myShader = new Shader("src/shaders/sandbox.vert",
									"src/shaders/sandbox.frag");//delete in desctructor
	renderer = new Renderer(myShader);
	player = new GameObject(0, 0, true);
	
	for (int i = 0; i < count; i++) {
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0f - 1;
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0f - 1;
		objs.push_back(new GameObject(r1, r2, false));
	}
}

Handler::~Handler() {
	delete renderer;
	delete player;
	for (GameObject* p: objs) {
		delete p;
	}
	objs.clear();
}

void Handler::tick(GLfloat dt) {
	//do connections
	if (connect->theMode != Serverless) {
		int size;
		float tempData[1024];
		float temp[3] = { 1, player->position.x, player->position.y };

		connect->sendData(temp, 3 * sizeof(float));
		connect->getData(tempData, size);

		for (int i = 0; i < size; i += 2) {
			float x1 = tempData[i];
			float y1 = tempData[i + 1];
			renderer->renderCircle(x1, y1, .04, 10);//render in update for now
		}

		if (leftClick) {
			float toSend[3] = { -2, mouseCoords.x, mouseCoords.y };
			connect->sendData(toSend, 3 * sizeof(float));
			leftClick = false;
		}
	}

	for (int i = 0; i < objs.size(); i++) {
		objs[i]->tick(dt, keys);
	}
	player->tick(dt, keys);

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
		collide(player, objs[i]);
	}
}

void Handler::render() {
	//Add all the objects shape to the render
	// dont render these if serverless?
	for (int i = 0; i < objs.size(); i++) {
		objs[i]->render(renderer);
	}
	if (connect->theMode == Serverless) {
		player->render(renderer);
	}
	//Do the actual draw
	renderer->Draw();
}


void Handler::addObj(GameObject* obj) {
	objs.push_back(obj);
}

void Handler::collide(GameObject* obj1, GameObject* obj2) {
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
	connect->connectionProtocol();
	return -1;
}