#include "languagehandler.hpp"
#include "misc.hpp"

LanguageHandler::LanguageHandler():
	strings(stringsHandler),
	language(""),
	languageChosen(false)
	{}

LanguageHandler::~LanguageHandler()
	{}

const bool LanguageHandler::setLanguage(const std::string& language) {
	if(this->language == language) {
		return(false);
	}

	assert(stringsHandler.contains(language));

	languageChosen = true;

	strings = (ResourceHandler&)(stringsHandler.lookUpResource(language));
	//.updateList(this->language, language);

	this->language = language;
	return true;
}

void LanguageHandler::loadStringFiles(const std::vector<std::string>& string_files) {
	assert(!string_files.empty());
	for(std::vector<std::string>::const_iterator j = string_files.begin(); j != string_files.end(); j++) {
		loadStringFile(*j, "lng", "STRINGS", stringsHandler);	
	}
}

LanguageHandler& LanguageHandler::operator=(const LanguageHandler& object) {
	this->languageChosen = object.languageChosen;
	this->language = object.language;
	this->stringsHandler = object.stringsHandler;
	this->strings = object.strings;
	return *this;
}

const Text& LanguageHandler::lookUpText(const std::string& id) const {
	assert(languageChosen);
	return strings.lookUpText(id);
}

const std::string& LanguageHandler::lookUpString(const std::string& id) const {
	assert(languageChosen);
	return strings.lookUpString(id);
}

/*const Text& LanguageHandler::lookUpStringByID(const unsigned int id) const {
	assert(languageChosen);
	return strings.lookUpStringByID(id);
}

const unsigned int LanguageHandler::lookUpIDbyString(const std::string& id) const {
	assert(languageChosen);
	return strings.lookUpIDbyString(id);
}*/

const std::string LanguageHandler::lookUpFormattedString(const std::string& id, const std::string& text) const {
	assert(languageChosen);
	return strings.lookUpFormattedString(id, text);
}

const std::string LanguageHandler::lookUpFormattedString(const std::string& id, const unsigned int i) const {
	assert(languageChosen);
	return strings.lookUpFormattedString(id, i);
}

const std::string LanguageHandler::lookUpFormattedString(const std::string& id, const unsigned int i, const unsigned int j) const {
	assert(languageChosen);
	return strings.lookUpFormattedString(id, i, j);
}
// TODO evtl std::list einführen
const std::string LanguageHandler::lookUpFormattedString(const std::string& id, const unsigned int i, const unsigned int j, const unsigned int k) const {
	assert(languageChosen);
	return strings.lookUpFormattedString(id, i, j, k);
}
