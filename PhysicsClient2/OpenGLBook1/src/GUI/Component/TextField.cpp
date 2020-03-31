#include "TextField.h"


TextField::TextField(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension) 
:Component::Component(TEXTFIELD, position, position) {
	this->position = position;
	this->color = color;
	this->text = text;
	this->dimension = dimension;
}


void TextField::tick() {
	Component::tick();
}

void TextField::render(Renderer* renderer) {
	renderer->renderString(text, position.x, position.y);
}