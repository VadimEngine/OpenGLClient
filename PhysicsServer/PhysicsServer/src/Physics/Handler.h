#pragma once
#include <vector>
#include <tuple>

#include "Particle.h"
#include "../Connection/ConnectionConstants.h"

class Handler {
public:
	std::vector<Particle*> objs;
	std::vector<Particle*> clientObjs;

	std::string worldName;

	Handler(int pCount, std::string worldName);

	~Handler();

	/**
	Method to update all particles in objs and clientObjs
	*/
	void update(double dt);

	void add(float x, float y);

	void addClient(Particle* p);

	float* getSendData();

	//might need to return a pointer to a vector
	std::vector<float> getSendData3();

	std::tuple<int, float*> getSendData2();

	static void UpdatePhysics(Handler* handler);

	void removeParticle(int id);

};
