#pragma once
#include "GameObject.h"
#include "../Graphics/SandboxShader.h"
#include "../Graphics/Renderer.h"

#include <vector>




class Handler {
public:
	GameObject* player;
	Renderer* renderer;
	std::vector<GameObject*> objs;
	bool isServer;

	GLboolean keys[1024];
	//list of other objs

public:
	Handler(int count);
	~Handler();

	void tick(GLfloat dt);
	void render();

	void addObj(GameObject* obj);

	void collide(GameObject* obj1, GameObject* obj2);

};



