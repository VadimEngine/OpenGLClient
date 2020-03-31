#pragma once
#include <vector>

#include "../../Graphics/Renderer.h"
#include "../Component/Component.h"
#include "../Component/Button.h"
#include "../Component/TextField.h"
#include "../Component/TextInput.h"
#include "../Component/RadioButtonSet.h"
#include "../Component/ListView.h"

enum PageType { Intro, Menu, Info, Connect, Load, Game };

class Page {
public:

	PageType type;

	std::vector<Button> buttons;

	std::vector<Component*> components;

	Page* nextPage;

	virtual void tick();

	virtual void render(Renderer* renderer);

	void mouseHover(glm::vec2 mouseCoord);

	virtual void mouseClick(glm::vec2 mouseClick) = 0;

};