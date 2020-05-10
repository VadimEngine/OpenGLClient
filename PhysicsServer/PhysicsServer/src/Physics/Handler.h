#pragma once
#include <vector>
#include <tuple>

#include "Particle.h"
#include "../Connection/ConnectionConstants.h"

#include <mutex>
#include <math.h>

class Handler {
public:
	std::mutex mu;
	std::vector<Particle*> objs;
	std::vector<Particle*> clientObjs;

	std::string worldName;

	bool gravtiy;
	bool paused;

	Handler(int pCount, std::string worldName);

	~Handler();

	/**
	Method to update all particles in objs and clientObjs
	*/
	void update(double dt);

	void add(float x, float y);

	void addClient(Particle* p);

	//might need to return a pointer to a vector
	std::vector<float> getSendData();


	static void UpdatePhysics(Handler* handler);

	void removeParticle(int id);

};
