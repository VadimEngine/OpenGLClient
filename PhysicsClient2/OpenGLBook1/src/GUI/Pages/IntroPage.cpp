#include "IntroPage.h"

IntroPage::IntroPage() {
	this->type = Intro;
	countSize = 60 * 3;//3 seconds
	counter = 0;

	components.push_back(new TextField(glm::vec3(0 - (.3 / 2.0), .1, 0),
		glm::vec3(.4, .4, .4), "VADIM GAMES", glm::vec2(.3, .1), 1.0f));
}

void IntroPage::mouseClick(glm::vec2 mouseClick) {
	//Click to skip intro
	nextPage = new MenuPage();
}

void IntroPage::render(Renderer* renderer) {
	Page::render(renderer);
}

void IntroPage::tick() {
	if (counter <= countSize) {
		counter++;
	} else {
		//lazy load this instead?
		nextPage = new MenuPage();
	}
}

void IntroPage::keyPress(int key) {
}