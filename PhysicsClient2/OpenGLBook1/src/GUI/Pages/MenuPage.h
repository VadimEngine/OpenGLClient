#pragma once
#include "Page.h"
#include "GamePage.h"
#include "InfoPage.h"
#include "LoadPage.h"
#include "ConnectPage.h"
#include "SettingPage.h"

class MenuPage :public Page {
public:

	MenuPage();

	void mouseClick(glm::vec2 mouseClick) override;

	void tick() override;

	void render(Renderer* renderer) override;

	void keyPress(int key) override;

};
