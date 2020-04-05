#include "MenuPage.h"


MenuPage::MenuPage() {
	this->type = Menu;

	components.push_back(new TextField(glm::vec3(0 - (.3 / 2.0), .80, 0),
		glm::vec3(.4, .4, .4), "MENU", glm::vec2(.3, .1), 1.0f));

	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .60, 0),
		glm::vec3(.4, .4, .4), "START", glm::vec2(.3, .1)));

	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .40, 0),
		glm::vec3(.4, .4, .4), "CONNECT", glm::vec2(.3, .1)));

	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .20, 0),
		glm::vec3(.4, .4, .4), "INFO", glm::vec2(.3, .1)));

	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .0, 0),
		glm::vec3(.4, .4, .4), "SETTINGS", glm::vec2(.3, .1)));

	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), -.20, 0),
		glm::vec3(.4, .4, .4), "EXIT", glm::vec2(.3, .1)));
}


void MenuPage::mouseClick(glm::vec2 mouseClick) {
	//Check inbound once, then do if else on the text/type
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].inbound(mouseClick) && buttons[i].text == "EXIT") {
			exit(0);
		} else 	if (buttons[i].inbound(mouseClick) && buttons[i].text == "START") {
			nextPage = new LoadPage();
		} else if (buttons[i].inbound(mouseClick) && buttons[i].text == "CONNECT") {
			nextPage = new ConnectPage();
		} else if (buttons[i].inbound(mouseClick) && buttons[i].text == "INFO") {
			nextPage = new InfoPage();
		} else if (buttons[i].inbound(mouseClick) && buttons[i].text == "SETTINGS") {
			nextPage = new SettingPage();
		}
	}
}

void MenuPage::render(Renderer* renderer) {
	Page::render(renderer);

	renderer->renderString("Menu2", glm::vec2(0, .8), .85);
}

void MenuPage::tick() {
	Page::tick();
}

void MenuPage::keyPress(int key) {
}