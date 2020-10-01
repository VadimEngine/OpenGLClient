#include "GameHUD.h"
#include "../Engine/Handler.h"

GameHUD::GameHUD(Handler* handler) {

	this->serverName = "";

	this->handler = handler;

	highlightedButton = nullptr;

	components.push_back(new Button(glm::vec3(.51, -.75, 0),
		glm::vec3(.4, .4, .4), "MENU", glm::vec2(.3, .05)));
	components.push_back(new Button(glm::vec3(.51, -.85, 0),
		glm::vec3(.4, .4, .4), "SAVE", glm::vec2(.3, .05)));

	components.push_back(new Button(glm::vec3(.15, -.71, 0),
		glm::vec3(.4, .4, .4), "PAUSE", glm::vec2(.3, .05)));

	//Make a radio button
	components.push_back(new Button(glm::vec3(-.44, -.95, 0),
		glm::vec3(.4, .4, .4), "GRAVITY", glm::vec2(.3, .05)));

	components.push_back(new Button(glm::vec3(.15, -.95, 0),
		glm::vec3(.4, .4, .4), "REMOVE", glm::vec2(.3, .05)));

	components.push_back(new Button(glm::vec3(.15, -.78, 0),
		glm::vec3(.4, .4, .4), "NEXT", glm::vec2(.3, .05)));

	serverConnectionInfo = new TextField(glm::vec3(-1, -.7, 0),
		glm::vec3(.4, .4, .4), "Server ", glm::vec2(.3, .1), .85f);

	if (handler->getConnection()->getMode() == ConnectionMode::SERVERLESS) {
		serverConnectionInfo->text = "Server Serverless";
	} else if (handler->getConnection()->getMode() == ConnectionMode::TCP) {
		serverConnectionInfo->text = "Server TCP";
	} else if (handler->getConnection()->getMode() == ConnectionMode::UDP) {
		serverConnectionInfo->text = "Server UDP";
	}

	serverNameInfo = new TextField(glm::vec3(-1, -.75, 0),
		glm::vec3(.4, .4, .4), "Server Name:", glm::vec2(.3, .1), .85f);

	components.push_back(serverNameInfo);

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

	//particle position
	selectedInfo = new TextField(glm::vec3(-.44, -.9, 0),
		glm::vec3(.4, .4, .4), "Particle", glm::vec2(.3, .1), .85f);

	components.push_back(selectedInfo);

	if (handler->getConnection()->getMode() == ConnectionMode::UDP) {
		std::string theServerName = handler->getConnection()->getUDPConnect()->getServerName();
		//gameHUD is still null at this point...
		setServerName(theServerName);
	}
}

GameHUD::~GameHUD() {
	for (int i = 0; i < components.size(); i++) {
		delete components[i];
	}
}

void GameHUD::tick() {
	for (Component* comp : components) {
		comp->tick();
	}
	particlesInfo->text = "Particles " + std::to_string(handler->getGameHander()->getObjs()->size());

	playerInfo->text = "Player " + std::to_string(handler->getGameHander()->getPlayer()->position.x) + ", " + std::to_string(handler->getGameHander()->getPlayer()->position.y);

	if (handler->getGameHander()->getSelectedObj() != nullptr) {
		selectedInfo->text = "Particle " + std::to_string(handler->getGameHander()->getSelectedObj()->position.x ) + ", " + std::to_string(handler->getGameHander()->getSelectedObj()->position.y);
	} else {
		selectedInfo->text = "Particle ";
	}

	if (handler->getConnection()->getMode() != ConnectionMode::SERVERLESS) {
		if (handler->getGameHander()->getServerObjs()->find(handler->getGameHander()->getSelectedId()) != handler->getGameHander()->getServerObjs()->end()) {
			GameObject* theObj = handler->getGameHander()->getServerObjs()->find(handler->getGameHander()->getSelectedId())->second;
			selectedInfo->text = "Particle " + std::to_string((theObj)->position.x) + ", " + std::to_string((theObj)->position.y);
		} else {
			selectedInfo->text = "Particle ";
		}
	}

	//keep updating for now, later keep variables in components
	serverNameInfo->text = "Server Name: " + serverName;
}

void GameHUD::render(Renderer* renderer) {
	renderer->renderRectangleColor(glm::vec3(-1, -.7f, 0),
									2, .3,
									glm::vec3(.5f, .5f, .5f));
	
	renderer->renderLineColor(glm::vec3(-.45, -.7, 0), glm::vec3(-.45, -1, 0), glm::vec3(.4, .4, .4), .01);

	renderer->renderLineColor(glm::vec3(.5, -.7, 0), glm::vec3(.5, -1, 0), glm::vec3(.4, .4, .4), .01);


	for (unsigned int i = 0; i < components.size(); i++) {
		if (components[i] == highlightedButton) {
			components[i]->renderHighlighted(renderer);
		} else {
			components[i]->render(renderer);
		}
	}
}

void GameHUD::mouseHover(glm::vec2 mouseCoord) {
	bool highlighted = false;
	for (unsigned int i = 0; i < components.size(); i++) {
		if (components[i]->type == ComponentType::BUTTON) {
			if (((Button*)components[i])->inbound(mouseCoord)) {
				highlightedButton = (Button*)components[i];
				highlighted = true;
			}
		}
		if (!highlighted) {
			highlightedButton = nullptr;
		}
	}
}

//make it take in a function? or pass in a void* data
//that can be used to update data
void GameHUD::mouseClick(glm::vec2 mouseCoord) {
	for (unsigned int i = 0; i < components.size(); i++) {
		//Check inbound once, then do if else on the text/type
		if (((Button*)components[i])->inbound(mouseCoord)) {
			if (((Button*)components[i])->text == "MENU") {
				//nextPage = new MenuPage();
				//toMenu = true;
				handler->setCurrentPage(new MenuPage());
			} else if (((Button*)components[i])->text == "SAVE") {
				std::cout << "Save game" << std::endl;
			} else if (((Button*)components[i])->text == "NEXT") {
				handler->getGameHander()->selectNext();
				handler->getGameHander()->selectNextServer();
				std::cout << "Next: " << handler->getGameHander()->getSelectedId() << std::endl;
			} else if (((Button*)components[i])->text == "REMOVE") {
				if (handler->getConnection()->getMode() != ConnectionMode::SERVERLESS) {
					handler->getGameHander()->removeParticleServer(handler->getGameHander()->getSelectedId());
				} else {
					handler->getGameHander()->removeParticle(handler->getGameHander()->getSelectedObj());
				}
			} else if (((Button*)components[i])->text == "PAUSE") {
				handler->getGameHander()->setPause(!handler->getGameHander()->isPaused());
				//if connected, send
				if (handler->getConnection()->getMode() != ConnectionMode::SERVERLESS) {
					float tosend[] = {ConnectionConstants::CLIENT_PAUSE, handler->getGameHander()->isPaused()};
					handler->getConnection()->sendData(tosend, sizeof(float)*2);
				}
			} else if (((Button*)components[i])->text == "GRAVITY") {
				handler->getGameHander()->setGravity(!handler->getGameHander()->gravityEnabled());
				//if connected, send
				if (handler->getConnection()->getMode() != ConnectionMode::SERVERLESS) {
					float tosend[] = { ConnectionConstants::CLIENT_GRAVITY, handler->getGameHander()->gravityEnabled() };
					handler->getConnection()->sendData(tosend, sizeof(float) * 2);
				}
			}
		}
	}
}

void GameHUD::setServerName(std::string serverName) {
	this->serverName = serverName;
}