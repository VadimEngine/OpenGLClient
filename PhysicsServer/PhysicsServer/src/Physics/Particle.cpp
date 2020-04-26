#include "Particle.h"

Particle::Particle(float x, float y, bool isPlayer)
:Particle(x,y,isPlayer, new Color(1,0,0), .05f){
	//Particle::Particle(x, y, isPlayer, new Color(1,1,1), .05f);
}

Particle::Particle(float x, float y, bool isPlayer, Color* color, float radius)
:x(x), y(y), isPlayer(isPlayer), color(color), radius(radius){
	//this->color = color;
	//this->radius = radius;
	this->id = ObjectIDGenerator::getNewId();
	this->isPlayer = isPlayer;
	//this->x = x;
	//this->y = y;

	if (!isPlayer) {
		float c1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float c2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		this->velx = c1;
		this->vely = c2;
	} else {
		this->velx = 0;
		this->vely = 0;
		this->oldx = x;
		this->oldy = y;
	}

	isDelete = false;
}

void Particle::update(double dt) {//properly update, float dt
	if (!isPlayer) {
		x += velx * dt;
		y += vely * dt;

		vely -= .981 * dt;

		if (y < -.7) {
			y = -.7;
			vely *= -1 * .9;
		}

		if (y > 1) {
			y = 1;
			vely *= -1 * .9;
		}

		if (x < -1) {
			x = -1;
			velx *= -1 * .9;
		}

		if (x > 1) {
			x = 1;
			velx *= -1 * .9;
		}
	}
	if (isPlayer) {
		velx = (x - oldx) / dt;
		vely = (y - oldy) / dt;
		oldx = x;
		oldy = y;
	}

}