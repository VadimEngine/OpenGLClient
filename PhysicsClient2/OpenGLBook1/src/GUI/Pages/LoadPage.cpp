#include "LoadPage.h"

LoadPage::LoadPage() {
	this->type = PageType::LOAD;
	nextPage = nullptr;
	components.push_back(new Button(glm::vec3(0 - (.3 / 2.0), .60, 0),
		glm::vec3(.4, .4, .4), "MENU", glm::vec2(.3, .1)));

	components.push_back(new Button(glm::vec3(0 - (.3 / 2.0), .30, 0),
		glm::vec3(.4, .4, .4), "NEW", glm::vec2(.3, .1)));

	components.push_back(new Button(glm::vec3(0 - (.3 / 2.0), -.30, 0),
		glm::vec3(.4, .4, .4), "LOAD", glm::vec2(.3, .1)));

	components.push_back(new TextField(glm::vec3(0 - (.3 / 2.0), .1, 0),
		glm::vec3(.4, .4, .4), "SELECT NEW OR LOAD", glm::vec2(.3, .1), 1.0f));
}

void LoadPage::render(Renderer* renderer) {
	Page::render(renderer);
}

void LoadPage::tick() {}

void LoadPage::mouseClick(glm::vec2 mouseClick) {
	for (unsigned int i = 0; i < components.size(); i++) {
		if (components[i]->type == ComponentType::BUTTON) {
			if (((Button*)components[i])->inbound(mouseClick)) {
				if (((Button*)components[i])->text == "MENU") {
					nextPage = new MenuPage();
				}
				if (((Button*)components[i])->text == "NEW") {
					nextPage = new GamePage();
				}
				if (((Button*)components[i])->text == "LOAD") {
					nextPage = new GamePage();
				}
			}
		}
	}
}

void LoadPage::keyPress(int key) {}