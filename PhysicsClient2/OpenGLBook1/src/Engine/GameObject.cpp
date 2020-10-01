#include "GameObject.h"


GameObject::GameObject(GLfloat x, GLfloat y, GLboolean isPlayer, glm::vec3 color) {
	this->isPlayer = isPlayer;
	//create separate class for player that extends GameObject
	radius = .04f;
	position = glm::vec2(x, y);

	float randXVel = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float randYvel = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	velocity = glm::vec2(randXVel, randYvel);
	lastPosition = glm::vec2(x, y);
	speed = 1;

	this->color = color;
}


GameObject::GameObject(GLfloat x, GLfloat y, GLboolean isPlayer)
:GameObject(x,y,isPlayer, glm::vec3(0,0,0)){
	float randXVel = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float randYvel = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float randZvel = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	this->color = glm::vec3(randXVel, randYvel, randZvel);
}

GameObject::~GameObject() {}

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
	renderer->renderCircleColor(glm::vec3(position, 0.0), color, .04, 10);
}

int GameObject::getId() {
	return id;
}

GLfloat GameObject::getRadius() {
	return radius;
}

void GameObject::setId(int id) {
	this->id = id;
}