#pragma once
#include <cstdlib>
#include <iostream>

class Particle {
public:
	float x, y;// coordinates between -(1,-1) and (1,1)
	float velx, vely;

	float oldx, oldy;

	bool isDelete;
	bool isPlayer;
	//radius
	Particle(float x, float y, bool isPlayer);

	void update(double dt);
};
