#pragma once
#include "Page.h"
#include "MenuPage.h"

class GamePage :public Page {
public:


	GamePage();

	void mouseClick(glm::vec2 mouseClick) override;

	void tick() override;

	void render(Renderer* renderer) override;

};