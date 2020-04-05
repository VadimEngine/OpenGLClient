#pragma once
#include "Component.h"

class TextInput :public Component{

public:
	//for flicker?
	int counter;
	int countLimit;
	int increment;


	std::string text;
	glm::vec2 dimension;

	bool selected;

	//for typing
	int keyTypeCounter;
	int lastKey;


	TextInput(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension);

	void tick() override;

	void render(Renderer* renderer) override;

	void addKey(int key);

	bool mouseClick(glm::vec2 mouseCoord);

};