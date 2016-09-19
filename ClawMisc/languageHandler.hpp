#ifndef _LANGUAGEHANDLER_HPP
#define _LANGUAGEHANDLER_HPP

#include "resourcehandler.hpp"

class LanguageHandler
{
	public:
		LanguageHandler();
		~LanguageHandler();
		const std::string& getLanguage() const;
		const bool setLanguage(const std::string& language);
		void loadStringFiles(const std::vector<std::string>& string_files);
		void setLanguageChosen(const bool chosen = true);
		const bool isLanguageChosen() const;
		const std::string& lookUpString(const std::string& id) const;
		const Text& lookUpText(const std::string& id) const;
//		const Text& lookUpStringByID(const unsigned int id) const;
//		const unsigned int lookUpIDbyString(const std::string& id) const;

		const std::string lookUpFormattedString(const std::string& id, const std::string& text) const;
		const std::string lookUpFormattedString(const std::string& id, const unsigned int i) const;
		const std::string lookUpFormattedString(const std::string& id, const unsigned int i, const unsigned int j) const;
		const std::string lookUpFormattedString(const std::string& id, const unsigned int i, const unsigned int j, const unsigned int k) const;
		LanguageHandler& operator=(const LanguageHandler& object);
	private:
		std::string language;
		bool languageChosen;

		ResourceHandler stringsHandler;
		ResourceHandler& strings;

};




inline const std::string& LanguageHandler::getLanguage() const {
	return(language);
}

inline const bool LanguageHandler::isLanguageChosen() const {
	return languageChosen;
}

inline void LanguageHandler::setLanguageChosen(const bool chosen) {
	languageChosen = chosen;
}

#endif