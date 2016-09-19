/**
 * Configuration management (defaults, loading/saving, initial parameter check etc.)
 *
 * @author Clemens Lode, clemens at lode.de
 */

#include "configuration.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

Blaue_Configuration::Blaue_Configuration(const std::string configuration_file):
	configurationFile(configuration_file)
{ 
	loadConfigurationFile();
}

Blaue_Configuration::~Blaue_Configuration()
{ }

/**
 * Initialize a working default set of values
 */
void Blaue_Configuration::initDefaults()
{
	maxX = 16;
	maxY = 16;
	hearRangeRed = 2.0;
	sightRangeRed = 3.0;
	sightRangeBlue = 5.0;
	hearRangeBlue = 4.0;
	memberCountRed = 5;
	memberCountBlue = 5;
	speedRed = 2.0;
	speedBlue = 1.0;
	memoryRed = 3;
	simulationSpeed = 50;
	scenarioType = RANDOM_SCENARIO;
	obstaclePercentage = 20;
	obstacleConnectionFactor = 99;
	unsafeDistance = 1.0;
	maxPathAge = 256;
	configurationFile = "settings/blaue.cfg";
}

/**
 * Save current settings to specified file
 */
void Blaue_Configuration::saveToFile() const
{
	std::ofstream pFile(configurationFile.c_str(), std::ios_base::out | std::ios_base::trunc);
	checkStreamIsOpen(pFile, "Blaue_Configuration::saveToFile", configurationFile);

	pFile << "@SETTINGS" << std::endl;
	pFile << "# Map size" << std::endl;
	pFile << "    \"Max X\" = \"" << getMaxX() << "\"" << std::endl;
	pFile << "    \"Max Y\" = \"" << getMaxY() << "\"" << std::endl;
	pFile << "# Ranges are in percent, e.g. 150 = 1.5 fields" << std::endl;
	pFile << "    \"Hear range red\" = \"" << (int)(100.0 * getHearRangeRed()) << "\"" << std::endl;
	pFile << "    \"Sight range red\" = \"" << (int)(100.0 * getSightRangeRed()) << "\"" << std::endl;
	pFile << "    \"Hear range blue\" = \"" << (int)(100.0 * getHearRangeBlue()) << "\"" << std::endl;
	pFile << "    \"Sight range blue\" = \"" << (int)(100.0 * getSightRangeBlue()) << "\"" << std::endl;
	pFile << "    \"Member count red\" = \"" << getMemberCountRed() << "\"" << std::endl;
	pFile << "    \"Member count blue\" = \"" << getMemberCountBlue() << "\"" << std::endl;
	pFile << "# Speed is in percent, e.g. 150 = 1.5 fields" << std::endl;
	pFile << "    \"Speed red\" = \"" << (int)(100.0 * getSpeedRed()) << "\"" << std::endl;
    pFile << "    \"Speed blue\" = \"" << (int)(100.0 * getSpeedBlue()) << "\"" << std::endl;
    pFile << "    \"Memory red\" = \"" << getMemoryRed() << "\"" << std::endl;
	pFile << "# Number of frames between each simulation step" << std::endl;
	pFile << "    \"Simulation speed\" = \"" << getSimulationSpeed() << "\"" << std::endl;
	pFile << "# 0: Random scenario, 1: Non-Torus scenario, 2: Pillar scenario, 3: Cross scenario, 4: Room scenario, 5: Maze scenario" << std::endl;
    pFile << "    \"Scenario type\" = \"" << (int)getScenarioType() << "\"" << std::endl;
	pFile << "# Percentage of obstacles of the total map size" << std::endl;
	pFile << "    \"Obstacle percentage\" = \"" << getObstaclePercentage() << "\"" << std::endl;
	pFile << "# Probability to create connected structures when creating the random map" << std::endl;
	pFile << "    \"Obstacle connection factor\" = \"" << getObstacleConnectionFactor() << "\"" << std::endl;
	pFile << "# At this distance a red agent can kill a blue agent" << std::endl;
	pFile << "    \"Unsafe distance\" = \"" << (int)(100.0 * getUnsafeDistance()) << "\"" << std::endl;
	pFile << "# Number of steps to show the path" << std::endl;
	pFile << "    \"Max path age\" = \"" << getMaxPathAge() << "\"" << std::endl;
	pFile << "@END" << std::endl;
}
	
/**
 * Load data from specified file or create a new file if the file does not exist
 */
void Blaue_Configuration::loadConfigurationFile()
{
	std::ifstream pFile(configurationFile.c_str());
	
	try{
		checkStreamIsOpen(pFile, "Blaue_Configuration::loadConfigurationFile()", configurationFile);
	} catch(...)
	{
		initDefaults();
		saveToFile();
		checkValues();
		return;
	}

	toInitLog("* Loading " + configurationFile);
	
	std::fstream::pos_type old_pos = pFile.tellg();
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		checkStreamForFailure(pFile, "Blaue_Configuration::loadConfigurationFile()", configurationFile);

		std::string text = line;
		size_t start=text.find_first_not_of("\t ");
		if((start==std::string::npos)||(text[0]=='#')||(text[0]=='\0'))
			continue; // ignore line
		size_t stop=text.find_first_of("\t ", start);
		if(stop==std::string::npos) stop = text.size();
		std::string index=text.substr(start, stop);
		std::string value;
		std::map<std::string, std::list<std::string> >::iterator i;
		if(index=="@SETTINGS")
		{
			std::map<std::string, std::list<std::string> > block;
			pFile.seekg(old_pos);
			assert(parse_block_map(pFile, block));
	
			if((i=block.find("Max X"))!=block.end()) {
				i->second.pop_front();
			   	setMaxX(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Max Y"))!=block.end()) {
				i->second.pop_front();
			   	setMaxY(atoi(i->second.front().c_str()));
			}

			if((i=block.find("Hear range red"))!=block.end()){
				i->second.pop_front();
			   	setHearRangeRed(0.01 * (double)atoi(i->second.front().c_str()));
			}	
			if((i=block.find("Sight range red"))!=block.end()){
				i->second.pop_front();
			   	setSightRangeRed(0.01 * (double)atoi(i->second.front().c_str()));
			}		
			if((i=block.find("Hear range blue"))!=block.end()){
				i->second.pop_front();
			   	setHearRangeBlue(0.01 * (double)atoi(i->second.front().c_str()));
			}	
			if((i=block.find("Sight range blue"))!=block.end()){
				i->second.pop_front();
				setSightRangeBlue(0.01 * (double)atoi(i->second.front().c_str()));
			}
			if((i=block.find("Member count red"))!=block.end()){
				i->second.pop_front();
				setMemberCountRed(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Member count blue"))!=block.end()){
				i->second.pop_front();
			   	setMemberCountBlue(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Speed red"))!=block.end()){
				i->second.pop_front();
			   	setSpeedRed(0.01 * (double)atoi(i->second.front().c_str()));
			}
            if((i=block.find("Speed blue"))!=block.end()) {
				i->second.pop_front();
				setSpeedBlue(0.01 * (double)atoi(i->second.front().c_str()));
			}
            if((i=block.find("Memory red"))!=block.end()) {
				i->second.pop_front();
				setMemoryRed(atoi(i->second.front().c_str()));
			}
            if((i=block.find("Simulation speed"))!=block.end()) {
				i->second.pop_front();
				setSimulationSpeed(atoi(i->second.front().c_str()));
			}
            if((i=block.find("Scenario type"))!=block.end()) {
				i->second.pop_front();
				setScenarioType((eScenario)atoi(i->second.front().c_str()));
			}
            if((i=block.find("Obstacle percentage"))!=block.end()) {
				i->second.pop_front();
				setObstaclePercentage(atoi(i->second.front().c_str()));
			}
            if((i=block.find("Obstacle connection factor"))!=block.end()) {
				i->second.pop_front();
				setObstacleConnectionFactor(atoi(i->second.front().c_str()));
			}
            if((i=block.find("Unsafe distance"))!=block.end()) {
				i->second.pop_front();
				setUnsafeDistance(0.01 * (double)atoi(i->second.front().c_str()));
			}
            if((i=block.find("Max path age"))!=block.end()) {
				i->second.pop_front();
				setMaxPathAge(atoi(i->second.front().c_str()));
			}
		}
		old_pos = pFile.tellg();
	}// END while
	checkValues();
}


/**
 * Check restrictions given by the design document
 */
void Blaue_Configuration::checkValues() {
	assert(maxX > 0 && maxY > 0);
	assert(maxX * maxY > memberCountBlue + memberCountRed);
	assert(hearRangeRed > 0);
	assert(hearRangeBlue > 1);
	assert(sightRangeBlue < maxX || sightRangeBlue < maxY);
	// larger than hearRangeRed
	assert(sightRangeRed > hearRangeRed);
	// larger than sightRangeRed
	assert(sightRangeBlue > sightRangeRed);
	// larger than hearRangeRed, smaller than sightRangeBlue
	assert(hearRangeBlue > hearRangeRed && hearRangeBlue < sightRangeBlue);
	// max 50% of speedRed
	assert(2 * speedBlue <= speedRed);
}


// -------- Simple SET functions --------

void Blaue_Configuration::setConfigurationFile(const std::string& configuration_file) {
	configurationFile = configuration_file;
}

void Blaue_Configuration::setMaxX(const unsigned int max_x) {
	assert(max_x > 0);

	maxX = max_x;
	halfMaxX = max_x / 2;
}

void Blaue_Configuration::setMaxY(const unsigned int max_y) {
	assert(max_y > 0);

	maxY = max_y;
	halfMaxY = max_y / 2;
}

void Blaue_Configuration::setHearRangeRed(const double hear_range_red) {
	hearRangeRed = hear_range_red;
}

void Blaue_Configuration::setSightRangeRed(const double sight_range_red) {
	sightRangeRed = sight_range_red;
}

void Blaue_Configuration::setSightRangeBlue(const double sight_range_blue) {
	sightRangeBlue = sight_range_blue;
}

void Blaue_Configuration::setHearRangeBlue(const double hear_range_blue) {
	hearRangeBlue = hear_range_blue;
}

void Blaue_Configuration::setMemberCountRed(const unsigned int member_count_red) {
	memberCountRed = member_count_red;
}

void Blaue_Configuration::setMemberCountBlue(const unsigned int member_count_blue) {
	memberCountBlue = member_count_blue;
}

void Blaue_Configuration::setSpeedRed(const double speed_red) {
	speedRed = speed_red;
}

void Blaue_Configuration::setSpeedBlue(const double speed_blue) {
	speedBlue = speed_blue;
}

void Blaue_Configuration::setMemoryRed(const unsigned int memory_red) {
	memoryRed = memory_red;
}

void Blaue_Configuration::setSimulationSpeed(const unsigned int simulation_speed) {
	simulationSpeed = simulation_speed;
}

void Blaue_Configuration::setScenarioType(const eScenario scenario_type) {
	scenarioType = scenario_type;
}

void Blaue_Configuration::setObstaclePercentage(const unsigned int obstacle_percentage) {
	obstaclePercentage = obstacle_percentage;
}

void Blaue_Configuration::setObstacleConnectionFactor(const unsigned int obstacle_connection_factor) {
	obstacleConnectionFactor = obstacle_connection_factor;
}

void Blaue_Configuration::setUnsafeDistance(const double unsafe_distance) {
	unsafeDistance = unsafe_distance;
}

void Blaue_Configuration::setMaxPathAge(const unsigned int max_path_age) {
	maxPathAge = max_path_age;
}

// global configuration variable
Blaue_Configuration Configuration;