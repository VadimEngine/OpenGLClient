#include "InfoPage.h"

InfoPage::InfoPage() {
	this->type = Info;
	components.push_back(new Button(glm::vec3(0 - (.3 / 2.0), .60, 0),
		glm::vec3(.4, .4, .4), "MENU", glm::vec2(.3, .1)));

	components.push_back(new TextField(glm::vec3(0 - (.3 / 2.0), .1, 0),
		glm::vec3(.4, .4, .4), "INFORMATION HERE", glm::vec2(.3, .1), 1.0f));
}


void InfoPage::render(Renderer* renderer) {
	Page::render(renderer);
}

void InfoPage::tick() {
	Page::tick();
}

void InfoPage::mouseClick(glm::vec2 mouseClick) {
	for (int i = 0; i < components.size(); i++) {
		if (((Button*)components[i])->inbound(mouseClick)) {
			if (((Button*)components[i])->text == "MENU") {
				nextPage = new MenuPage();
			}
			
		}
	}
}

void InfoPage::keyPress(int key) {
}