#pragma once
#include "Page.h"
#include "MenuPage.h"


class ConnectPage : public Page {
public:
	//Which input component is selected, make abstract
	//to hold any input componenet
	TextInput* myInput;

	//User connection input
	TextInput* ipInput;
	TextInput* idInput;
	ListView* connectModeInput;

	TextField* connectStatus;

	bool doConnect = false;

	ConnectPage();

	void mouseClick(glm::vec2 mouseClick) override;

	void tick() override;

	void render(Renderer* renderer) override;

	void keyPress(int key) override;

	//int parameter instead of char to hold asciis
	//larger than 256 such as delete (259 in glfw?)
	//void KeyType(int key);
};
