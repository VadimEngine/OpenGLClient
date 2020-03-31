#pragma once
#include "Component.h"
#include "ListViewElement.h"

class ListView : public Component{
public:

	//ListViewElement selectedElement
	glm::vec2 dimension;
	std::vector<ListViewElement*> elements;

	ListViewElement* selectedElement;

	ListView(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension);

	void tick() override;

	void render(Renderer* renderer) override;

	void addElement(std::string elementText);

	void mouseHover(glm::vec2 mousePos) override;

	void mouseClick(glm::vec2 mousePos);
};
