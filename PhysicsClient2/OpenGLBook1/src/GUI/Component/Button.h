#pragma once
#include "../../Graphics/Renderer.h"

class Button {
private:
	bool highLight;
public:

	glm::vec3 position;
	glm::vec3 color;
	std::string text;
	glm::vec2 dimension;
	

	Button(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension);

	void tick();

	void render(Renderer* renderer);

	void clickAction();

	bool inbound(glm::vec2 coord);

	void setHighlight(bool value);

};
