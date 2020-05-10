#include "ConnectPage.h"

ConnectPage::ConnectPage() {
	this->type = PageType::CONNECT;
	components.push_back(new Button(glm::vec3(0 - (.3 / 2.0), .60, 0),
		glm::vec3(.4, .4, .4), "MENU", glm::vec2(.3, .1)));

	components.push_back(new Button(glm::vec3(0 - (.3 / 2.0), .30, 0),
		glm::vec3(.4, .4, .4), "CONNECT", glm::vec2(.3, .1)));

	ipInput = new TextInput(glm::vec3(0 - (.3 / 2.0), -.3, 0),
		glm::vec3(.4, .4, .4), "127.0.0.1", glm::vec2(.3, .1));
	components.push_back(ipInput);


	components.push_back(new TextField(glm::vec3(0 - (.6), -.3, 0),
		glm::vec3(.4, .4, .4), "SERVER IP", glm::vec2(.3, .1), 1.0f));

	idInput = new TextInput(glm::vec3(0 - (.3 / 2.0), -.6, 0),
		glm::vec3(.4, .4, .4), "1", glm::vec2(.3, .1));

	components.push_back(idInput);


	components.push_back(new TextField(glm::vec3(0 - (.6), -.6, 0),
		glm::vec3(.4, .4, .4), "USER ID", glm::vec2(.3, .1), 1.0f));

	connectModeInput = new ListView(glm::vec3(0 - (.3 / 2.0), .9, 0),
							glm::vec3(.4, .4, .4), "", glm::vec2(.3, .1*2));
	connectModeInput->addElement("TCP");
	connectModeInput->addElement("UDP");

	components.push_back(connectModeInput);

	connectStatus = new TextField(glm::vec3(0 - (.3 / 2.0), -.95, 0),
		glm::vec3(.4, .4, .4), "STATUS:", glm::vec2(.3, .1), 1.0f);

	components.push_back(connectStatus);

	components.push_back(new TextField(glm::vec3(0 - (.3 / 2.0), .1, 0),
		glm::vec3(.4, .4, .4), "CONNECTION", glm::vec2(.3, .1), 1.0f));

}

void ConnectPage::render(Renderer * renderer) {
	Page::render(renderer);
}

void ConnectPage::tick() {
	Page::tick();
}

void ConnectPage::mouseClick(glm::vec2 mouseClick) {
	for (unsigned int i = 0; i < components.size(); i++) {
		if (components[i]->type == LISTVIEW) {
			((ListView*)components[i])->mouseClick(mouseClick);
		} else if (components[i]->type == TEXTINPUT) {
			if (((TextInput*)components[i])->mouseClick(mouseClick)) {
				myInput = ((TextInput*)components[i]);
				((TextInput*)components[i])->selected = true;
			} else {
				((TextInput*)components[i])->selected = false;
			}
		} else if (components[i]->type == ComponentType::BUTTON) {
			if (((Button*)components[i])->inbound(mouseClick)) {
				if (((Button*)components[i])->text == "MENU") {
					nextPage = new MenuPage();
				}
				if (((Button*)components[i])->text == "CONNECT") {
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
					for (unsigned int i = 0; i < ip.length(); i++) {
						if (!(isdigit(ip[i]) || ip[i] == '.')) {
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
					if (validIp && (mode._Equal("TCP") || (mode._Equal("UDP") && validId))) {
						std::cout << "Connecting: " << mode << " ip: " << ip << " id: " << id << std::endl;
						doConnect = true;
						this->connectStatus->text = "STATUS: Connecting...";
						nextPage = new GamePage();
					} else {
						this->connectStatus->text = "STATUS: Invaid connect input";
					}
				}
			}
		}
	}
}

void ConnectPage::keyPress(int key) {
	//handle the timeing and if character should be added in here	
	if (myInput != nullptr) {
		myInput->addKey(key);
	}
}