#include "Page.h"

void Page::tick() {
	for (int i = 0; i < components.size(); i++) {
		components[i]->tick();
	}
}

void Page::render(Renderer* renderer) {
	for (Button button : buttons) {
		button.render(renderer);
	}
	
	for (int i = 0; i < components.size(); i++) {
		components[i]->render(renderer);
	}
}

void Page::mouseHover(glm::vec2 mouseCoord) {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].inbound(mouseCoord)) {
			buttons[i].setHighlight(true);
		} else {
			buttons[i].setHighlight(false);
		}
	}

	for (int i = 0; i < components.size(); i++) {
		components[i]->mouseHover(mouseCoord);
	}
}