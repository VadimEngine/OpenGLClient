#pragma once
#include "Component.h"

class TextField : public Component{
public:

	std::string text;
	glm::vec2 dimension;
	float size;

	//array of args that can be set outside so the entire string does not need
	//to be rebuilt everytime


	TextField(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension, float size);



	void tick() override;

	void render(Renderer* renderer) override;


};