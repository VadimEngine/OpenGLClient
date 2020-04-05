#pragma once
#include "Page.h"
#include "MenuPage.h"


class InfoPage :public Page {
public:

	InfoPage();

	void mouseClick(glm::vec2 mouseClick) override;

	void tick() override;

	void render(Renderer* renderer) override;

	void keyPress(int key) override;

};