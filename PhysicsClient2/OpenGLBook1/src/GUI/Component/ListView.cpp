#include "ListView.h"

ListView::ListView(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension) 
:Component::Component(LISTVIEW, position, color) {
	this->dimension = dimension;
	selectedElement = nullptr; 
}

void ListView::tick() {

}

void ListView::render(Renderer* renderer) {
	//draw boarder

	renderer->renderRectangleColor(position + glm::vec3(-.01, .01, 0), dimension.x + .02 + (.01 * 2), dimension.y + .02 + (.01 * 2), glm::vec3(1, 1, 1));
	renderer->renderRectangleColor(position, dimension.x + .01*2, dimension.y + .01 * 2, color);


	for (int i = 0; i < elements.size(); i++) {
		if (selectedElement != elements[i]) {
			elements[i]->render(renderer);
		} else {
			elements[i]->renderSelected(renderer);
		}

	}
}

void ListView::addElement(std::string elementText) {
	if (elements.empty()) {
		elements.push_back(new ListViewElement(position + glm::vec3(.01, -.01, 0),
			glm::vec3(.4, .4, .4), elementText, glm::vec2(.3, .1)));
		selectedElement = elements.back();
	} else {
		glm::vec3 nextPos = elements.back()->position + glm::vec3(0, -.1, 0);
		elements.push_back(new ListViewElement(nextPos,
			glm::vec3(.4, .4, .4), elementText, glm::vec2(.3, .1)));
		
	}
}

void ListView::mouseHover(glm::vec2 mousePos) {
	if (mousePos.x > position.x && mousePos.x < position.x + dimension.x
		&& mousePos.y < position.y && mousePos.y > position.y - dimension.y) {
		for (int i = 0; i < elements.size(); i++) {
			elements[i]->mouseHover(mousePos);
		}
	}
}

void ListView::mouseClick(glm::vec2 mousePos) {
	if (mousePos.x > position.x&& mousePos.x < position.x + dimension.x
		&& mousePos.y < position.y && mousePos.y > position.y - dimension.y) {
		for (int i = 0; i < elements.size(); i++) {
			if (elements[i]->hover) {
				selectedElement = elements[i];
				break;
			}
		}
	}
}