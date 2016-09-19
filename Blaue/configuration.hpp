#ifndef _BLAUE_CONFIGURATION_HPP
#define _BLAUE_CONFIGURATION_HPP

/**
 * Configuration management (defaults, loading/saving, initial parameter check etc.)
 *
 * @author Clemens Lode, clemens at lode.de
 */

#include <assert.h>
#include <misc.hpp>
#include <string>

enum eScenario
{
    RANDOM_SCENARIO,
    NON_TORUS_SCENARIO,
    PILLAR_SCENARIO,
    CROSS_SCENARIO,
    ROOM_SCENARIO,
    DIFFICULT_SCENARIO,

	MAX_SCENARIOS
};

class Blaue_Configuration
{
	public:
		Blaue_Configuration(const std::string configuration_file = "settings/blaue.cfg");
		~Blaue_Configuration();

		void setConfigurationFile(const std::string& configuration_file);

		/**
		 * Save current settings to specified file
		 */
		void saveToFile() const;

// -------- Simple GET functions --------
		unsigned int getMaxX() const;
		unsigned int getMaxY() const;
		double getHearRangeRed() const;
		double getSightRangeRed() const;
		double getHearRangeBlue() const;
		double getSightRangeBlue() const;
		unsigned int getMemberCountRed() const;
		unsigned int getMemberCountBlue() const;
		double getSpeedRed() const;
		double getSpeedBlue() const;
		unsigned int getMemoryRed() const;
		unsigned int getSimulationSpeed() const;
		eScenario getScenarioType() const;
		unsigned int getObstaclePercentage() const;
		unsigned int getObstacleConnectionFactor() const;
		double getUnsafeDistance() const;
		unsigned int getMaxPathAge() const;
				
		unsigned int getHalfMaxX() const;
		unsigned int getHalfMaxY() const;
	
	private:
		unsigned int maxX;
		unsigned int maxY;
		double hearRangeRed;
	// larger than hearRangeRed
		double sightRangeRed;
	// larger than sightRangeRed
		double sightRangeBlue;
	// larger than hearRangeRed, smaller than sightRangeBlue
		double hearRangeBlue;
		unsigned int memberCountRed;
		unsigned int memberCountBlue;
		double speedRed;
	// max 50% of speedRed
		double speedBlue;
		unsigned int memoryRed;
		
	// number of frames between each agent movement
		unsigned int simulationSpeed;

		eScenario scenarioType;
	// Percentage of obstacles of the total map size
		unsigned int obstaclePercentage;
	// Probability to create connected structures when creating the random map
		unsigned int obstacleConnectionFactor;

	// At this distance a red agent can kill a blue agent
		double unsafeDistance;

		// number of steps to show the path
		unsigned int maxPathAge;

		std::string configurationFile;	

		// temporary variables, simply maxX /2 and maxY /2 
		unsigned int halfMaxX;
		unsigned int halfMaxY;

		/**
		 * Initialize a working default set of values
		 */
		void initDefaults();

		/**
		 * Load data from specified file or create a new file if the file does not exist
		 */
		void loadConfigurationFile();

		/**
		 * Check the ranges of the values we just read from the configuration file
		 */
		void checkValues();


// -------- Simple SET functions --------
		void setMaxX(const unsigned int max_x);
		void setMaxY(const unsigned int max_y);
		void setHearRangeRed(const double hear_range_red);
		void setSightRangeRed(const double sight_range_red);
		void setSightRangeBlue(const double sight_range_blue);
		void setHearRangeBlue(const double hear_range_blue);
		void setMemberCountRed(const unsigned int member_count_red);
		void setMemberCountBlue(const unsigned int member_count_blue);
		void setSpeedRed(const double speed_red);
		void setSpeedBlue(const double speed_blue);
		void setMemoryRed(const unsigned int memory_red);
		void setSimulationSpeed(const unsigned int simulation_speed);
		void setScenarioType(const eScenario scenario_type);
		void setObstaclePercentage(const unsigned int obstacle_percentage);
		void setObstacleConnectionFactor(const unsigned int obstacle_connection_factor);
		void setUnsafeDistance(const double unsafe_distance);
		void setMaxPathAge(const unsigned int max_path_age);
};

// global configuration variable
extern Blaue_Configuration Configuration;


// -------- Simple GET functions --------

inline unsigned int Blaue_Configuration::getMaxX() const {
	assert(maxX > 0);
	return maxX;
}

inline unsigned int Blaue_Configuration::getMaxY() const {
	assert(maxY > 0);
	return maxY;
}

inline unsigned int Blaue_Configuration::getHalfMaxX() const {
	assert(halfMaxX > 0);
	return halfMaxX;
}

inline unsigned int Blaue_Configuration::getHalfMaxY() const {
	assert(halfMaxY > 0);
	return halfMaxY;
}

inline double Blaue_Configuration::getHearRangeRed() const {
	return hearRangeRed;
}

inline double Blaue_Configuration::getSightRangeRed() const {
	return sightRangeRed;
}

inline double Blaue_Configuration::getSightRangeBlue() const {
	return sightRangeBlue;
}

inline double Blaue_Configuration::getHearRangeBlue() const {
	return hearRangeBlue;
}

inline unsigned int Blaue_Configuration::getMemberCountRed() const {
	return memberCountRed;
}

inline unsigned int Blaue_Configuration::getMemberCountBlue() const {
	return memberCountBlue;
}

inline double Blaue_Configuration::getSpeedRed() const {
	return speedRed;
}

inline double Blaue_Configuration::getSpeedBlue() const {
	return speedBlue;
}

inline unsigned int Blaue_Configuration::getMemoryRed() const {
	return memoryRed;
}

inline unsigned int Blaue_Configuration::getSimulationSpeed() const {
	assert(simulationSpeed > 0);
	return simulationSpeed;
}


inline eScenario Blaue_Configuration::getScenarioType() const {
	return scenarioType;
}

inline unsigned int Blaue_Configuration::getObstaclePercentage() const {
	return obstaclePercentage;
}

inline unsigned int Blaue_Configuration::getObstacleConnectionFactor() const {
	return obstacleConnectionFactor;
}

inline double Blaue_Configuration::getUnsafeDistance() const {
	return unsafeDistance;
}

inline unsigned int Blaue_Configuration::getMaxPathAge() const {
	return maxPathAge;
}


#endif // _BLAUE_CONFIGURATION_HPP



