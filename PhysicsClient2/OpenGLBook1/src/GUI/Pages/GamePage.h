#pragma once
#include "../GameHUD.h"
#include "MenuPage.h"
#include "Page.h"

//class GameHUD;

class GamePage :public Page {
public:

	GameHUD* gameHUD;

	GamePage();

	void mouseClick(glm::vec2 mouseCoord) override;

	void tick() override;

	void render(Renderer* renderer) override;

	void keyPress(int key) override;

	void mouseHover(glm::vec2 mouseCoord) override;

};