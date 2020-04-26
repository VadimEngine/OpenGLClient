#include "ObjectIDGenerator.h"

int ObjectIDGenerator::lastId = 0;
std::unordered_set<int> ObjectIDGenerator::idSet{};

int ObjectIDGenerator::getNewId() {
	//increment lastId if it is in the set of used ids
	while (ObjectIDGenerator::idSet.find(++lastId) == idSet.end()) {
		idSet.insert(lastId);
		return lastId;
	}
	//unexpected error
	return -1;
}