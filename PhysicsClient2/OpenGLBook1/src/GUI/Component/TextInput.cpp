#include "TextInput.h"

TextInput::TextInput(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension) 
:Component::Component(TEXTINPUT, position, color) {
	this->text = text;
	this->dimension = dimension;
	this->selected = false;

	counter = 0;
	countLimit = 60;
	increment = 1;
}


void TextInput::tick() {
	Component::tick();
}

void TextInput::render(Renderer* renderer) {
	Component::render(renderer);
	counter += increment;
	//counter = (counter + 1) % countLimit;
	//draw rectangle frame
	renderer->renderRectangleColor(position + glm::vec3(-.01, .01, 0), dimension.x + .02, dimension.y + .02, glm::vec3(1, 1, 1));

	//render fill
	renderer->renderRectangleColor(position, dimension.x, dimension.y, color);

	renderer->renderString(text, position.x, position.y);

	//draw caret if selected
	if (selected) {
		if (increment > 0) {
			GLfloat tempX = (text.size() * (.06 / 2.0));
			renderer->renderLineColor(position + glm::vec3(tempX + .01, 0, 0), position - glm::vec3(-.01 - tempX, .1, 0), glm::vec3(1, 1, 1), .01);
		}
	}
	//should be in tick
	if (counter > countLimit || counter < 0) {
		increment *= -1;
	}
}

//Add single key at first and if held for a second, then add freely
void TextInput::addKey(int key) {
	std::cout << "Keyinput: " << (int)key << std::endl;
	if (key == 259) {
		//std::cout << "Delete" << std::endl;
		if (text.size() != 0) {
			text.pop_back();
		}
		
	} else {
		text += key;
		std::cout << "text: " << text << std::endl;
	}
	
}

bool TextInput::mouseClick(glm::vec2 mousePos) {
	if (mousePos.x > position.x&& mousePos.x < position.x + dimension.x
		&& mousePos.y < position.y && mousePos.y > position.y - dimension.y) {
		return true;
	} else {
		return false;
	}
}