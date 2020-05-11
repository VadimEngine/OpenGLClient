#include "Button.h"


Button::Button(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension)
:Component::Component(ComponentType::BUTTON, position, color){
	this->position = position;
	this->color = color;
	this->text = text;
	this->dimension = dimension;
}

void Button::tick() {}

void Button::render(Renderer* renderer) {
	renderer->renderRectangleColor(position, dimension.x, dimension.y, color);
	renderer->renderString(text, glm::vec2(position.x + .05, position.y - .01));//,  position.x + .1, position.y - .01
}


void Button::renderHighlighted(Renderer* renderer) {
	renderer->renderRectangleColor(position + glm::vec3(-.01, .01, 0), dimension.x + .02, dimension.y + .02, glm::vec3(1, 1, 1));

	renderer->renderRectangleColor(position, dimension.x, dimension.y, color);
	renderer->renderString(text, glm::vec2(position.x + .05, position.y - .01));//,  position.x + .1, position.y - .01
}

void Button::clickAction() {}

bool Button::inbound(glm::vec2 coord) {
	if (coord.x >= position.x && coord.x <= position.x + dimension.x
		&& coord.y <= position.y && coord.y >= position.y - dimension.y) {
		return true;
	}
	return false;
}