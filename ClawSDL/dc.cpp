#include "dc.hpp"
#include <stdlib.h>

// ---- DRIVER AND INIITIALIZATION ----

DC::DC(const char* chosen_driver):
	surface(NULL),
	oldSurface(NULL),
	pressedRectangle(false),
	brush(),
	pen(),
	textColor(),
	font(),
// evtl gar nicht initialisieren...
	bitDepth(DEPTH_32BIT),
	bits(0),
	resolution(Size()),
	Draw_HLine(NULL),
	Draw_VLine(NULL),
	Draw_Line(NULL),
	DrawFilledRound(NULL),
	DrawFilledEdgedRound(NULL),
	DrawEmptyEdgedRound(NULL),
	DrawEmptyRound(NULL),
	DrawFilledEdgedBorderRound(NULL),
	changedRectangles(0),
	max_x(0),
	max_y(0),
	currentDriver(chosen_driver) 
{
	std::ostringstream video;
	video.str("");
	video << "SDL_VIDEODRIVER=" << currentDriver;
	char* video_cstr = new char[strlen(video.str().c_str())];

	strcpy(video_cstr, video.str().c_str());
	putenv(video_cstr);
}


const bool DC::initSDL(const Size& current_resolution, const eBitDepth bit_depth, const Uint32 nflags, const Uint32 initflags)
{
	if ( SDL_Init(initflags) < 0 )
	{
		std::string error = "ERROR (DC::initSDL()): Could not initialize SDL (" + std::string(SDL_GetError()) + ")."; 
		throw MyException(error);
	}
	atexit(SDL_Quit);
	return setScreen(current_resolution, bit_depth, nflags);
}

DC::~DC()
{
	SDL_FreeSurface(surface);
}

const std::list<std::string> DC::getAvailableDrivers()
{
	std::list<std::string> available_drivers;
#ifdef WIN32
	available_drivers.push_back("directx");
	available_drivers.push_back("windib");
#elif __linux__
	available_drivers.push_back("x11");
	available_drivers.push_back("dga");
	available_drivers.push_back("nano");
	available_drivers.push_back("fbcon");
	available_drivers.push_back("directfb");
	available_drivers.push_back("svgalib");	
	available_drivers.push_back("aalib");
#endif
	return(available_drivers);
}





const eChooseDriverError DC::chooseDriver(std::string& chosen_driver, const std::string& selected_driver)
{
	chosen_driver = "NO DRIVER";
	if(getenv("SDL_VIDEODRIVER")!=NULL)
		chosen_driver = getenv("SDL_VIDEODRIVER");

	/*chosen_driver = "NO DRIVER";
	char* driverVar;
	size_t requiredSize;
	getenv_s( &requiredSize, NULL, 0, "SDL_VIDEODRIVER");
    driverVar = (char*) malloc(requiredSize * sizeof(char));
	if (!driverVar) {
		throw MyException("ERROR (DC::chooseDriver()): Failed to allocate memory for driver names.");
    }

    getenv_s( &requiredSize, driverVar, requiredSize, "SDL_VIDEODRIVER" );

	if(driverVar != NULL) {
		chosen_driver = driverVar;
	}*/

	bool no_vo_argument = false; // TODO

	if(!selected_driver.empty()) {
		no_vo_argument = true;
	} else {
		chosen_driver = selected_driver;
	}

	
	std::list<std::string> available_drivers = DC::getAvailableDrivers();
	if(available_drivers.empty()) {
		return(NO_VIDEO_DRIVERS_AVAILABLE);
	}
	if(chosen_driver.empty() || chosen_driver == "NO DRIVER" || chosen_driver == "") {
		chosen_driver = *available_drivers.begin();
	}

	for(std::list<std::string>::const_iterator j = available_drivers.begin(); j != available_drivers.end(); ++j) {
		if(chosen_driver == *j)
		{
			return DRIVER_SUCCESS;
		}
	}

	return(SDL_DRIVER_NOT_SUPPORTED);
}

const bool DC::setScreen(const Size& current_resolution, const eBitDepth bit_depth, const Uint32 nflags)
{
	// no action
	if((current_resolution == resolution) && (bit_depth == bitDepth) && (surface != NULL)) {
		return false;
	}

	resolution = current_resolution;
	max_x = resolution.getWidth();
	max_y = resolution.getHeight();
	switch(bit_depth)
	{
		case DEPTH_8BIT:bits = 8;break;
		case DEPTH_16BIT:bits = 16;break;
		case DEPTH_24BIT:bits = 24;break;
		case DEPTH_32BIT:bits = 32;break;
		default:assert(false);break;
	}
//	const SDL_VideoInfo* hardware = SDL_GetVideoInfo();
//	if(bits > (unsigned int)hardware->vfmt->BitsPerPixel)
//		bits = (unsigned int)hardware->vfmt->BitsPerPixel; :(

	surface = SDL_SetVideoMode(max_x, max_y, bits, nflags);
	if(surface == NULL) {
		std::string error = "ERROR (DC::setScreen()): Could not set video mode (" + std::string(SDL_GetError()) + ").";
		throw MyException(error);
	}

	changedRectangle[0] = createRect(0, 0, max_x, max_y);
	changedRectangles = 1;

	switch(surface->format->BitsPerPixel)
	{
		case 8:	bitDepth = DEPTH_8BIT;
			Draw_HLine = &DC::Draw_HLine_8bit;
			Draw_VLine = &DC::Draw_VLine_8bit;
			Draw_Line = &DC::Draw_Line_8bit;
			DrawFilledRound = &DC::DrawFilledRound_8bit;
			DrawFilledEdgedRound = &DC::DrawFilledEdgedRound_8bit;
			DrawEmptyEdgedRound = &DC::DrawEmptyEdgedRound_8bit;
			DrawEmptyRound = &DC::DrawEmptyRound_8bit;
			DrawFilledEdgedBorderRound = &DC::DrawFilledEdgedBorderRound_8bit;
			DrawTab = &DC::DrawTab_8bit;
			break;

		case 16:bitDepth = DEPTH_16BIT;
			Draw_HLine = &DC::Draw_HLine_16bit;
	        Draw_VLine = &DC::Draw_VLine_16bit;
	        Draw_Line = &DC::Draw_Line_16bit;
	        DrawFilledRound = &DC::DrawFilledRound_16bit;
	        DrawFilledEdgedRound = &DC::DrawFilledEdgedRound_16bit;
	        DrawEmptyEdgedRound = &DC::DrawEmptyEdgedRound_16bit;
	        DrawEmptyRound = &DC::DrawEmptyRound_16bit;
	        DrawFilledEdgedBorderRound = &DC::DrawFilledEdgedBorderRound_16bit;
			DrawTab = &DC::DrawTab_16bit;
			break;

		case 24:bitDepth = DEPTH_24BIT;
			Draw_HLine = &DC::Draw_HLine_24bit;
	        Draw_VLine = &DC::Draw_VLine_24bit;
	        Draw_Line = &DC::Draw_Line_24bit;
	        DrawFilledRound = &DC::DrawFilledRound_24bit;
	        DrawFilledEdgedRound = &DC::DrawFilledEdgedRound_24bit;
	        DrawEmptyEdgedRound = &DC::DrawEmptyEdgedRound_24bit;
	        DrawEmptyRound = &DC::DrawEmptyRound_24bit;
	        DrawFilledEdgedBorderRound = &DC::DrawFilledEdgedBorderRound_24bit;
			DrawTab = &DC::DrawTab_24bit;
			break;

		case 32:bitDepth = DEPTH_32BIT;
			Draw_HLine = &DC::Draw_HLine_32bit;
	        Draw_VLine = &DC::Draw_VLine_32bit;
	        Draw_Line = &DC::Draw_Line_32bit;
	        DrawFilledRound = &DC::DrawFilledRound_32bit;
	        DrawFilledEdgedRound = &DC::DrawFilledEdgedRound_32bit;
	        DrawEmptyEdgedRound = &DC::DrawEmptyEdgedRound_32bit;
	        DrawEmptyRound = &DC::DrawEmptyRound_32bit;
	        DrawFilledEdgedBorderRound = &DC::DrawFilledEdgedBorderRound_32bit;
			DrawTab = &DC::DrawTab_32bit;
			break;

		default:assert(false);break;
	}
	return true;
}


const bool DC::Lock() const {
	assert(surface);
	if(SDL_MUSTLOCK(surface)) {
		return SDL_LockSurface(surface) == 0;
	}
	else return true;
}

void DC::Unlock() const {
	assert(surface);
	if(SDL_MUSTLOCK(surface)) {
		SDL_UnlockSurface(surface);
	}
}


/*void DC::setColor(const Color* dc_color) 
{
	assert(dc_color);
	color = dc_color;
}*/

void DC::setFont(const Font& dc_font) 
{
	font = dc_font;
}

// ----- SURFACE OPERATIONS


void DC::switchToSurface(SDL_Surface* temp_surface) 
{
	assert(oldSurface == NULL);
	oldSurface = surface;
	surface = temp_surface;
}

void DC::switchToOriginalSurface()
{
	assert(oldSurface);
	surface = oldSurface;
	oldSurface = NULL;
}


const bool DC::setFullScreen(const bool full_screen)
{
	assert(surface);
	if(isFullScreen() != full_screen)
	{
#ifdef __linux
		SDL_WM_ToggleFullScreen(surface);
#elif __WIN32__
		Uint32 nflags = surface->flags;
		nflags ^= SDL_FULLSCREEN;
		surface = SDL_SetVideoMode(surface->w, surface->h, surface->format->BitsPerPixel, nflags);
#endif
		return true;
	}				
	return false;
}






// ----- SCREEN UPDATE 

void DC::addRectangle(const Rect& rect)
{
//Größe des Rechtecks (größere testen ob sie es beinhalten, kleinere testen ob sie umgeben werden)
//Position des Rechtecks: rechte untere Ecke, linke obere Ecke, Quadtrees, dynamisch angepasste Trees

	if((changedRectangles >= DC_MAX_CHANGED_RECTANGLES) || (rect.getLeft() >= max_x) || (rect.getTop() >= max_y) || (rect.getRight() < 0) || (rect.getBottom() < 0)) {
		return;
	}

// cut borders
	Point p = Point(rect.getLeft() < 0  ? 0 : rect.getLeft(),
					rect.getTop() < 0   ? 0 : rect.getTop());
	Size s = Size(rect.getRight()  >= max_x ? max_x - rect.getLeft() : rect.getWidth(),
				  rect.getBottom() >= max_y ? max_y - rect.getTop()  : rect.getHeight());
		                            
	SDL_Rect r = createRect(Rect(p, s));
	


	for(unsigned int i = 0; i < changedRectangles; i ++)
	{
		if(
			(r.x >= changedRectangle[i].x)&&
			(r.x + r.w <= changedRectangle[i].x + changedRectangle[i].w)&&
			(r.y >= changedRectangle[i].y)&&
			(r.y + r.h <= changedRectangle[i].y + changedRectangle[i].h)
		)
			return;
		if(
			(r.x <= changedRectangle[i].x)&&
			(r.x + r.w >= changedRectangle[i].x + changedRectangle[i].w)&&
			(r.y <= changedRectangle[i].y)&&
			(r.y + r.h >= changedRectangle[i].y + changedRectangle[i].h)
		)
		{
			changedRectangle[i] = r;
			return;
		}
	}
	
	changedRectangle[changedRectangles] = r;
	changedRectangles++;
}



void DC::updateScreen()
{
	assert(surface);
//	SDL_UpdateRect(surface, 0, 0, this->getMaxX(), this->getMaxY());
	SDL_UpdateRects(surface, changedRectangles, changedRectangle);
	changedRectangles = 0;
}

const char* DC::dcVersion = "2.00";