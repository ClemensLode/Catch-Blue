#ifndef _BLAUE_FIELD_HPP
#define _BLAUE_FIELD_HPP

/**
 * Single field on the grid.
 *
 * @author Clemens Lode, clemens at lode.de
 */

#include <vector>
#include <string>

#include "baseagent.hpp"

/**
 * IDs of the cells of the grid
 * IDs above OBSTACLE denote the agent id
 */
enum eFieldContent
{
	EMPTY,
	OBSTACLE,
	FIRST_AGENT_ID
};

struct AgePath {
	GridPoint p;
	unsigned int age;
	eAgentColor color;
};


class Field {
	public:
		Field();
		~Field();

		
		/**
		 * Save a pointer of an agent that sits on this field
		 * @param a The agent in question
		 */
		void setAgent(BaseAgent* a);

		/**
		 * Completely clears all information from this field
		 */
		void clear();

		/**
		 * Clear only sight and hearing ranges
		 */
		void clearSight();

		/**
		 * Checks whether this field is in sight range of the agent
		 * @param id ID of the agent in question
		 * @return true if the agent can see what is going on in this field
		 */
		bool isSeenBy(const unsigned int id) const;

		/**
		 * Checks whether this field is in sight range of the agent
		 * @param id ID of the agent in question
		 * @param observer Directly return a pointer to the agent in question
		 * @return true if the agent can see what is going on in this field
		 */
		bool isSeenBy(const unsigned int id, const BaseAgent*& observer) const;		

		/**
		 * Checks whether this field is in hearing range of the agent
		 * @param id ID of the agent in question
		 * @return true if the agent can hear what is going on in this field
		 */
		bool isHeardBy(const unsigned int id) const;

		/**
		 * Mark this field as a field someone can see other agents and obstacles
		 * @param agent This agent can see what is going on in this field
		 */
		void addSeen(const BaseAgent* agent);

		/**
		 * Mark this field as a field someone can hear movements
		 * @param agent This agent can hear what is going on in this field
		 */
		void addHear(const BaseAgent* agent);



		/**
		 * Remember that an agent passed by here
		 * @param p The position the agent went to
		 * @param color The color of the agent
		 */
		void addPath(const GridPoint& p, const eAgentColor color);

		/**
		 * Increase age of all path vectors, remove them if they get too old
		*/
		void doAgePath();

// -------- Simple GET/SET functions --------
		bool isSeen() const;
		bool isHeard() const;
		bool isEmpty() const;
		bool isOccupied() const;
		BaseAgent* getAgent() const;
		eFieldContent getContent() const;
		void setContent(const eFieldContent content);
		const std::vector<const BaseAgent*>& getSeenBy() const;
		const std::vector<const BaseAgent*>& getHeardBy() const;
		const std::list<AgePath>& getAgePathList() const;

	private:
		BaseAgent* agent;
		eFieldContent content;
		std::vector<const BaseAgent*> seen_by;
		std::vector<const BaseAgent*> heard_by;
		std::list<AgePath> agePathList;
};


// -------- Simple GET/SET functions --------

inline const std::list<AgePath>& Field::getAgePathList() const {
	return agePathList;
}

inline eFieldContent Field::getContent() const {
    return content;
}

inline void Field::setContent(const eFieldContent content) {
    this->content = content;
}

inline BaseAgent* Field::getAgent() const {
	if(!agent || !agent->isAlive()) {
		return NULL;
	}
	return agent;
}

inline const std::vector<const BaseAgent*>& Field::getSeenBy() const {
	return seen_by;
}

inline const std::vector<const BaseAgent*>& Field::getHeardBy() const {
	return heard_by;
}

inline bool Field::isEmpty() const {
    return content == EMPTY;
}

inline bool Field::isOccupied() const {
    return content != EMPTY;
}

/**
 * @return true if there is at least one agent in sight range
 */
inline bool Field::isSeen() const {
    return !seen_by.empty();
}

inline bool Field::isHeard() const {
	return !heard_by.empty();
}

#endif