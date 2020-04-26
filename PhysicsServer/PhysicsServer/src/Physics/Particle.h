#pragma once
#include <cstdlib>
#include <iostream>
#include "../Connection/ObjectIDGenerator.h"
#include "Color.h"

class Particle {
public:
	float x, y;// coordinates between -(1,-1) and (1,1)
	float velx, vely;

	float oldx, oldy;

	//object id
	//might get away with sending pointer address but that
	//sounds like a security risk...
	int id;
	Color* color;
	float radius;
	

	bool isDelete;
	bool isPlayer;
	//radius
	Particle(float x, float y, bool isPlayer);
	Particle(float x, float y, bool isPlayer, Color* color, float radius);

	void update(double dt);
};
