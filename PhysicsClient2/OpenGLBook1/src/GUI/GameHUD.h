#pragma once
#include "../Graphics/Renderer.h"
#include "../GUI/Component/Button.h"
#include "../GUI/Component/Component.h"
#include "../GUI/Component/TextField.h"
#include "../Connection/Connection.h"
#include "../GUI/Pages/Page.h"
#include "../Engine/GameObject.h"
#include "../Engine/GameHandler.h"
//#include "../GUI/Pages/MenuPage.h"

//class MenuPage; //forward declarations

class GameHUD {
public:

	glm::vec3 position;

	//std::vector<Button*> buttons;

	std::vector<Component*> components;

	Button* highlightedButton;

	TextField* serverConnectionInfo;
	TextField* serverNameInfo;
	TextField* serverClientsCount;
	TextField* serverIpInfo;
	TextField* clientIpInfo;
	TextField* clientIdInfo;
	


	TextField* particlesInfo;
	TextField* playerInfo;
	TextField* selectedInfo;

	//placeholders
	Connection* connect;
	Page* nextPage;
	GameObject* player;
	GameObject** selectedObj;
	std::vector<GameObject*>* gameObjects;
	//maybe gamehandler should have a gameHud?
	GameHandler* gameHandler;

	bool toMenu;

	GameHUD(Connection* connect, std::vector<GameObject*>* gameObjects, GameObject* player, GameObject** selectedObj, GameHandler* gameHandler);

	void tick();

	void render(Renderer* renderer);

	void mouseHover(glm::vec2 mouseCoord);


	//make it take in a function? or pass in a void* data
	//that can be used to update data
	void mouseClick(glm::vec2 mouseCoord);

	/*
	void mouse action
	void key action
	*/

};
