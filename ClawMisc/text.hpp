#ifndef _MISC_TEXT_HPP
#define _MISC_TEXT_HPP

#include "resource.hpp"

// evtl Templates stattdessen

class Text : public Resource 
{
	public:
		Text(const std::string& text);
		~Text();
		operator const std::string&() const;
		std::string operator->() const;
		std::string toString() const;
	private:
		std::string text;
};

inline Text::Text(const std::string& text) {
	this->text = text;
}

inline Text::operator const std::string&() const { 
	return text;
}

inline std::string Text::toString() const {
	return text;
}
		
inline std::string Text::operator->() const {
	return text;
}

#endif