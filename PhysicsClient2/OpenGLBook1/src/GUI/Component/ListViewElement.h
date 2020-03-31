#pragma once
#include "Component.h"


class ListViewElement : public Component {
public:

	//selected (might just have a boolean parameter in the render method)
	//high lighed
	glm::vec2 dimension;
	std::string text;
	bool hover;

	ListViewElement(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension);

	void tick() override;

	void render(Renderer* renderer) override;

	void mouseHover(glm::vec2 mouseCoord) override;

	void renderSelected(Renderer* renderer);

};
