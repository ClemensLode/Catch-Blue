#ifndef _FONTTYPE_HPP
#define _FONTTYPE_HPP

#include <font.hpp>

class FontType
{
	public:
		FontType(Font* font,
			Font* boldFont,
			Font* shadowFont,
			Font* underlinedFont,
			Font* shadowBoldFont,
			Font* shadowUnderlinedFont,
			Font* underlinedBoldFont,
			Font* shadowUnderlinedBoldFont
		);
		~FontType();

		const Font* getNormalFont() const {return font;}
		const Font* getBoldFont() const {return boldFont;}
		const Font* getShadowFont() const {return shadowFont;}
		const Font* getUnderlinedFont() const {return underlinedFont;}
		const Font* getShadowBoldFont() const {return shadowBoldFont;}
		const Font* getShadowUnderlinedFont() const {return shadowUnderlinedFont;}
		const Font* getUnderlinedBoldFont() const {return underlinedBoldFont;}
		const Font* getShadowUnderlinedBoldFont() const {return shadowUnderlinedBoldFont;}

	private:
		Font* font;
		Font* boldFont;
		Font* shadowFont;
		Font* underlinedFont;
		Font* shadowBoldFont;
		Font* shadowUnderlinedFont;
		Font* underlinedBoldFont;
		Font* shadowUnderlinedBoldFont;
};

#endif