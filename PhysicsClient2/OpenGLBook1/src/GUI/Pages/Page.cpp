#include "Page.h"

Page::Page() {
	this->nextPage = nullptr; 
	this->highlightedButton = nullptr;
	this->selectedComponent = nullptr;
}

void Page::tick() {
	for (unsigned int i = 0; i < components.size(); i++) {
		components[i]->tick();
	}
}

void Page::render(Renderer* renderer) {	
	for (unsigned int i = 0; i < components.size(); i++) {
		if (components[i]->type == ComponentType::BUTTON && highlightedButton == components[i]) {
			components[i]->renderHighlighted(renderer);
		} else {
			components[i]->render(renderer);
		}
		
	}
}

void Page::mouseHover(glm::vec2 mouseCoord) {
	bool highlighted = false;
	for (unsigned int i = 0; i < components.size(); i++) {
		if (((Button*)components[i])->inbound(mouseCoord)) {
			highlightedButton = (Button*)components[i];
			highlighted = true;
		}
		components[i]->mouseHover(mouseCoord);
	}
	if (!highlighted) {
		highlightedButton = nullptr;
	}
}