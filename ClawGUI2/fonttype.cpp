#include "fonttype.hpp"

FontType::FontType(Font* font,
		 Font* boldFont,
		 Font* shadowFont,
		 Font* underlinedFont,
		 Font* shadowBoldFont,
		 Font* shadowUnderlinedFont,
		 Font* underlinedBoldFont,
		 Font* shadowUnderlinedBoldFont
		 ):
	font(font),
	boldFont(boldFont),
	shadowFont(shadowFont),
	underlinedFont(underlinedFont),
	shadowBoldFont(shadowBoldFont),
	shadowUnderlinedFont(shadowUnderlinedFont),
	underlinedBoldFont(underlinedBoldFont),
	shadowUnderlinedBoldFont(shadowUnderlinedBoldFont)
{
}

FontType::~FontType() {}