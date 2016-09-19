#include "resource.hpp"

Resource::Resource() {
}

Resource::~Resource() {
	observers.clear();
}

// called by the objects when created
// let's say some class has a color* object. That Pointer will be updated when a pointer to this resource in the ResourceHandler changes

void Resource::addObserver(Resource const**const observer) {
	observers.push_back(observer);
}

// called by the objects when deleted
void Resource::removeObserver(Resource const*const*const observer) {
	for(std::vector<Resource const**const>::const_iterator i = observers.begin(); i != observers.end(); i++) {
		if(observer == *i) {
			observers.erase(i);
			break;
		}
	}
}

const std::string& Resource::toString() const {
	assert(false);
	return("");
}

void Resource::updateObservers(Resource const*const new_object) const {
	for(std::vector<Resource const**const>::const_iterator i = observers.begin(); i != observers.end(); i++) {
		*(*i) = new_object;
	}
}