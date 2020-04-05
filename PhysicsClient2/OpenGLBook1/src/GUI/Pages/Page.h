#pragma once
#include <vector>

#include "../../Graphics/Renderer.h"
#include "../Component/Component.h"
#include "../Component/Button.h"
#include "../Component/TextField.h"
#include "../Component/TextInput.h"
#include "../Component/RadioButtonSet.h"
#include "../Component/ListView.h"

enum PageType { Intro, Menu, Info, Connect, Load, Game, Setting};

class Page {
public:

	PageType type;

	std::vector<Button> buttons;

	std::vector<Component*> components;

	Page* nextPage;

	//constructor/destructor?

	virtual void tick();

	virtual void render(Renderer* renderer);

	//should this be virtual
	virtual void mouseHover(glm::vec2 mouseCoord);

	virtual void mouseClick(glm::vec2 mouseCoord) = 0;

	virtual void keyPress(int key) = 0;

};