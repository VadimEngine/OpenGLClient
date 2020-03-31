#pragma once
#include "Page.h"
#include "MenuPage.h"


class LoadPage : public Page {
public:

	LoadPage();

	void mouseClick(glm::vec2 mouseClick) override;

	void tick() override;

	void render(Renderer* renderer) override;
};
