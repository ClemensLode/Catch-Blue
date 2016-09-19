#include "ui_statictext.hpp"
#include <sstream>

// TODO KASTEN UM STATICTEXT
	//	dc->setBrush(*gui.lookUpBrush(WINDOW_FOREGROUND_BRUSH));
//	dc->setPen(*gui.lookUpPen(INNER_BORDER_HIGHLIGHT_PEN)); Kasten hinter Title... TODO StaticText
//	Size s = dc->getTextExtent(text);
//	Rect titleRect = Rect(Point(), s + Size(5, 0));
	
//	dc->DrawRectangle(titleRect);
//	titleRect.setTopLeft(titleRect.getTopLeft() + Point(2, 3));
//	dc->DrawText(text, titleRect.getTopLeft());


UI_StaticText::UI_StaticText(SDL_Object* st_parent,
							 const Rect& st_pos, 
							 const Size& distance_bottom_right, 
							 const Color* st_color, 
							 const Font* st_font, 
							 const ePositionMode position_mode) :
	UI_Object(st_parent, st_pos, distance_bottom_right, position_mode, AUTO_SIZE),
	text(),
	textWasChanged(true),
	font(st_font),
	color(st_color),
	tempColor(),
	tempColorIsSet(false),
	eText(NULL),
	highlight(false)
{ 
	setDrawType(TRANSPARENT_OBJECT);
//	setDrawType(ANTI_ALIASED_OBJECT);
}


UI_StaticText::UI_StaticText(SDL_Object* st_parent, 
							 const Rect& st_pos, 
							 const Size& distance_bottom_right, 
							 const Text* st_text, 
							 const Color* st_color, 
							 const Font* st_font, 
							 const ePositionMode position_mode) :
	UI_Object(st_parent, st_pos, distance_bottom_right, position_mode, NO_AUTO_SIZE),
	text(),
	textWasChanged(true),
	font(st_font),
	color(st_color),
	tempColor(),
	tempColorIsSet(false),
	eText(st_text),
	highlight(false)
{
	setDrawType(TRANSPARENT_OBJECT);
//	setDrawType(ANTI_ALIASED_OBJECT);
	updateText(*st_text); //?
}

UI_StaticText::UI_StaticText(SDL_Object* st_parent, 
							 const Rect& st_pos, 
							 const Size& distance_bottom_right, 
							 const std::string& st_text, 
							 const Color* st_color, 
							 const Font* st_font, 
							 const ePositionMode position_mode) :
	UI_Object(st_parent, st_pos, distance_bottom_right, position_mode, NO_AUTO_SIZE),
	text(st_text),
	textWasChanged(true),
	font(st_font),
	color(st_color),
	tempColor(),
	tempColorIsSet(false),
	eText(NULL),
	highlight(false)
{
	setDrawType(TRANSPARENT_OBJECT);
//	setDrawType(ANTI_ALIASED_OBJECT);
}

UI_StaticText::~UI_StaticText()
{}

void UI_StaticText::doHighlight(const bool high_light) 
{
	if(high_light == highlight)
		return;
	highlight = high_light;
}

UI_Object* UI_StaticText::checkToolTip(const Point& mouse) const {
	if(isShown() && Rect(getAbsolutePosition(), getTextSize()).isTopLeftCornerInside(mouse)) {
		return((UI_Object*)this);
	}
	return(NULL);
}

void UI_StaticText::draw(DC* dc) const
{
	assert(dc);
	assert(font);

	dc->setFont(*font);
	Color normal;
	Color highlighted;
	if(color) 
	{
		if(tempColorIsSet)
		{
			highlighted = tempColor.changeAbsoluteBrightness(dc->getSurface(), 60);
			normal = tempColor;
		} else
		{
			highlighted = color->changeAbsoluteBrightness(dc->getSurface(), 60);
			normal = *color;
		}
		if(highlight) {
			dc->setTextForeground(highlighted);
		} else {
			dc->setTextForeground(normal);
		}

		Point p = Point(0,3);
		/*bool done = false;
		for(unsigned int i = 0; i < text.size(); i++)
			// Schatten oder so? TODO
			if(text[i] == '&')
			{
				if(i > 0) {
					dc->DrawText(text.substr(0, i), p);
				}
				dc->setTextForeground(*gui.lookUpColor(FORCE_TEXT_COLOR));
				if(i + 1 < text.size()) {
					dc->DrawText(text.substr(i+1, 1), p + Size(dc->getTextExtent(text.substr(0,i)).getWidth(),0));
				}

//				if(highlight)
//					dc->setTextForeground(highlighted);
//				else
					dc->setTextForeground(normal);
				if((i+2) < text.size()) {
					dc->DrawText(text.substr(i+2, text.size() - i - 2), p + Size(dc->getTextExtent(text.substr(i+1, 1)).getWidth() + dc->getTextExtent(text.substr(0,i)).getWidth(), 0));	
				}
				done = true;
				break;
			}
		if(!done) {*/
			dc->DrawText(text, p);
		//}
	
	}
	
	UI_Object::draw(dc);
}

void UI_StaticText::addChar(const unsigned int pos, const char key)
{
	std::ostringstream os;
	os.str("");
	if(pos>=text.size()) {
		os << text << key;
	} else if(pos==0) {
		os << key << text;
	} else {
		os << text.substr(0, pos) << key << text.substr(pos);
	}
	updateText(os.str());
}

void UI_StaticText::removeCharBackspace(const unsigned int pos)
{
	if((pos == 0) || (text.size() == 0)) {
		return;
	}
	std::ostringstream os;
	os.str("");
	if(pos>=text.size()) {
		os << text.substr(0, text.size()-1);
	} else {
		os << text.substr(0, pos-1) << text.substr(pos);
	}
	updateText(os.str());
}

void UI_StaticText::removeCharDelete(const unsigned int pos)
{
	if((text.size()==0)||(pos>=text.size())) {
		return;
	}
	std::ostringstream os;
	os.str("");
	if(pos==0) {
		os << text.substr(1);
	} else {
		if(pos==text.size()-1) {
			os << text.substr(0, text.size()-1);
		} else {
			os << text.substr(0, pos) << text.substr(pos+1);
		}
	}
	updateText(os.str());
}

void UI_StaticText::process()
{
	UI_Object::process();
//	if(!isShown())
//		return;
	tempColorIsSet = false;
//	if( (isShown()) && (Rect(getAbsolutePosition(), getTextSize()).Inside(mouse )) )
//		setNeedRedrawMoved(); ??
	if(textWasChanged)
	{
		makePufferInvalid();
		setSize(getTextSize() + Size(1+font->getSize()/6, font->getSize()/6)); // if it has shadow...
		textWasChanged = false;
	}
}


void UI_StaticText::reloadOriginalSize()
{
	UI_Object::reloadOriginalSize();
}

void UI_StaticText::updateText(const std::string& st_text, const bool etext_change)
{
	if((st_text==text) && (!etext_change)) {
		return; //?
	}
	text = st_text;
	eText = NULL;
	setTextWasChanged();
}

void UI_StaticText::updateText(const Text* st_text)
{
	if(eText == st_text) {// o_O 
		return;
	}
	updateText(*st_text);
	eText = st_text; //?
}

void UI_StaticText::reloadText(const std::string& st_text)
{
	text = st_text;
	setTextWasChanged();
}

void UI_StaticText::reloadText(const Text* st_text)
{
	eText = st_text;
	reloadText(*st_text);
}


