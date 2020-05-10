#pragma once
#include "Page.h"
#include "MenuPage.h"

class IntroPage : public Page {
public:
	int countSize;
	int counter;

	IntroPage();

	void mouseClick(glm::vec2 mouseClick) override;

	void tick() override;

	void render(Renderer* renderer) override;

	void keyPress(int key) override;
};