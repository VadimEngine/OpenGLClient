#pragma once
#include "Page.h"
#include "MenuPage.h"


class ConnectPage : public Page {
public:
	//Which to type
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

	//int parameter instead of char to hold asciis
	//larger than 256 such as delete (259 in glfw?)
	void KeyType(int key);

};
