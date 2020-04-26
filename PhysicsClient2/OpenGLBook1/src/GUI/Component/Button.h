#pragma once
#include "Component.h"
#include "../../Graphics/Renderer.h"


//TODO MERGE BUTTON AND COMPONENT

class Button : public Component {
public:

	//glm::vec3 position;
	//glm::vec3 color;
	std::string text;
	glm::vec2 dimension;
	

	Button(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension);

	void tick();

	void render(Renderer* renderer) override;

	void renderHighlighted(Renderer* renderer) override;

	void clickAction();

	bool inbound(glm::vec2 coord);
};
