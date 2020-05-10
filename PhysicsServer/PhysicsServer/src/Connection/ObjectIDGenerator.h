#pragma once
#include <unordered_set>

class ObjectIDGenerator {
public:
	//maybe tree/orderd set would be better so "holes"
	//in ids can be filled?
	//might need to use a map<id, obj> so the object can be received from
	//the id
	static std::unordered_set<int> idSet;
	static int lastId;

	static int getNewId();
};