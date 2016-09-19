#ifndef _UI_OBJECT_HPP
#define _UI_OBJECT_HPP


#include <sdl_object.hpp>
#include <text.hpp>

class UI_Object;

class UI_Object : public SDL_Object
{
	public:


		UI_Object(SDL_Object* parent_object, 
				const Rect& relative_rect = Rect(0, 0, 0, 0), 
				const Size& distance_bottom_right = Size(0, 0), 
				const ePositionMode position_mode = DO_NOT_ADJUST, 
				const eAutoSize auto_size = NOTHING);

		UI_Object(SDL_Object* parent_object, 
				UI_Object* position_parent_object, 
				const Rect& relative_rect = Rect(0, 0, 0, 0), 
				const Size& distance_bottom_right = Size(0, 0), 
				const ePositionMode position_mode = DO_NOT_ADJUST, 
				const eAutoSize auto_size = NOTHING);

		virtual ~UI_Object(); 
		
		void draw(DC* dc) const;
		
// returns the object the mouse cursor is currently over
		virtual UI_Object* checkToolTip(const Point& mouse) const;

		void process();
		void resetData();
		
		void updateToolTip(const Text* tool_tip_string);
		void updateToolTip(const std::string& tool_tip_string);		
		const Text* getToolTipEString() const;
		const std::string& getToolTipString() const;
		const bool hasToolTip() const;
				
		virtual const bool addKey(unsigned int key, unsigned int mod);

		const bool isClipped() const;
		const Rect& getClipRect() const;
		void setClipRect(const Rect& rect);
		void setClipped(const bool clipped = true);
	private:
		const Text* toolTipEString;
		std::string toolTipString;

		bool clipped;
		Rect clipRect;
};

inline void UI_Object::setClipped(const bool clipped) {
	this->clipped = clipped;
}

inline const bool UI_Object::isClipped() const {
	return clipped;
}

inline const Rect& UI_Object::getClipRect() const {
	return clipRect;
}

inline void UI_Object::setClipRect(const Rect& rect) {
	this->clipRect = rect;
}


inline void UI_Object::updateToolTip(const Text* tool_tip_string) {
	toolTipEString = tool_tip_string;
}

inline void UI_Object::updateToolTip(const std::string& tool_tip_string) {
	toolTipString = tool_tip_string;
}

inline const bool UI_Object::hasToolTip() const {
	return( (!toolTipString.empty()) || (toolTipEString != NULL) );
}

inline const Text* UI_Object::getToolTipEString() const {
	return(toolTipEString);
}

inline const std::string& UI_Object::getToolTipString() const {
	return(toolTipString);
}

#endif

