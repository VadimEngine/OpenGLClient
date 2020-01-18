#pragma once
#include <vector>
#include <tuple>

#include "Particle.h"

class Handler {
public:
	std::vector<Particle*> objs;
	std::vector<Particle*> clientObjs;

	Handler(int pCount);

	~Handler();

	/**
	Method to update all particles in objs and clientObjs
	*/
	void update(double dt);

	void add(float x, float y);

	void addClient(Particle* p);

	float* getSendData();

	std::tuple<int, float*> getSendData2();

};