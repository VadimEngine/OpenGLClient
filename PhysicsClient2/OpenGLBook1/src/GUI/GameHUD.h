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

class Handler;


/// <summary>
/// Heads up Display during game.
/// </summary>
///<remarks>
/// Include TextField* serverClientsCount, serverIpInfo,
/// clientIpInfo, clientIdInfo;
///</remarks>
class GameHUD {
private:
	glm::vec3 position;

	std::vector<Component*> components;

	Button* highlightedButton;

	TextField* serverConnectionInfo;

	TextField* serverNameInfo;

	TextField* particlesInfo;

	TextField* playerInfo;

	TextField* selectedInfo;

	std::string serverName;

	Handler* handler;

public:
	GameHUD(Handler* handler);

	~GameHUD();

	void tick();

	void render(Renderer* renderer);

	void mouseHover(glm::vec2 mouseCoord);

	//make it take in a function? or pass in a void* data
	//that can be used to update data
	void mouseClick(glm::vec2 mouseCoord);

	void setServerName(std::string serverName);
};
