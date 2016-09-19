#include "resourcehandler.hpp"
#include <sstream>
#include <iomanip>
#include "text.hpp"

ResourceHandler::ResourceHandler() {
}

ResourceHandler::~ResourceHandler() {
	for(std::map<std::string, const Resource*>::iterator i = resourceList.begin(); i != resourceList.end(); i++) {
		delete i->second;
	}
	resourceList.clear();
}

const Text& ResourceHandler::lookUpText(const std::string& id) const {
	return (const Text&)lookUpResource(id);
}

const std::string& ResourceHandler::lookUpString(const std::string& id) const {
	return (const std::string&)((const Text&)(lookUpResource(id)));
}

/*const Text& ResourceHandler::lookUpStringByID(const unsigned int id) const {
	resourceList.
}

const unsigned int ResourceHandler::lookUpIDbyString(const std::string& id) const {
}*/


const Resource& ResourceHandler::lookUpResource(const std::string& id) const {
	std::map<std::string, const Resource*>::const_iterator i = resourceList.find(id);

	assert(i != resourceList.end());

	return *(i->second);
}

void ResourceHandler::addResource(const std::string& id, const Resource* resource) {
	assert(id.size() > 0);
	resourceList[id] = resource;
}


void ResourceHandler::updateList(const std::string& id1, const std::string& id2) {  // z.B. update("english", "german") => weise alle englischen deutsche Begriffe zu
	((ResourceHandler*)(resourceList[id1]))->updateItems(((ResourceHandler*)resourceList[id2])->resourceList);
}

void ResourceHandler::updateItems(std::map<std::string, const Resource*> list) {
	for(std::map<std::string, const Resource*>::const_iterator i = list.begin(); i != list.end(); i++) {
		resourceList[i->first]->updateObservers(i->second);
	}
}

const bool ResourceHandler::contains(const std::string& entry) const {
	std::map<std::string, const Resource*>::const_iterator i = resourceList.find(entry);
	return(i != resourceList.end());
}

void findandreplace( std::string& source, const std::string& find, const std::string& replace )
{
	size_t j = source.find(find);
	if(j==std::string::npos) {
		return;
	}
	source.replace(j,find.length(),replace);
}

const std::string ResourceHandler::lookUpFormattedString(const std::string& id, const std::string& text) const
{
	std::string txt = lookUpString(id);
	findandreplace(txt, "%s", text);
	return(txt);
}

const std::string ResourceHandler::lookUpFormattedString(const std::string& id, const unsigned int i) const
{
	std::string txt = lookUpString(id);
	std::ostringstream os;
	os.str("");
	os << i; 
	findandreplace(txt, "%i", os.str());
	return(txt);
}
const std::string ResourceHandler::lookUpFormattedString(const std::string& id, const unsigned int i, const unsigned int j, const unsigned int k) const
{
	std::string txt = lookUpString(id);
	std::ostringstream os;
	os.str("");
	os << i;findandreplace(txt, "%i", os.str());os.str("");
	os << j;findandreplace(txt, "%i", os.str());os.str("");
	os << k;findandreplace(txt, "%i", os.str());
	return(txt);
}

const std::string ResourceHandler::lookUpFormattedString(const std::string& id, const unsigned int i, const unsigned int j) const
{
	std::string txt = lookUpString(id);
	std::ostringstream os;
	os.str("");
	if(txt.find("%2i") != std::string::npos)
	{
		os << std::setfill('0') << std::setw(2) << i;
		findandreplace(txt, "%2i", os.str());os.str("");
	}
	else
	{
		os << i;
		findandreplace(txt, "%i", os.str());os.str("");
	}
	if(txt.find("%2i")!=std::string::npos)
	{
		os << std::setfill('0') << std::setw(2) << j;
		findandreplace(txt, "%2i", os.str());os.str("");
	}
	else
	{
		os << j;
		findandreplace(txt, "%i", os.str());os.str("");
	}
	return(txt);
}