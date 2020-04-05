#include "ListViewElement.h"


ListViewElement::ListViewElement(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension) 
:Component::Component(LISTVIEWELEMENT, position, color) {
	this->dimension = dimension;
	this->text = text;
	this->hover = false;
}

void ListViewElement::tick() {

}


//Have a render relative to postition method? or pass in parent
void ListViewElement::render(Renderer* renderer) {
	//render rectangle
	//render selected
	//render hover?

	if (hover) {
		renderer->renderRectangleColor(position + glm::vec3(-.01, .01, 0), dimension.x + .02, dimension.y + .02, glm::vec3(1, 1, 1));
	}

	//renderer->renderRectangleColor(position + glm::vec3(-.01, .01, 0), dimension.x + .02, dimension.y + .02, glm::vec3(1, 1, 1));
	renderer->renderRectangleColor(position, dimension.x, dimension.y, color);
	renderer->renderString(text, glm::vec2(position.x, position.y));//; position.x, position.y

}

void ListViewElement::renderSelected(Renderer* renderer) {
	//std::cout << "Render selected" << std::endl;
	//render rectangle
	//render selected
	//render hover?
	if (hover) {
		renderer->renderRectangleColor(position + glm::vec3(-.01, .01, 0), dimension.x + .02, dimension.y + .02, glm::vec3(1, 1, 1));
	}

	//renderer->renderRectangleColor(position + glm::vec3(-.01, .01, 0), dimension.x + .02, dimension.y + .02, glm::vec3(1, 1, 1));
	renderer->renderRectangleColor(position, dimension.x, dimension.y, glm::vec3(.6, .6, .6));
	renderer->renderString(text, glm::vec2(position.x, position.y));
}

void ListViewElement::mouseHover(glm::vec2 mouseCoord) {
	if (mouseCoord.x >= position.x && mouseCoord.x <= position.x + dimension.x
		&& mouseCoord.y <= position.y && mouseCoord.y >= position.y - dimension.y) {
		hover = true;
	} else {
		hover = false;
	}
}

