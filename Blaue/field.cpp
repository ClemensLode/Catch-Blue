/**
 * Single field on the grid.
 *
 * @author Clemens Lode, clemens at lode.de
 */

#include <assert.h>

#include "configuration.hpp"
#include "field.hpp"

Field::Field():
	content(EMPTY),
	seen_by(),
	heard_by(),
	agent(NULL)
{}

Field::~Field()
{}

/**
 * Save a pointer of an agent that sits on this field
 * @param a The agent in question
 */
void Field::setAgent(BaseAgent* a) {
	agent = a;
	if(a == NULL) {
		setContent(EMPTY);
	} else {
		setContent((eFieldContent)(FIRST_AGENT_ID + a->getID()));
	}
}




/**
 * Completely clears all information from this field
 */
void Field::clear() {
    content = EMPTY;
	agent = NULL;
    seen_by.clear();
    heard_by.clear();
}

/**
 * Clear only sight and hearing ranges
 */
void Field::clearSight() {
    seen_by.clear();
    heard_by.clear();
}





/**
 * Remember that an agent passed by here
 * @param p The position the agent went to
 * @param color The color of the agent
 */
void Field::addPath(const GridPoint& p, const eAgentColor color) {
	AgePath ap;
	ap.p = p;
	ap.age = Configuration.getMaxPathAge();
	ap.color = color;

	agePathList.push_back(ap);
}

/**
 * Increase age of all path vectors, remove them if they get too old
 */
void Field::doAgePath() {
	for(std::list<AgePath>::iterator i = agePathList.begin(); i != agePathList.end();) {
		i->age--;
		if(i->age == 0) {
			i = agePathList.erase(i);
		} else {
			i++;
		}
	}
}

/**
 * Checks whether this field is in sight range of the agent
 * @param id ID of the agent in question
 * @param observer Directly return a pointer to the agent in question
 * @return true if the agent can see what is going on in this field
 */
bool Field::isSeenBy(const unsigned int id, const BaseAgent*& observer) const {
	for(std::vector<const BaseAgent*>::const_iterator i = seen_by.begin(); i != seen_by.end(); i++) {
		if(!(*i)->isAlive()) {
			continue;
		}
		if((*i)->getID() == id) {
			observer = (*i);
			return true;
		}
	}
	return false;
}

/**
 * Checks whether this field is in sight range of the agent
 * @param id ID of the agent in question
 * @return true if the agent can see what is going on in this field
 */
bool Field::isSeenBy(const unsigned int id) const {
	for(std::vector<const BaseAgent*>::const_iterator i = seen_by.begin(); i != seen_by.end(); i++) {
		if(!(*i)->isAlive()) {
			continue;
		}
		if((*i)->getID() == id) {
			return true;
		}
	}
	return false;
}

/**
 * Checks whether this field is in hearing range of the agent
 * @param id ID of the agent in question
 * @return true if the agent can hear what is going on in this field
 */
bool Field::isHeardBy(const unsigned int id) const {
	for(std::vector<const BaseAgent*>::const_iterator i = heard_by.begin(); i != heard_by.end(); i++) {
		if(!(*i)->isAlive()) {
			continue;
		}
		if((*i)->getID() == id) {
			return true;
		}
	}
	return false;
}

/**
 * Mark this field as a field someone can see other agents and obstacles
 * @param agent This agent can see what is going on in this field
 */
void Field::addSeen(const BaseAgent* agent) {
// does it already contain an entry?
	for(std::vector<const BaseAgent*>::const_iterator i = seen_by.begin(); i != seen_by.end(); i++) {
		if(!(*i)->isAlive()) {
			continue;
		}
		if((*i)->getID() == agent->getID()) {
			return;
		}
	}
    seen_by.push_back(agent);
}

/**
 * Mark this field as a field someone can hear movements
 * @param agent This agent can hear what is going on in this field
 */
void Field::addHear(const BaseAgent* agent) {
// does it already contain an entry?
	for(std::vector<const BaseAgent*>::const_iterator i = heard_by.begin(); i != heard_by.end(); i++) {
		if(!(*i)->isAlive()) {
			continue;
		}
		if((*i)->getID() == agent->getID()) {
			return;
		}
	}
    heard_by.push_back(agent);
}