#include "RetainedHandler.h"



RetainedHandler::RetainedHandler(int count)
	:keys{} {

	//create shader
	SandboxShader* myShader = new SandboxShader("src/shaders/sandbox.vert",
												"src/shaders/sandbox.frag");//delete in desctructor
	renderer = new RetainedRenderer(myShader);

	player = new RetainedObj(0, 0, glm::vec3(1, 1, 1), myShader, true);
	
	for (int i = 0; i < count; i++) {
		//objs.push_back(new RetainedObj((float)i/10.0f, (float)i/10.0f, glm::vec3(1, 1, 1), myShader));

		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0f - 1;
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.0f - 1;

		float c1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float c2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float c3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//objs.push_back(new ImmediateObj((float)i/10, (float)i / 10, glm::vec3((float)i / 10, (float)i / 10, (float)i / 10), false));
		objs.push_back(new RetainedObj(r1, r2, glm::vec3(c1, c2, c3), NULL, false));
	}


}

RetainedHandler::~RetainedHandler() {
	delete renderer;
	delete player;
	std::cout << "Deleting: " << objs.size() << std::endl;
	for (RetainedObj* p: objs) {
		delete p;
	}
	objs.clear();
	//delete mysShader
}

void RetainedHandler::addObj(RetainedObj* obj) {
	objs.push_back(obj);
}

void RetainedHandler::render() {

	for (int i = 0; i < objs.size(); i++) {
		objs[i]->render(renderer);
	}

	if (!isServer) {
		player->render(renderer);//pass in shader on render?
	}
	
	//renderer->initRenderData();//init every time render is called?
	renderer->Draw();
}

void RetainedHandler::collide(RetainedObj* obj1, RetainedObj* obj2) {
	float rad = .04f;

	float dist = sqrt(pow(obj1->position.x - obj2->position.x, 2) + pow(obj1->position.y - obj2->position.y, 2));

	if (dist <= rad) {
		glm::vec2 temp = glm::vec2(obj1->velocity.x, obj1->velocity.y);
		obj1->velocity.x = obj2->velocity.x * 1.0f;
		obj1->velocity.y = obj2->velocity.y * 1.0f;

		obj2->velocity.x = temp.x * 1.0f;
		obj2->velocity.y = temp.y * 1.0f;
	}

#ifdef false

	float dist = sqrt(pow(center2.x - center1.x, 2) + pow(center2.y - center1.y, 2));

	if (dist <= rad1 || dist <= rad2) {

		glm::vec2 temp = glm::vec2(a.Velocity.x, a.Velocity.y);
		a.Velocity.x = b.Velocity.x * 1.0f;
		a.Velocity.y = b.Velocity.y * 1.0f;

		b.Velocity.x = temp.x * 1.0f;
		b.Velocity.y = temp.y * 1.0f;

#endif // false



}

void RetainedHandler::tick(GLfloat dt) {

	for (int i = 0; i < objs.size(); i++) {
		objs[i]->tick(dt, keys);
	}

	player->tick(dt, keys);


	for (int i = 0; i < objs.size(); i++) {
		RetainedObj* obj = objs[i];

		if (obj->position.y > -1.0f && obj->position.y < 1) {
			obj->velocity -= glm::vec2(0, .981) * dt;
		}

		if (obj->position.x <= -1) {
			obj->velocity.x *= -0.9f;
			obj->position.x = -1;
		}

		if (obj->position.x >= 1) {//width
			obj->velocity.x *= -0.9f;
			obj->position.x = 1;// -BALL_SIZE.x;
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

	//player
	for (int i = 0; i < objs.size(); i++) {
		collide(player, objs[i]);
	}



}

