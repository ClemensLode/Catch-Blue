#ifndef _UI_MULTILINETEXT_HPP
#define _UI_MULTILINETEXT_HPP

#include <SDL_Object.hpp>
#include "windowcolors.hpp"
#include <vector>

class UI_MultiLineText : public SDL_Object
{
public:
	UI_MultiLineText(SDL_Object* mlt_parent, 
				const Rect& mlt_pos,
				const std::string& ml_text, 
				WindowColors* window_colors);
	UI_MultiLineText(SDL_Object* mlt_parent, 
				const Rect& mlt_pos,
				Text* ml_text, 
				WindowColors* window_colors);
	~UI_MultiLineText();

	void draw(DC* dc) const;
	void process();
	void reloadOriginalSize();
private:
	Text* eText;
	std::string text;
	WindowColors* windowColors;
	std::vector<std::pair<int, int> > linebreaks;
};

#endif