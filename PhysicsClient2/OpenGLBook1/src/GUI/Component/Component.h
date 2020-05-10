#pragma once
#include "../../Graphics/Renderer.h"

enum ComponentType { BUTTON, TEXTFIELD, LISTVIEW, LISTVIEWELEMENT, RADIOBUTTONSET, TEXTINPUT };


class Component {
public:
	const ComponentType type;//Should be constant/read only

	//selectable, clickable
	bool highLightable;

	glm::vec3 position;

	glm::vec3 color;

	Component(ComponentType type, glm::vec3 position, glm::vec3 color);
	
	virtual void render(Renderer* renderer);

	virtual void renderHighlighted(Renderer* renderer);

	virtual void tick();

	virtual void mouseHover(glm::vec2 mouseCoord);

	virtual void action();
};