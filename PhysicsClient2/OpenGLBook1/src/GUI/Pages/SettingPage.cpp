#include "SettingPage.h"


SettingPage::SettingPage() {
	this->type = PageType::Setting;

	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .60, 0),
		glm::vec3(.4, .4, .4), "MENU", glm::vec2(.3, .1)));

	components.push_back(new TextField(glm::vec3(0 - (.3 / 2.0), .8, 0),
		glm::vec3(.4, .4, .4), "SETTINGS", glm::vec2(.3, .1), 1.0f));

}

void SettingPage::mouseClick(glm::vec2 mouseClick) {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].inbound(mouseClick) && buttons[i].text == "MENU") {
			nextPage = new MenuPage();
		}
	}
}

void SettingPage::tick() {
	Page::tick();
}

void SettingPage::render(Renderer* renderer) {
	Page::render(renderer);
}

void SettingPage::keyPress(int key) {
}