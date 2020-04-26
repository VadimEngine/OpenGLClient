#include "GamePage.h"


GamePage::GamePage() {
	this->type = Game;
	nextPage = nullptr;
	//gameHUD = new GameHUD();
}

void GamePage::render(Renderer* renderer) {
	Page::render(renderer);
	gameHUD->render(renderer);
	//Render HUD
	//renderer->renderRectangleColor(glm::vec3(.7,1,0), .3, 2, glm::vec3(.5,.5,.5));
}

void GamePage::tick() {
	Page::tick();
	gameHUD->tick();
}

void GamePage::mouseClick(glm::vec2 mouseCoord) {
	for (int i = 0; i < components.size(); i++) {
		if (components[i]->type == ComponentType::BUTTON) {
			if (((Button*)components[i])->inbound(mouseCoord)) {
				if (((Button*)components[i])->text == "MENU") {
					nextPage = new MenuPage();
				}
			}
		}
	}

	gameHUD->mouseClick(mouseCoord);
}

void GamePage::mouseHover(glm::vec2 mouseCoord) {
	Page::mouseHover(mouseCoord);
	gameHUD->mouseHover(mouseCoord);

}

void GamePage::keyPress(int key) {
}