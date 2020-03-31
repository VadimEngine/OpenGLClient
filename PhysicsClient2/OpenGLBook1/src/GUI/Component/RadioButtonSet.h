#pragma once
#include "Component.h"

class RadioButtonSet :public Component{
public:
	//number of options

	RadioButtonSet(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension);

	void tick() override;

	void render(Renderer* renderer) override;

};