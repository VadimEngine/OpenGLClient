#pragma once
#include "Component.h"

class TextField : public Component{
public:

	std::string text;
	glm::vec2 dimension;


	TextField(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension);



	void tick() override;

	void render(Renderer* renderer) override;


};