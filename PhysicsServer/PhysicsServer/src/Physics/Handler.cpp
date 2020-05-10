#include "Handler.h"

Handler::Handler(int pCount, std::string worldName)
:worldName(worldName){
	gravtiy = true;
	paused = false;

	for (int i = 0; i < pCount; i++) {
		float c1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float c2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		Particle* p = new Particle(c1, c2, false);
		objs.push_back(p);
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
			if (!paused) {
				objs[i]->update(dt);
				if (gravtiy) {
					objs[i]->vely -= .981 * dt;
				}
			}
		}
	}

	for (int i = clientObjs.size() - 1; i >= 0; i--) {
		if (clientObjs[i]->isDelete) {
			Particle* p = clientObjs[i];
			clientObjs.erase(clientObjs.begin() + i);
			delete p;
		} else {
			if (!paused) {
				clientObjs[i]->update(dt);
				if (gravtiy) {
					objs[i]->vely -= .981 * dt;
				}
				
			}
		}
	}

	//collide process
	for (int i = 0; i < objs.size(); i++) {
		Particle* p1 = objs[i];
		for (int j = i + 1; j < objs.size(); j++) {
			Particle* p2 = objs[j];
			//exception thrown where when client suddenly disconnects
			//check before going to this line
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

void Handler::removeParticle(int id) {
	//iterate and remove if exists for now
	for (int i = 0; i < objs.size(); i++) {
		if (objs[i]->id == id) {
			delete objs[i];
			objs.erase(objs.begin() + i);
			break;
		}
	}
}

//return the index of the posistion in this vector as an ID which can later be used to remove
void Handler::addClient(Particle* p) {
	clientObjs.push_back(p);
}

//might be faster (no need to convert back to array and
//no need to resize the vector when adding data)
std::vector<float> Handler::getSendData() {
	mu.lock();
	//set initial capaticy to avoid resizing
	//std::vector<float> theData((objs.size() * 7) + (clientObjs.size() * 7) + 1);
	std::vector<float> theData;

	theData.push_back(ConnectionConstants::SERVER_PARTICLE_POST);
	//first float(s) should indicate what this data is
	// such as 1=particles data and then also state how many fields?
	// but then the client code would need to be more dynamic to handle 
	//various number of fields...
	//id, radius, x, y, r, g, b ...
	for (int i = 0; i < objs.size(); i++) {
		theData.push_back(objs[i]->id);
		theData.push_back(objs[i]->radius);
		theData.push_back(objs[i]->x);
		theData.push_back(objs[i]->y);
		theData.push_back(objs[i]->color->r);
		theData.push_back(objs[i]->color->g);
		theData.push_back(objs[i]->color->b);
	}

	for (int i = 0; i < clientObjs.size(); i++) {
		theData.push_back(clientObjs[i]->id);
		theData.push_back(clientObjs[i]->radius);
		theData.push_back(clientObjs[i]->x);
		theData.push_back(clientObjs[i]->y);
		theData.push_back(clientObjs[i]->color->r);
		theData.push_back(clientObjs[i]->color->g);
		theData.push_back(clientObjs[i]->color->b);
	}

	mu.unlock();
	return theData;
}

void Handler::UpdatePhysics(Handler* handler) {
	auto last = std::chrono::high_resolution_clock::now();//get current time
	while (true) {
		double dt = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - last).count() / 1000000000.0;//get time difference
		if (dt >= 1.0 / 60.0) {//update if 1/60 seconds has passed in order to update 60 times a second
			handler->update(dt);
			last = std::chrono::high_resolution_clock::now();
		} else {
			std::this_thread::sleep_for(std::chrono::microseconds(1));//if too fast then sleep
		}
	}
}