#include "Component.h"


Component::Component(ComponentType type, glm::vec3 position, glm::vec3 color)
:type(type) {
	this->position = position;
	this->color = color;
}

void Component::render(Renderer* renderer) {}

void Component::renderHighlighted(Renderer* renderer) {}

void Component::tick() {}

void Component::mouseHover(glm::vec2 mouseCoord) {}

void Component::action() {}