#include "RadioButtonSet.h"

RadioButtonSet::RadioButtonSet(glm::vec3 position, glm::vec3 color, std::string text, glm::vec2 dimension)
:Component::Component(RADIOBUTTONSET, position, position) {

}


void RadioButtonSet::tick() {

}

void RadioButtonSet::render(Renderer* renderer) {
	//Take position into account later, for now get list view working
	
	//render rectangle that fist all elements
	//need list element class?
	
	
	renderer->renderString("RADIOSET", glm::vec2(-1,1));//-1, 1
}