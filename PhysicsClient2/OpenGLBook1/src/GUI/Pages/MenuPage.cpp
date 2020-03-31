#include "MenuPage.h"


MenuPage::MenuPage() {
	this->type = Menu;
	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .60, 0),
		glm::vec3(.4, .4, .4), "START", glm::vec2(.3, .1)));

	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .40, 0),
		glm::vec3(.4, .4, .4), "CONNECT", glm::vec2(.3, .1)));

	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .20, 0),
		glm::vec3(.4, .4, .4), "INFO", glm::vec2(.3, .1)));

	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .0, 0),
		glm::vec3(.4, .4, .4), "EXIT", glm::vec2(.3, .1)));
}


void MenuPage::mouseClick(glm::vec2 mouseClick) {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].inbound(mouseClick) && buttons[i].text == "EXIT") {
			exit(0);
		} else 	if (buttons[i].inbound(mouseClick) && buttons[i].text == "START") {
			nextPage = new LoadPage();
		} else if (buttons[i].inbound(mouseClick) && buttons[i].text == "CONNECT") {
			nextPage = new ConnectPage();
		} else if (buttons[i].inbound(mouseClick) && buttons[i].text == "INFO") {
			nextPage = new InfoPage();
		}
	}
}

void MenuPage::render(Renderer* renderer) {
	Page::render(renderer);
}



void MenuPage::tick() {
	Page::tick();
}