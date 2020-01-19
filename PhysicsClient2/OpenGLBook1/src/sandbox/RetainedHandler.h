#pragma once
#include "RetainedObj.h"
#include "SandboxShader.h"
#include "RetainedRenderer.h"

#include <vector>




class RetainedHandler {
public:
	RetainedObj* player;
	RetainedRenderer* renderer;
	std::vector<RetainedObj*> objs;

	GLboolean keys[1024];
	//list of other objs

public:
	RetainedHandler(int count);
	~RetainedHandler();

	void tick(GLfloat dt);
	void render();

	void addObj(RetainedObj* obj);

	void collide(RetainedObj* obj1, RetainedObj* obj2);

};



