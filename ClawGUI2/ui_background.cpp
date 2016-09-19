#include "ui_background.hpp"

UI_BackGround::UI_BackGround(const Size& size, SDL_Surface* background_bitmap) : 
	UI_Object( NULL, Rect(Point(0,0), size), Size(), DO_NOT_ADJUST),
		backgroundBitmap(background_bitmap)
{
	makePufferInvalid();
}

UI_BackGround::~UI_BackGround()
{}

void UI_BackGround::reloadOriginalSize()
{
//	setOriginalSize(gui.getCurrentResolutionSize());
	UI_Object::reloadOriginalSize();
}

void UI_BackGround::process()
{
	UI_Object::process();
}

void UI_BackGround::draw(DC* dc) const
{
	assert(dc);

	UI_Object::draw(dc);
	if(backgroundBitmap)
	{
		SDL_Rect rc = DC::createRect(0, 0, dc->getMaxX(), dc->getMaxY());
		dc->Blit(backgroundBitmap, rc);
	}
	else {
		dc->clearScreen();
	}

	// TODO evtl Versionsnummern an background übergeben!
	/*dc->setFont(gui.lookUpFont(SMALL_FONT));
	Size s = dc->getTextExtent(CORE_VERSION);	
	dc->setTextForeground(DC::toSDL_Color(100, 100, 100));
	dc->DrawText(CORE_VERSION, Point(dc->getMaxX() - s.getWidth() - 10, dc->getMaxY() - s.getHeight() - 2));
	*/	
	//gui.setColorTheme(gui.getTheme().getMainColorTheme());
	
#if 0
// ------ MOUSE DRAWING ------
		if(efConfiguration.isSoftwareMouse())
		{
//			SDL_ShowCursor(SDL_DISABLE);
			Point p = UI_Object::mouse - Size(20,10);//Point(90, 140);
			dc->setFont(gui.lookUpFont(SMALL_ITALICS_BOLD_FONT));
			switch(UI_Object::mouseType)
			{
				case 0://dc->DrawBitmap(*gui.lookUpBitmap(MOUSE_NONE), p);
					break;
				case 1:
//					dc->DrawBitmap(*gui.lookUpBitmap(MOUSE_LEFT), p);
					dc->setTextForeground(DC::toSDL_Color(179,0,0));
					dc->DrawText("Add a unit", p.x-50, p.y+2);
				break;
				case 2:
//					dc->DrawBitmap(*gui.lookUpBitmap(MOUSE_RIGHT), p);
					dc->setTextForeground(DC::toSDL_Color(0,177,188));
					dc->DrawText("Remove a unit", p.x+38, p.y+1);
				break;
				case 3:
//					dc->DrawBitmap(*gui.lookUpBitmap(MOUSE_BOTH), p);
					dc->setTextForeground(DC::toSDL_Color(179,0,0));
					dc->DrawText("Add a unit", p.x-50, p.y+2);
					dc->setTextForeground(DC::toSDL_Color(0,177,188));
					dc->DrawText("Remove a unit", p.x+38, p.y+1);
				break;
				default:toLog("error, mouseType out of range");break;
			}
		}
//		else
//			SDL_ShowCursor(SDL_ENABLE);
// ------ END MOUSE DRAWING ------
#endif
}


