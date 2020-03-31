#include "ConnectPage.h"

ConnectPage::ConnectPage() {
	this->type = Connect;
	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .60, 0),
		glm::vec3(.4, .4, .4), "MENU", glm::vec2(.3, .1)));

	buttons.push_back(Button(glm::vec3(0 - (.3 / 2.0), .30, 0),
		glm::vec3(.4, .4, .4), "CONNECT", glm::vec2(.3, .1)));

	//myInput = new TextInput(glm::vec3(0 - (.3 / 2.0), -.3, 0),
		//glm::vec3(.4, .4, .4), "", glm::vec2(.3, .1));

	ipInput = new TextInput(glm::vec3(0 - (.3 / 2.0), -.3, 0),
		glm::vec3(.4, .4, .4), "", glm::vec2(.3, .1));
	components.push_back(ipInput);


	components.push_back(new TextField(glm::vec3(0 - (.6), -.3, 0),
		glm::vec3(.4, .4, .4), "SERVER IP", glm::vec2(.3, .1)));

	idInput = new TextInput(glm::vec3(0 - (.3 / 2.0), -.6, 0),
		glm::vec3(.4, .4, .4), "", glm::vec2(.3, .1));

	components.push_back(idInput);


	components.push_back(new TextField(glm::vec3(0 - (.6), -.6, 0),
		glm::vec3(.4, .4, .4), "USER ID", glm::vec2(.3, .1)));

	connectModeInput = new ListView(glm::vec3(0 - (.3 / 2.0), .9, 0),
							glm::vec3(.4, .4, .4), "", glm::vec2(.3, .1*2));
	connectModeInput->addElement("TCP");
	connectModeInput->addElement("UDP");

	components.push_back(connectModeInput);

	connectStatus = new TextField(glm::vec3(0 - (.3 / 2.0), -.95, 0),
		glm::vec3(.4, .4, .4), "STATUS: CANT CONNECT", glm::vec2(.3, .1));

	components.push_back(connectStatus);

}

void ConnectPage::render(Renderer * renderer) {
	Page::render(renderer);
	renderer->renderString("CONNECTION", -.3, 0);
	//myInput->render(renderer);
}


void ConnectPage::tick() {
	Page::tick();
}

void ConnectPage::mouseClick(glm::vec2 mouseClick) {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].inbound(mouseClick) && buttons[i].text == "MENU") {
			nextPage = new MenuPage();
		} 
		if (buttons[i].inbound(mouseClick) && buttons[i].text == "CONNECT") {
			std::string mode;// = connectModeInput->selectedElement->text;
			std::string ip = ipInput->text;
			std::string id = idInput->text;
			if (connectModeInput->selectedElement != nullptr) {
				mode = connectModeInput->selectedElement->text;
			} else {
				continue;
			}
			
			bool validIp = ip.size() != 0;

			char* p;
			long converted = strtol(id.c_str(), &p, 10);


			bool validId = *p == 0;
			if (validId) {
				std::cout << "Valid id" << converted << std::endl;
			} else {
				std::cout << "Invalid id" << std::endl;
			}
			//validate ip only consists of numbrs and '.'
			//can use regex later?
			for (int i = 0; i < ip.length(); i++) {
				if ( !(isdigit(ip[i]) || ip[i]=='.') ){
					validIp = false;
					break;
				}
			}

			if (validIp) {
				std::cout << "Valid Ip" << ip << std::endl;
			} else {
				std::cout << "Invlaid Ip" << ip << std::endl;
			}

			// confirm valid inputs
			if (validIp && (mode._Equal("TCP") || (mode._Equal("UDP") && validId)) ) {
			//if (true) {
			// do connection

				std::cout << "Connecting: " << mode << " ip: " << ip << " id: " << id << std::endl;
				doConnect = true;
				this->connectStatus->text = "Connecting...";
				nextPage = new GamePage();
			} else {
				this->connectStatus->text = "Invaid connect input";
			}

		}
	}
	for (int i = 0; i < components.size(); i++) {
		if (components[i]->type == LISTVIEW) {
			((ListView*)components[i])->mouseClick(mouseClick);
		} else if (components[i]->type == TEXTINPUT) {
			if (((TextInput*)components[i])->mouseClick(mouseClick)) {
				myInput = ((TextInput*)components[i]);
				((TextInput*)components[i])->selected = true;
			} else {
				((TextInput*)components[i])->selected = false;
			}
		}
	}



}

void ConnectPage::KeyType(int key) {

	if (myInput != nullptr) {
		myInput->addKey(key);
	}


	//myInput->addKey(key);
}