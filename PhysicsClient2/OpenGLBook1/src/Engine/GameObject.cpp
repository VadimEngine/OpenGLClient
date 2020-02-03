#include "GameObject.h"


GameObject::GameObject(GLfloat x, GLfloat y, GLboolean isPlayer)
	:isPlayer(isPlayer){

	radius = .04f;
	position = glm::vec2(x, y);
	float randXVel = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float randYvel = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	velocity = glm::vec2(randXVel, randYvel);
	lastPosition = glm::vec2(x,y);
	speed = 1;
}

void GameObject::tick(GLfloat dt, GLboolean keys[1024]) {
	if (isPlayer) {
		if (keys[GLFW_KEY_UP]) {
			position.y += speed * dt;
		}
		if (keys[GLFW_KEY_RIGHT]) {
			position.x += speed * dt;
		}
		if (keys[GLFW_KEY_DOWN]) {
			position.y -= speed * dt;
		}
		if (keys[GLFW_KEY_LEFT]) {
			position.x -= speed * dt;
		}
		//calculate players velocity
		velocity = glm::vec2((position.x - lastPosition.x)/dt, (position.y - lastPosition.y) / dt);

		lastPosition.x = position.x;
		lastPosition.y = position.y;
	} else {
		position += velocity * dt;
	}
}

void GameObject::render(Renderer* renderer) {
	renderer->renderCircle(position.x, position.y, .04, 10);
}