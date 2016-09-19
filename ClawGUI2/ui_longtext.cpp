#include "ui_longtext.hpp"

#include <sstream>

UI_LongText::UI_LongText(SDL_Object* lt_parent, 
		const Rect lt_pos, 
		const Size distance_bottom_right, 
		const std::string& lt_text, 
		Color* lt_color1, 
		Color* lt_color2, 
		FontType* lt_font, 
		FontType* big_lt_font,
		ButtonColors* lt_button, 
		ButtonColors* lt_visited_button, 
		UI_Object* scroll_bar, 
		const bool use_help_chapters, 
		const ePositionMode position_mode, 
		const eAutoSize auto_size) :
	UI_Object(lt_parent, lt_pos, distance_bottom_right, position_mode, auto_size),
	scrollBar(scroll_bar),
	longText(),
	longButton(),
	longNumber(),
	longBitmap(),
	text(lt_text),
	color1(lt_color1),
	color2(lt_color2),
	fontType(lt_font),
	bigFontType(big_lt_font),
	button(lt_button),
	visitedButton(lt_visited_button),
	textWasChanged(true),
	useHelpChapters(use_help_chapters)
{
	setDrawType(TRANSPARENT_OBJECT);
}

UI_LongText::~UI_LongText()
{
	std::list<UI_StaticText*>::iterator i = longText.begin();
	while(!longText.empty())
	{
		delete(*i);
		i = longText.erase(i);
	}
	std::list<UI_Button*>::iterator j = longButton.begin();
	while(!longButton.empty())
	{
		delete(*j);
		j = longButton.erase(j);
	}
	std::list<UI_Bitmap*>::iterator k = longBitmap.begin();
	while(!longBitmap.empty())
	{
		delete(*k);
		k = longBitmap.erase(k);
	}
}



void UI_LongText::draw(DC* dc) const
{
	assert(dc);
	UI_Object::draw(dc);
}

void UI_LongText::process()
{
//	if(!isShown())
//		return;
	UI_Object::process();
	if(textWasChanged)
	{
		updateText(text);
		textWasChanged = false;
	}

// TODO irgendwie alle invalid machen...  mmmh... oder Groesse von UI_LongText mal anschaun
#if 0
	std::list<UI_StaticText*>::iterator i = longText.begin();
	while(i!=longText.end())
	{
		if((*i)->isPufferInvalid())
		{
			makePufferInvalid();
			setNeedRedrawMoved(); TODO
			return;
//		}
		++i;
	}
	std::list<UI_Button*>::iterator j = longButton.begin();
	while(j!=longButton.end())
	{
		if((*j)->checkForNeedRedraw())
		{
//			setNeedRedrawMoved(); TODO
			return;
		}
		++j;
	}
	std::list<UI_Bitmap*>::iterator k = longBitmap.begin();
	while(k!=longBitmap.end())
	{
		if((*k)->checkForNeedRedraw())
		{
//			setNeedRedrawMoved(); TODO
			return;
		}
		++k;
	}
#endif
}

void UI_LongText::reloadOriginalSize()
{
	if(!isShown()) {
		return;
	}
	UI_Object::reloadOriginalSize();
}

void UI_LongText::updateText(const std::string& lt_text)
{
//	if(text == lt_text)
//		return; OMFG
//	setNeedRedrawMoved(); TODO
	text = lt_text;
//	if(getParent()!=NULL)
//	{
//		getParent()->adjustPositionAndSize(ADJUST_ONLY_POSITION);
//	}

	unsigned int text_cursor_x = 5;
	unsigned int first_text_cursor_x = 5;
	unsigned int bold = 0;
	unsigned int big = 0;
	unsigned int textbutton = 0;
	unsigned int bitmap = 0;
	bool must_not_make_new_line = false;
	bool align_next_bitmap_right = false;
	unsigned int first_char_position=0;
	unsigned int last_char_position=0;
	signed int current_row=1;
	signed int maxdy=0;
	unsigned int bitmap_left_border = 5;
	unsigned int bitmap_lower_border = 0; // TODO, funzt nur mit Bildern links oben...

// clear items so far
	{
		std::list<UI_StaticText*>::iterator i = longText.begin();
		while(!longText.empty())
		{
			delete(*i);
			i = longText.erase(i);
		}
		

		std::list<UI_Button*>::iterator j = longButton.begin();
		while(!longButton.empty())
		{
			delete(*j);
			j = longButton.erase(j);
		}
		
		std::list<signed int>::iterator k = longNumber.begin();
		while(!longNumber.empty()) {
			k = longNumber.erase(k);
		}
		UI_Button::resetButton();
	
		std::list<UI_Bitmap*>::iterator l = longBitmap.begin();
		while(!longBitmap.empty())
		{
			delete(*l);
			l = longBitmap.erase(l);
		}
	}
	
	Bitmap* current_bitmap = NULL;



	Point p;
	if(scrollBar != NULL) {
		p = getRelativePosition();
	}
			
	// TODO maxdy nur ungefährer Richtwert, kein bigFont benutzt!
	Color* current_color = color1;
	const Font* current_font = fontType->getNormalFont();
	maxdy = current_font->getTextExtent(text).getHeight();

	for(unsigned int i = 0; i < text.length(); i++)
	{
		bool new_line = false;
		bool new_check = false;
		bool end_of_order=false;
		bool move_below_bitmap = false;
//		if(text[i]=='&') {
//			must_not_make_new_line = !must_not_make_new_line;
//		} else 
// set new 'milestone' when reaching end of a word:
		if((text[i]==' ')||(text[i]=='/')||(text[i]==','))//&&(!must_not_make_new_line)) 
		{
			last_char_position = i;
			text_cursor_x += current_font->getTextExtent(" ").getWidth();
		}
// ... or a line:
		else if(text[i]=='#') {
			last_char_position = i;
			new_line = true;
			end_of_order = true;
		} else if(text[i]=='>') {
			align_next_bitmap_right = true;
		}		
		else if(text[i]=='^') {
			last_char_position = i;
			new_line = true;
			end_of_order = true;
			move_below_bitmap = true;
		}
		else if(i==text.length()-1) {
			last_char_position = i+1;
			new_line=true;
		}	
// bold:
		else if(text[i]=='$')
		{
			bold++;
			last_char_position = i;
			new_check = true;
			end_of_order = true;
// big:
		} else if(text[i]=='`')	{
			big++;
			last_char_position = i;
			new_check = true;
			end_of_order = true;
// button:			
		} else if(text[i]=='@')
		{
			if(textbutton > 0)
			{
				new_check = true;
				end_of_order = true;
			}
			textbutton++;
			last_char_position = i;
// bitmap			
		} else if(text[i]=='~')
		{
			bitmap++;
			last_char_position = i;
			new_check = true;
			end_of_order = true;
		} else
		// move the textcursor forward:
		{
			std::ostringstream os;
			os.str("");
			os << text[i];
			if(textbutton == 0) {
				text_cursor_x += current_font->getTextExtent(os.str()).getWidth();
			} else {
				// TODO?
				// gui.lookUpFont((eFont)(font+4))
				text_cursor_x += current_font->getTextExtent(os.str()).getWidth();
			} // TODO
		}
		// TODO zeilenuebergreifende BOLDs und TEXTBUTTONs
// reached end of line? Calculate line and draw it
			
		if((text_cursor_x > getWidth()-10) || new_line || new_check)
		{
			new_check = false;
			
//			if((((bold==1)&&((text_cursor_x>getWidth()-10)||(big==1)))||(bold==2))&&(((big==1)&&((text_cursor_x>getWidth()-10)||(bold==1)))||(big==2)))

			if(((bold==1) && (big==2)) || (big==1)&&(bold==2))
			{
				if(bold==2) {
					bold=0;
				}
				if(big==2) {
					big=0;
				}
				current_color = color2;
				current_font = bigFontType->getBoldFont();

			} else 	if(((bold==1)&&((text_cursor_x>getWidth()-10)||(big==1)))||(bold==2))
			{
				if(bold==2) {
					bold=0;
				}
                current_color = color2;
				current_font = fontType->getBoldFont();
			} else 	if(((big==1)&&((text_cursor_x>getWidth()-10)||(bold==1)))||(big==2))
			{
				if(big==2) {
					big=0;
				}
				current_color = color1;
				current_font = bigFontType->getNormalFont();
			} else
			{
				current_color = color1;
				current_font = fontType->getNormalFont();
			}
			std::ostringstream realstring;
			realstring.str("");
			for(unsigned int j = first_char_position; j < last_char_position; j++)
			{
				if((text[j]=='$')||(text[j]=='>')||(text[j]=='`')||(text[j]=='#')||(text[j]=='^')||(text[j]=='&')||(text[j]=='@')||(text[j]=='~'))//||(text[j]<32))
				// Sonderzeichen!!!
					continue;
				realstring << text[j];
			}
			if((realstring.str().length()>0)||((textbutton>1)&& current_bitmap ))
			{
				if((textbutton==3)||(textbutton==5))
				{
					int this_number = atoi(realstring.str().c_str());
					longNumber.push_back(this_number);
					if(textbutton==5) {
						longNumber.push_back(this_number); // ok... wenn button ueber zwei Zeilen geht
					}
					if(current_bitmap)
					{
						longNumber.push_back(this_number);	
						current_bitmap = NULL;	
					}
					
					// visited button?
/*					if((useHelpChapters) && (gui.isVisitedHelpChapter(this_number)))
					{
						std::list<UI_Button*>::iterator h = longButton.end();
						--h;
						if(textbutton==5) --h;
						(*h)->setButtonColorsType(visitedButton);++h;
						if(textbutton==5)
							(*h)->setButtonColorsType(visitedButton);
					}*/

					text_cursor_x = first_text_cursor_x;
					textbutton=0;
				} else /*if((textbutton==2)||(textbutton==4))
				{
					current_font = fontType->getUnderlinedFont();
					unsigned int height = current_font->getTextExtent(realstring.str()).getHeight();
					// TODO
					unsigned int ty;
					if(current_row+maxdy/2 < height/2) {
						ty = 0;
					} else {
						ty = current_row+maxdy/2-height/2+2;
					}
					UI_Button* t;
					unsigned int old_x = first_text_cursor_x;
					if(current_bitmap)
					{
						t = new UI_Button(scrollBar==NULL?this:scrollBar, Rect(p+Point(first_text_cursor_x-1, ty), Size(0, 0)), Size(0, 0), button, current_bitmap, PRESS_BUTTON_MODE, NULL, DO_NOT_ADJUST, current_font, AUTO_SIZE);
						longButton.push_back(t);
						
						text_cursor_x = (*current_bitmap)->w + first_text_cursor_x;
						first_text_cursor_x = text_cursor_x;
						bitmap_left_border = text_cursor_x;
						bitmap_lower_border = current_row + (*current_bitmap)->h;
					}
					if(realstring.str() != "")
					{
						t = new UI_Button(scrollBar==NULL?this:scrollBar, Rect(p+Point(first_text_cursor_x+5, ty), Size(0, 0)), Size(0, 0), button, NULL, PRESS_BUTTON_MODE, realstring.str(), DO_NOT_ADJUST, current_font, AUTO_SIZE);
						longButton.push_back(t);
						text_cursor_x = first_text_cursor_x + t->getTextWidth() + current_font->getTextExtent(" ").getWidth();
					}
				} else if((textbutton==1)&&(!end_of_order))
				{
					// abgebrochener button
					if(text_cursor_x>getWidth()-10)
					{
						current_font = fontType->getUnderlinedFont();
						unsigned int height = current_font->getTextExtent(realstring.str()).getHeight();
						unsigned int ty;
						if(current_row+maxdy/2 < height/2) {
							ty = 0;
						} else {
							ty = current_row+maxdy/2-height/2+2;
						}

						UI_Button* t;
						if(current_bitmap)
						{
							t = new UI_Button(scrollBar==NULL?this:scrollBar, Rect(p+Point(first_text_cursor_x-1, ty), Size(0, 0)), Size(0, 0), button, current_bitmap, PRESS_BUTTON_MODE, NULL, DO_NOT_ADJUST, current_font, AUTO_SIZE);
							longButton.push_back(t);
							text_cursor_x = (*current_bitmap)->w + first_text_cursor_x;
							first_text_cursor_x = text_cursor_x;
							bitmap_left_border = text_cursor_x;
							bitmap_lower_border = current_row + (*current_bitmap)->h;
						}
						t = new UI_Button(scrollBar==NULL?this:scrollBar, Rect(p+Point(first_text_cursor_x+5, ty), Size(0, 0)), Size(0, 0), button, NULL, PRESS_BUTTON_MODE, realstring.str(), DO_NOT_ADJUST, current_font, AUTO_SIZE);
						longButton.push_back(t);
						textbutton=3;
					} else*/ {
						continue;
					}
				} else /*if(bitmap==2) {
					bitmap = 0; 
					current_bitmap = gui.getBitmapFromIdentifier(realstring.str());
					// TODO
					if(current_bitmap == NULL) {
						throw MyException("ERROR (UI_LongText::updateText()): Bitmap identifier " + realstring.str() + " not recognized => ignoring bitmap.");
					}
					else
					{
						if((align_next_bitmap_right)&&(textbutton==0))
						{
							UI_Bitmap* t = new UI_Bitmap(scrollBar == NULL?this:scrollBar, Rect(), Size(), current_bitmap);
							if(t->getBitmapHeight() < maxdy) {
								t->setOriginalPosition(t->getOriginalPosition() + Size(0, 1+(maxdy - t->getBitmapHeight())/2)); //?
							}
							t->setOriginalPosition(Point(this->getRelativeRightBound() - t->getBitmapWidth() - 5, t->getOriginalPosition().getY()));
							longBitmap.push_back(t);
							current_bitmap = NULL;
							align_next_bitmap_right = false;
							text_cursor_x-= current_font->getTextExtent(realstring.str()).getWidth();
						} else
						{
							
							UI_Bitmap* t = new UI_Bitmap(scrollBar==NULL?this:scrollBar, Rect(p+Point(first_text_cursor_x, current_row), Size(0, 0)), Size(0, 0), current_bitmap);
							if(t->getBitmapHeight() < maxdy) {
								t->setOriginalPosition(t->getOriginalPosition() + Size(0, 1+(maxdy - t->getBitmapHeight())/2)); //?
							}
							text_cursor_x-= current_font->getTextExtent(realstring.str()).getWidth();
							// TODO... ein Zaehler mit linkerRand und linkerRandEnde machen
 							if(textbutton == 0)
							{
								text_cursor_x+=t->getBitmapWidth();
								bitmap_left_border = text_cursor_x;
								bitmap_lower_border = current_row+t->getBitmapHeight();
								longBitmap.push_back(t);
								current_bitmap = NULL;
							}
							else {
								delete t;
							}
						}
					}
				} else*/
				{
					unsigned int height = current_font->getTextExtent(realstring.str()).getHeight();
					unsigned int ty;
					if(current_row+maxdy/2 < height/2) {
						ty = 0;
					} else { 
						ty = current_row+maxdy/2-height/2+2;
					}
					UI_StaticText* t = new UI_StaticText(scrollBar==NULL?this:scrollBar, Rect(p+Point(first_text_cursor_x, ty), Size(0,0)), Size(0,0), realstring.str(), current_color, current_font, DO_NOT_ADJUST);
					longText.push_back(t);
					text_cursor_x = first_text_cursor_x + current_font->getTextExtent(realstring.str()).getWidth() ;
				}
			}
			first_char_position = last_char_position;
			first_text_cursor_x = text_cursor_x;

			if((new_line)||(text_cursor_x>getWidth()-10))
			{
				new_line = false;
				if(move_below_bitmap && (bitmap_lower_border > current_row)) {
					current_row = bitmap_lower_border;
				} else {
					current_row += maxdy;
				}
				last_char_position = 0;
				if(current_row >= bitmap_lower_border) {
					bitmap_left_border = 5;
				}
				first_text_cursor_x = bitmap_left_border;
				text_cursor_x = bitmap_left_border;
// this must be resetted!
				must_not_make_new_line = false;
			}
		}
	
//	textWasChanged=true; ?

}

const Uint16 UI_LongText::getTextHeight() const
{
	Uint16 text_height = 0;
	if((longText.size()>0)&&(text_height < longText.back()->getRelativeLowerBound())) {
		text_height = longText.back()->getRelativeLowerBound();
	}
	if((longButton.size()>0)&&(text_height < longButton.back()->getRelativeLowerBound())) {
		text_height = longButton.back()->getRelativeLowerBound();
	}
	if((longBitmap.size()>0)&&(text_height < longBitmap.back()->getRelativeLowerBound())) {
		text_height = longBitmap.back()->getRelativeLowerBound();
	}
	return(text_height);
}

const Uint16 UI_LongText::getTextWidth() const
{
	Uint16 text_width = 0;

	for(std::list<UI_StaticText*>::const_iterator i = longText.begin(); i != longText.end(); ++i) {
		if(text_width < (*i)->getRelativeRightBound()) {
			text_width = (*i)->getRelativeRightBound();
		}
	}
	
	for(std::list<UI_Button*>::const_iterator i = longButton.begin(); i != longButton.end(); ++i) {
		if(text_width < (*i)->getRelativeRightBound()) {
			text_width = (*i)->getRelativeRightBound();
		}
	}
	
	for(std::list<UI_Bitmap*>::const_iterator i = longBitmap.begin(); i != longBitmap.end(); ++i) {
		if(text_width < (*i)->getRelativeRightBound()) {
			text_width = (*i)->getRelativeRightBound();
		}
	}

	return(text_width);
}

const signed int UI_LongText::getPressed() const
{
	std::list<UI_Button*>::const_iterator i = longButton.begin();
	std::list<signed int>::const_iterator j = longNumber.begin();
	while(i!=longButton.end())
	{
		if((*i)->isLeftClicked()) {
			return(*j);
		}
		++j;
		++i;
	}
	return(-1);
}


