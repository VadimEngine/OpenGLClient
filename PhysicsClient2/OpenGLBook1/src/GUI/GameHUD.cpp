#include "GameHUD.h"

GameHUD::GameHUD(Connection* connect, std::vector<GameObject*>* gameObjects, GameObject* player, GameObject** selectedObj, GameHandler* gameHandler) {
	buttons.push_back(Button(glm::vec3(.51, -.75, 0),
		glm::vec3(.4, .4, .4), "MENU", glm::vec2(.3, .05)));
	buttons.push_back(Button(glm::vec3(.51, -.85, 0),
		glm::vec3(.4, .4, .4), "SAVE", glm::vec2(.3, .05)));

	buttons.push_back(Button(glm::vec3(.15, -.71, 0),
		glm::vec3(.4, .4, .4), "PAUSE", glm::vec2(.3, .05)));

	//Make a radio button
	buttons.push_back(Button(glm::vec3(-.44, -.95, 0),
		glm::vec3(.4, .4, .4), "GRAVITY", glm::vec2(.3, .05)));

	buttons.push_back(Button(glm::vec3(.15, -.95, 0),
		glm::vec3(.4, .4, .4), "REMOVE", glm::vec2(.3, .05)));

	buttons.push_back(Button(glm::vec3(.15, -.78, 0),
		glm::vec3(.4, .4, .4), "NEXT", glm::vec2(.3, .05)));

	serverConnectionInfo = new TextField(glm::vec3(-1, -.7, 0),
		glm::vec3(.4, .4, .4), "Server ", glm::vec2(.3, .1), .85f);

	if (connect->theMode == ConnectionMode::Serverless) {
		serverConnectionInfo->text = "Server Serverless";
	} else if (connect->theMode == ConnectionMode::TCP) {
		serverConnectionInfo->text = "Server TCP";
	} else if (connect->theMode == ConnectionMode::UDP) {
		serverConnectionInfo->text = "Server UDP";
	}

	components.push_back(new TextField(glm::vec3(-1, -.75, 0),
		glm::vec3(.4, .4, .4), "Server Name", glm::vec2(.3, .1), .85f));

	components.push_back(new TextField(glm::vec3(-1, -.80, 0),
		glm::vec3(.4, .4, .4), "Server Ip", glm::vec2(.3, .1), .85f));

	serverClientsCount = new TextField(glm::vec3(-1, -.85, 0),
		glm::vec3(.4, .4, .4), "Clients", glm::vec2(.3, .1), .85f);

	components.push_back(serverClientsCount);

	components.push_back(new TextField(glm::vec3(-1, -.90, 0),
		glm::vec3(.4, .4, .4), "Client IP", glm::vec2(.3, .1), .85f));

	components.push_back(new TextField(glm::vec3(-1, -.95, 0),
		glm::vec3(.4, .4, .4), "Client ID", glm::vec2(.3, .1), .85f));

	components.push_back(serverConnectionInfo);

	components.push_back(new TextField(glm::vec3(-.44, -.7, 0),
		glm::vec3(.4, .4, .4), "Environment", glm::vec2(.3, .1), .85f));

	components.push_back(new TextField(glm::vec3(.51, -.7, 0),
		glm::vec3(.4, .4, .4), "Menu", glm::vec2(.3, .1), .85f));



	//Number of particles

	particlesInfo = new TextField(glm::vec3(-.44, -.75, 0),
		glm::vec3(.4, .4, .4), "Particles", glm::vec2(.3, .1), .85f);
	components.push_back(particlesInfo);
	//player position

	playerInfo = new TextField(glm::vec3(-.44, -.85, 0),
		glm::vec3(.4, .4, .4), "Player: ", glm::vec2(.3, .1), .85f);

	components.push_back(playerInfo);

	selectedInfo = new TextField(glm::vec3(-.44, -.9, 0),
		glm::vec3(.4, .4, .4), "Particle", glm::vec2(.3, .1), .85f);

	//paricle position
	components.push_back(selectedInfo);

	//nextPage = nullptr;
	this->connect = connect;
	this->gameObjects = gameObjects;
	this->player = player;
	this->selectedObj = selectedObj;
	this->gameHandler = gameHandler;
	toMenu = false;

}

void GameHUD::tick() {
	for (Button button : buttons) {
		button.tick();
	}
	particlesInfo->text = "Particles " + std::to_string(gameObjects->size());

	playerInfo->text = "Player " + std::to_string(player->position.x) + ", " + std::to_string(player->position.y);

	if (selectedObj != nullptr && *selectedObj != nullptr) {
		selectedInfo->text = "Particle " + std::to_string( (*selectedObj)->position.x ) + ", " + std::to_string((*selectedObj)->position.y);
	} else {
		selectedInfo->text = "Particle ";
	}
}

void GameHUD::render(Renderer* renderer) {
	renderer->renderRectangleColor(glm::vec3(-1, -.7, 0),
									2, .3,
									glm::vec3(.5, .5, .5));
	
	renderer->renderLineColor(glm::vec3(-.45, -.7, 0), glm::vec3(-.45, -1, 0), glm::vec3(.4, .4, .4), .01);

	renderer->renderLineColor(glm::vec3(.5, -.7, 0), glm::vec3(.5, -1, 0), glm::vec3(.4, .4, .4), .01);

	for (Button button : buttons) {
		button.render(renderer);
	}
	for (int i = 0; i < components.size(); i++) {
		components[i]->render(renderer);
	}
}

void GameHUD::mouseHover(glm::vec2 mouseCoord) {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].inbound(mouseCoord)) {
			buttons[i].setHighlight(true);
		} else {
			buttons[i].setHighlight(false);
		}
	}
}

//make it take in a function? or pass in a void* data
//that can be used to update data
void GameHUD::mouseClick(glm::vec2 mouseCoord) {
	for (int i = 0; i < buttons.size(); i++) {
		//Check inbound once, then do if else on the text/type
		if (buttons[i].inbound(mouseCoord) && buttons[i].text == "MENU") {
			//nextPage = new MenuPage();
			toMenu = true;
		} else if (buttons[i].inbound(mouseCoord) && buttons[i].text == "SAVE") {
			std::cout << "Save game" << std::endl;
		} else if (buttons[i].inbound(mouseCoord) && buttons[i].text == "NEXT") {
			gameHandler->selectNext();
		} else if (buttons[i].inbound(mouseCoord) && buttons[i].text == "REMOVE") {
			gameHandler->removeParticle(gameHandler->selectedObj);
		} else if (buttons[i].inbound(mouseCoord) && buttons[i].text == "PAUSE") {
			gameHandler->setPause(!gameHandler->paused);
		} else if (buttons[i].inbound(mouseCoord) && buttons[i].text == "GRAVITY") {
			gameHandler->setGravity(!gameHandler->gravity);
		}
	}
}