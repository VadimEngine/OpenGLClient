#include "IntroPage.h"

IntroPage::IntroPage() {
	this->type = Intro;
	countSize = 60 * 3;//3 seconds
	counter = 0;
}

void IntroPage::mouseClick(glm::vec2 mouseClick) {
	std::cout << "next page click" << std::endl;
	//Click to skip intro
	nextPage = new MenuPage();
}

void IntroPage::render(Renderer* renderer) {
	Page::render(renderer);
	renderer->renderString("VADIM GAMES", -.3, 0);

}



void IntroPage::tick() {
	//std::cout << "counter: " << counter << std::endl;
	//std::cout << "countSize: " << countSize << std::endl;
	if (counter <= countSize) {
		counter++;
	} else {
		std::cout << "Next page" << std::endl;
		//lazy load this instead?
		nextPage = new MenuPage();
	}
}