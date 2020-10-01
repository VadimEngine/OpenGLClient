#include "TextInput.h"

TextInput::TextInput(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension) 
:Component::Component(ComponentType::TEXTINPUT, position, color) {
	this->text = text;
	this->dimension = dimension;
	this->selected = false;

	counter = 0;
	countLimit = 60;
	increment = 1;

	lastKey = 0;
	keyTypeCounter = 0;
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
	renderer->renderString(text, glm::vec2(position.x, position.y));

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

	if (keyTypeCounter != 0 && keyTypeCounter < 15) {
		keyTypeCounter = (keyTypeCounter + 1) % 15;
	}
}

//Add single key at first and if held for a second, then add freely
void TextInput::addKey(int key) {
	bool typeKey = false;
	if (keyTypeCounter == 0 || lastKey != key) {
		lastKey = key;
		keyTypeCounter = 1;
		typeKey = true;
	}

	if (typeKey) {
		if (key == 259) {
			if (text.size() != 0) {
				text.pop_back();
			}
		} else {
			text += key;
		}
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