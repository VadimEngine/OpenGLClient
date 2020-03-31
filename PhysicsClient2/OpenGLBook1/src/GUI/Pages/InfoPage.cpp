#include "InfoPage.h"

InfoPage::InfoPage() {
	this->type = Info;
	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .60, 0),
		glm::vec3(.4, .4, .4), "MENU", glm::vec2(.3, .1)));
}


void InfoPage::render(Renderer* renderer) {
	Page::render(renderer);
	renderer->renderString("INFORMATION HERE", -.3, 0);
}



void InfoPage::tick() {
	Page::tick();
}

void InfoPage::mouseClick(glm::vec2 mouseClick) {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].inbound(mouseClick) && buttons[i].text == "MENU") {
			nextPage = new MenuPage();
		}
	}
}