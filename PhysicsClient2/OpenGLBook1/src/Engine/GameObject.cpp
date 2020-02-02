#include "GameObject.h"


GameObject::GameObject(GLfloat x, GLfloat y, glm::vec3 color,
	SandboxShader* myShader, GLboolean isPlayer)
	:color(color), isPlayer(isPlayer){

	radius = .04;
	position = glm::vec2(x, y);
	float c1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float c2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	velocity = glm::vec2(c1, c2);//player vel = 0, but calculate it when it moves (x_0 - x_1) / dt on tick
	lastPosition = glm::vec2(x,y);
}

void GameObject::tick(GLfloat dt, GLboolean keys[1024]) {
	GLfloat speed = 1;
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