#include "Handler.h"
#include <mutex>
#include <math.h>

std::mutex mu;

Handler::Handler(int pCount) {
	for (int i = 0; i < pCount; i++) {
		float c1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float c2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		objs.push_back(new Particle(c1, c2, false));
	}
}

Handler::~Handler() {
	for (Particle* p: objs) {
		delete p;
	}
	for (Particle* p : clientObjs) {
		delete p;
	}
	objs.clear();
	clientObjs.clear();

}

void Handler::update(double dt) {//update properly, float dt?
	//Move and collide particles
	for (int i = objs.size() - 1; i >= 0; i--) {
		if (objs[i]->isDelete) {
			objs.erase(objs.begin() + i);
		} else {
			objs[i]->update(dt);
		}
	}

	for (int i = clientObjs.size() - 1; i >= 0; i--) {
		if (clientObjs[i]->isDelete) {
			Particle* p = clientObjs[i];
			clientObjs.erase(clientObjs.begin() + i);
			delete p;
		} else {
			clientObjs[i]->update(dt);
		}
	}

	//collide process
	for (int i = 0; i < objs.size(); i++) {
		Particle* p1 = objs[i];
		for (int j = i + 1; j < objs.size(); j++) {
			Particle* p2 = objs[j];

			float distance = sqrt( pow(p2->x - p1->x,2) + pow(p2->y - p1->y, 2));
			if (distance < .05f) {
				//swap velocities
				float tempXvel = p1->velx;
				float tempYvel = p1->vely;
				p1->velx = p2->velx;
				p1->vely = p2->vely;
				p2->velx = tempXvel;
				p2->vely = tempYvel;
			}
		}
		//client enviornment reaction
		for (int j = 0; j < clientObjs.size(); j++) {
			Particle* p2 = clientObjs[j];
			float distance = sqrt(pow(p2->x - p1->x, 2) + pow(p2->y - p1->y, 2));
			if (distance < .05f) {
				p1->velx = p2->velx;
				p1->vely = p2->vely;
			}
		}

	}

}

void Handler::add(float x, float y) {
	objs.push_back(new Particle(x, y, false));
}

//return the index of the posistion in this vector as an ID which can later be used to remove
void Handler::addClient(Particle* p) {
	clientObjs.push_back(p);
}


/*Retruns a float array of the coordinates of the particles in the simulator
 */
float* Handler::getSendData() {
	mu.lock();
	float* temp = new float[(objs.size() * 2) + (clientObjs.size() * 2)];

	for (int i = 0; i < objs.size(); i++) {
		temp[2 * i] = objs[i]->x;
		temp[2 * i + 1] = objs[i]->y;
	}
	for (int i = 0; i < clientObjs.size(); i++) {
		temp[(objs.size() * 2) + (2 * i)] = clientObjs[i]->x;
		temp[(objs.size() * 2) + (2 * i) + 1] = clientObjs[i]->y;
	}
	mu.unlock();

	return temp;
}

std::tuple<int, float*> Handler::getSendData2() {
	mu.lock();
	int tempSize = (objs.size() * 2) + (clientObjs.size() * 2);
	float* temp = new float[(objs.size() * 2) + (clientObjs.size() * 2)];

	for (int i = 0; i < objs.size(); i++) {
		temp[2 * i] = objs[i]->x;
		temp[2 * i + 1] = objs[i]->y;
	}
	for (int i = 0; i < clientObjs.size(); i++) {
		temp[(objs.size() * 2) + (2 * i)] = clientObjs[i]->x;
		temp[(objs.size() * 2) + (2 * i) + 1] = clientObjs[i]->y;
	}
	mu.unlock();

	return std::make_tuple(tempSize, temp);
}
