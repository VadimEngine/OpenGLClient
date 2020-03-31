#include "GamePage.h"


GamePage::GamePage() {
	this->type = Game;
	nextPage = nullptr;
	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .60, 0),
		glm::vec3(.4, .4, .4), "MENU", glm::vec2(.3, .1)));
}

void GamePage::render(Renderer* renderer) {
	Page::render(renderer);
	renderer->renderString("GAME HERE", -.3, 0);

	//Render HUD
	//renderer->renderRectangleColor(glm::vec3(.7,1,0), .3, 2, glm::vec3(.5,.5,.5));

}

void GamePage::tick() {

}

void GamePage::mouseClick(glm::vec2 mouseClick) {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].inbound(mouseClick) && buttons[i].text == "MENU") {
			nextPage = new MenuPage();
		}
	}
}