#include "GameObject.h"


GameObject::GameObject(GLfloat x, GLfloat y, GLboolean isPlayer, glm::vec3 color) {
	this->isPlayer = isPlayer;
	//create seperate class for player that extends GameObject
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
	//renderer->renderCircle(position.x, position.y, .04, 10);
	//renderer->renderTriangle(glm::vec3(position.x, position.y + .04, 0),
	//					     glm::vec3(position.x - .04, position.y, 0),
	//					     glm::vec3(position.x + .04, position.y, 0));
	renderer->renderCircleColor(glm::vec3(position, 0.0), color, .04, 10);
	//
	//renderer->renderRectangleColor(glm::vec3(position, 0.0), .04, .04, color);

	//renderer->renderLineColor(glm::vec3(position, 0), glm::vec3(position + (velocity/10.0f), 0),
		//                       glm::vec3(1,1,1), .01);
	//renderer->renderSubImage(glm::vec3(position + glm::vec2(.5,0), 0), color);
	//renderer->renderSubImageAdvanced(glm::vec3(position, 0.0), color,
	//		256, 32, 6, 8, 0, 1);

	//renderer->renderString("TEST", position.x, position.y);
}