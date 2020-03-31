#include "LoadPage.h"

LoadPage::LoadPage() {
	this->type = Load;
	nextPage = nullptr;
	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .60, 0),
		glm::vec3(.4, .4, .4), "MENU", glm::vec2(.3, .1)));

	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .30, 0),
		glm::vec3(.4, .4, .4), "NEW", glm::vec2(.3, .1)));

	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), -.30, 0),
		glm::vec3(.4, .4, .4), "LOAD", glm::vec2(.3, .1)));

}

void LoadPage::render(Renderer* renderer) {
	Page::render(renderer);
	renderer->renderString("SELECT NEW OR LOAD", -.3, 0);
}

void LoadPage::tick() {

}

void LoadPage::mouseClick(glm::vec2 mouseClick) {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].inbound(mouseClick) && buttons[i].text == "MENU") {
			nextPage = new MenuPage();
		}
		if (buttons[i].inbound(mouseClick) && buttons[i].text == "NEW") {
			nextPage = new GamePage();
		}
		if (buttons[i].inbound(mouseClick) && buttons[i].text == "LOAD") {
			nextPage = new GamePage();
		}
	}
}