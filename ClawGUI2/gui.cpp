#include "gui.hpp"
#include <sstream>
#include <fstream>
#include <iomanip>
#include <SDL_Object.hpp>
#include "ui_button.hpp"

#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
#define MUSIC_TRANSITION_DURATION 2000
#endif 

#ifdef _FMOD_SOUND
	#include <fmod.hpp>
	#include <fmod_errors.h>
#endif

GUI* GUI::createInstance(const std::list<std::string>& parameter_list, const std::string& window_title) {
	assert(gui == NULL);

	try {
		gui = new GUI(parameter_list, window_title);
	} catch(MyException e) {
		toErrorLog(e.what());
		return NULL;
	}
	return gui;
}


GUI::GUI(const std::list<std::string>& parameter_list, const std::string& window_title) : 
	dc(NULL),
	configurationFile("settings/ui.cfg"),
#ifdef _FMOD_SOUND
	soundEngine(NULL),
	musicChannel(NULL),
#endif
#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
	soundInitialized(true),
#endif
	defaultCursor(SDL_GetCursor())
{
	toInitLog("[Loading GUI language files...]");
	guiStrings.loadStringFiles(findFiles("strings"));
	guiStrings.setLanguage("ENGLISH");

	eBitDepth bit_depth = DEPTH_32BIT;
	Size res_size = Size(640, 480);
	bool is_fullscreen = false;
	toInitLog("[Loading GUI configuration file...]");
	loadConfigurationFile(res_size, bit_depth, is_fullscreen);


// ------ INIT SDL AND WINDOW ------

	toInitLog("* " + guiStrings.lookUpString("START_INIT_SDL_STRING"));
	toInitLog("* " + guiStrings.lookUpString("START_AVAILABLE_GRAPHIC_DRIVERS_STRING"));

	std::ostringstream os;
	os << "* " << guiStrings.lookUpString("START_AVAILABLE_GRAPHIC_DRIVERS_STRING");
	std::list<std::string> s = DC::getAvailableDrivers();
	for(std::list<std::string>::const_iterator i = s.begin(); i!=s.end(); i++) {
		os << *i << " ";
	}
	toInitLog(os.str());


	std::string selected_driver;
	bool no_vo_argument = false;
	for(std::list<std::string>::const_iterator i = parameter_list.begin(); i != parameter_list.end(); ++i) {
		if((*i) == "-vo")
		{
			i++;
			if(i == parameter_list.end()) {
				no_vo_argument = true;
			}
			else {
				selected_driver = *i;
			}
			break;
		}
	}

	std::string chosen_driver;
	switch(DC::chooseDriver(chosen_driver, selected_driver))
	{
		case DRIVER_SUCCESS:toInitLog("* " + guiStrings.lookUpFormattedString("START_SDL_USING_DRIVER_STRING", chosen_driver));break;
		case NO_VIDEO_DRIVERS_AVAILABLE:throw MyException("* " + guiStrings.lookUpString("START_ERROR_NO_DRIVER_AVAILABLE_STRING"));
		case SDL_DRIVER_NOT_SUPPORTED:throw MyException("* " + guiStrings.lookUpFormattedString("START_ERROR_DRIVER_NOT_SUPPORTED_STRING", chosen_driver));
	}
	dc = new DC(chosen_driver.c_str());
	assert(dc);

	Uint32 flags = SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_HWACCEL|SDL_HWPALETTE|SDL_SRCCOLORKEY|SDL_RLEACCEL|SDL_SRCALPHA|SDL_PREALLOC|SDL_DOUBLEBUF;

	toInitLog("* " + guiStrings.lookUpString(is_fullscreen ? "START_SET_FULLSCREEN_MODE_STRING" : "START_SET_WINDOW_MODE_STRING"));
	if(is_fullscreen) {
		flags |= SDL_FULLSCREEN;
	}
#ifdef _SDL_MIXER_SOUND
	flags |= SDL_INIT_AUDIO;
#endif 
	try {
		dc->initSDL(res_size, bit_depth, flags, SDL_INIT_NOPARACHUTE | SDL_INIT_VIDEO);
	} catch(...) {
		delete dc;
		dc = NULL;
		throw MyException(guiStrings.lookUpString("START_UNABLE_TO_INIT_SDL_STRING") + " [SDL ERROR: \"" + SDL_GetError() + "\"]");
	}
	
	if ( !dc->valid() ) {
		delete dc;
		dc = NULL;
		throw MyException(guiStrings.lookUpString("START_ERROR_SETTING_VIDEO_MODE_STRING") + " [SDL ERROR: \"" + SDL_GetError() + "\"]");
	}

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	toInitLog(printHardwareInformation());
	toInitLog("* " + guiStrings.lookUpString("START_CREATED_SURFACE_STRING") + " " + printSurfaceInformation(dc));
// ------ END INIT SDL AND WINDOW ------

	SDL_WM_SetCaption(window_title.c_str(),"");

// ------ INIT SDL_TTF ------
	toInitLog("* " + guiStrings.lookUpString("START_INIT_SDL_TRUETYPE_FONTS_STRING"));
	if(TTF_Init()==-1) {
		delete dc;
		dc = NULL;
		throw MyException(guiStrings.lookUpString("START_INIT_SDL_TTF_ERROR_STRING") + " [\"" + TTF_GetError() + "\"]");
	}
	atexit(TTF_Quit); 
// ------ END INIT SDL_TTF ------

// ------ INIT SOUND ENGINE -------
#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
	if(isSound() || isMusic())
	{
		try {
			initSoundEngine();
		} catch(MyException e) {
			setSound(false);
			setMusic(false);
			soundInitialized = false;
		}
	} else {
		toInitLog(guiStrings.lookUpString("START_INIT_NOSOUND_STRING"));
	}
#endif
// ------ END INIT SOUND ENGINE -------
}

void GUI::deleteInstance() {
	delete gui;
	gui = NULL;
}


GUI::~GUI()
{
#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
	releaseSoundEngine();
#endif

//	toInitLog("* " + guiStrings.lookUpString("END_RESET_MOUSE_CURSOR_STRING"));
//	SDL_SetCursor(defaultCursor);

// TODO: Jeder Mauszeiger der benutzt wurde verursacht bei SDL_FreeCursor einen segfault :/ (bzw. 'wurde schon geloescht' Fehler der glibc
//	for(unsigned int i = MAX_CURSORS; i--;)
//		for(unsigned int j = 2; j--;)
//	SDL_FreeCursor(cursorList[ARROW_CURSOR][0]);
//	SDL_FreeCursor(cursorList[HAND_CURSOR][0]);
//	SDL_FreeCursor(cursorList[CLOCK_CURSOR][0]);
//	SDL_FreeCursor(cursorList[CLOCK_CURSOR][1]);

/*	toInitLog("* " + guiStrings.lookUpString(END_FREEING_COLORS_BRUSHES_PENS_STRING"));
	for(unsigned int i = MAX_COLOR_THEMES;i--;)
	{
		for(unsigned int j = MAX_COLORS;j--;)
			delete colorList[i][j];
		for(unsigned int j = MAX_BRUSHES;j--;)
			delete brushList[i][j];
		for(unsigned int j = MAX_PENS;j--;)
			delete penList[i][j];
	}

	toInitLog("* " + guiStrings.lookUpString(END_FREEING_BITMAPS_STRING"));
	for(std::list<BitmapEntry>::iterator l = loadedBitmaps.begin(); l!=loadedBitmaps.end();++l) {
		SDL_FreeSurface(l->bitmap);
	}

#ifdef USE_SMPEG
	toInitLog("* " + guiStrings.lookUpString(END_FREEING_ANIMATIONS_STRING"));
	for(std::list<AnimationEntry>::iterator l = loadedAnimations.begin(); l!=loadedAnimations.end();++l)
		if(l->animation)
			SMPEG_delete(l->animation);
#endif

	toInitLog("* " + guiStrings.lookUpString(END_FREEING_COORDINATES_STRING"));
        for(unsigned int i = MAX_RESOLUTIONS;i--;)
        {
                for(unsigned int j = MAX_GLOBAL_WINDOWS;j--;)
			delete globalRectList[i][j];
                for(unsigned int j = MAX_COMPARE_GAMES;j--;)
                        for(unsigned int k = MAX_COMPARE_GAMES;k--;)
			{
                                for(unsigned int l = MAX_GAME_WINDOWS;l--;)
                                        delete gameRectList[i][j][k][l];
				for(unsigned int l = MAX_PLAYER;l--;)
					for(unsigned int m = MAX_PLAYER;m--;)
						for(unsigned int n = MAX_PLAYER_WINDOWS;n--;)
							delete playerRectList[i][j][k][l][m][n];
			}
        }

	toInitLog("* " + guiStrings.lookUpString(END_FREEING_BUTTONS_STRING"));
	for(unsigned int i=MAX_BUTTON_COLORS_TYPES;i--;)
		delete buttonColorsList[i];

	toInitLog("* " + guiStrings.lookUpString(END_FREEING_FONTS_STRING"));
	for(unsigned int i = MAX_RESOLUTIONS;i--;)
//		for(unsigned int j = MAX_LANGUAGES;j--;)
			for(unsigned int k = MAX_FONTS;k--;)
				delete fontList[i][k];
*/
}



void GUI::initDefaults()
{
//	setLanguage("ENGLISH"); TODO
	setGlowingButtons(true);
	setTransparency(false);
	setSmoothMovements(true);
	setUnloadGraphics(false);

	setDesiredFramerate(MAX_DESIRED_FRAMERATE);
	setDesiredCPU(99);
	configurationFile = "settings/ui.cfg";

#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
	setMusicVolume(75);
	setSoundVolume(75);
	setSoundChannels(16);
	setMusic(true);
	setSound(true);
	setDesiredFramerate(25);
	setDesiredCPU(75);
#endif
}


void GUI::resetWindow()
{
	windowSelected = false;
	currentWindow = NULL;
}





void GUI::addMessage(const std::string& msg)
{
	remainingMessages.push_back(msg);
}



std::string GUI::printHardwareInformation()
{
	// TODO: uebersetzen bzw. dem Aufrufer nur Daten uebergeben
	SDL_Rect **modes;
	std::ostringstream os;
	os.str("");
	modes = SDL_ListModes(NULL, SDL_SWSURFACE);
	if(modes == (SDL_Rect **)0) {
		os << "* " << guiStrings.lookUpString("START_SDL_NO_MODES_AVAILABLE_STRING") << std::endl;
	}
	else
	{
		if(modes == (SDL_Rect **)-1) {
			os << "* " << guiStrings.lookUpString("START_SDL_ALL_RESOLUTIONS_AVAILABLE_STRING") << std::endl;
		} else
		{
			os << "* " << guiStrings.lookUpString("START_SDL_AVAILABLE_MODES_STRING");
			for(unsigned int i=0; modes[i]; ++i) {
				os << "  " << modes[i]->w << " x " << modes[i]->h;
			}
			os << std::endl;
		}
	}
	const SDL_VideoInfo* hardware = SDL_GetVideoInfo();
	os << " - " << guiStrings.lookUpString("START_SDL_MAX_COLOR_DEPTH_STRING") << (unsigned int)hardware->vfmt->BitsPerPixel;
//	if(hardware->hw_available) os << "\n- " << guiStrings.lookUpString("START_SDL_HARDWARE_SURFACES_POSSIBLE_STRING");
	if(hardware->wm_available) os << "\n - " << guiStrings.lookUpString("START_SDL_WINDOW_MANAGER_AVAILABLE_STRING");
	if(hardware->blit_hw) os << "\n - " << guiStrings.lookUpString("START_SDL_HARDWARE_TO_HARDWARE_BLITS_ACCELERATED_STRING");
	if(hardware->blit_hw_CC) os << "\n - " << guiStrings.lookUpString("START_SDL_HARDWARE_TO_HARDWARE_COLORKEY_BLITS_ACCELERATED_STRING");
	if(hardware->blit_hw_A) os << "\n - " << guiStrings.lookUpString("START_SDL_HARDWARE_TO_HARDWARE_ALPHA_BLITS_ACCELERATED_STRING");
	if(hardware->blit_sw) os << "\n - " << guiStrings.lookUpString("START_SDL_SOFTWARE_TO_HARDWARE_BLITS_ACCELERATED_STRING");
	if(hardware->blit_sw_CC) os << "\n - " << guiStrings.lookUpString("START_SDL_SOFTWARE_TO_HARDWARE_COLORKEY_BLITS_ACCELERATED_STRING");
	if(hardware->blit_sw_A)	os << "\n - " << guiStrings.lookUpString("START_SDL_SOFTWARE_TO_HARDWARE_ALPHA_BLITS_ACCELERATED_STRING");
	if(hardware->blit_fill)	os << "\n - " << guiStrings.lookUpString("START_SDL_COLOR_FILLS_ACCELERATED_STRING");
	if(hardware->video_mem>0) os << "\n - " << guiStrings.lookUpFormattedString("START_SDL_TOTAL_VIDEO_MEMORY_STRING", hardware->video_mem);
//	Total amount of video memory: " << hardware->video_mem << "kb";
	return(os.str());
}





// after a bit-depth change
/*void GUI::updateColors(SDL_Surface* surface)
{
	for(unsigned int i = MAX_COLOR_THEMES;i--;)
	{
		for(unsigned int j = MAX_COLORS;j--;)
			if(colorList[i][j]!=NULL)
				colorList[i][j]->updateColor(surface);
		for(unsigned int j = MAX_BRUSHES;j--;)
			if(brushList[i][j]!=NULL)
				brushList[i][j]->updateColor(surface);
		for(unsigned int j = MAX_PENS;j--;)
			if(penList[i][j]!=NULL)
				penList[i][j]->updateColor(surface);
	}
}*/



std::string GUI::printSurfaceInformation(DC* surface)
{
	std::ostringstream os; os.str("");
	os << surface->getSurface()->w << " x " << surface->getSurface()->h << " @ " << (unsigned int)(surface->getSurface()->format->BitsPerPixel);
	if (surface->flags() & SDL_SWSURFACE) os << "\n- " << guiStrings.lookUpString("START_SDL_SURFACE_STORED_IN_SYSTEM_MEMORY_STRING");
	//Surface is stored in system memory";
	else if(surface->flags() & SDL_HWSURFACE) os << "\n- " << guiStrings.lookUpString("START_SDL_SURFACE_STORED_IN_VIDEO_MEMORY_STRING");
//			Surface is stored in video memory";
	if(surface->flags() & SDL_ASYNCBLIT) os << "\n- " << guiStrings.lookUpString("START_SDL_SURFACE_USES_ASYNCHRONOUS_BLITS_STRING");
	//urface uses asynchronous blits if possible";
	if(surface->flags() & SDL_ANYFORMAT) os << "\n- " << guiStrings.lookUpString("START_SDL_SURFACE_ALLOWS_ANY_PIXEL_FORMAT_STRING");
	if(surface->flags() & SDL_HWPALETTE) os << "\n- " << guiStrings.lookUpString("START_SDL_SURFACE_HAS_EXCLUSIVE_PALETTE_STRING");
	if(surface->flags() & SDL_DOUBLEBUF) os << "\n- " << guiStrings.lookUpString("START_SDL_SURFACE_IS_DOUBLE_BUFFERED_STRING");
	if(surface->flags() & SDL_OPENGL) os << "\n- " << guiStrings.lookUpString("START_SDL_SURFACE_HAS_OPENGL_CONTEXT_STRING");
	if(surface->flags() & SDL_OPENGLBLIT) os << "\n- " << guiStrings.lookUpString("START_SDL_SURFACE_SUPPORTS_OPENGL_BLITTING_STRING");
	if(surface->flags() & SDL_RESIZABLE) os << "\n- " << guiStrings.lookUpString("START_SDL_SURFACE_IS_RESIZABLE_STRING");
	if(surface->flags() & SDL_HWACCEL) os << "\n- " << guiStrings.lookUpString("START_SDL_SURFACE_BLIT_USES_HARDWARE_ACCELERATION_STRING");
	//Surface blit uses hardware acceleration";
	if(surface->flags() & SDL_SRCCOLORKEY) os << "\n- " << guiStrings.lookUpString("START_SDL_SURFACE_USES_COLORKEY_BLITTING_STRING");
	//Surface use colorkey blitting";
	if(surface->flags() & SDL_RLEACCEL) os << "\n- " << guiStrings.lookUpString("START_SDL_COLORKEY_BLITTING_RLE_ACCELERATED_STRING");
	//Colorkey blitting is accelerated with RLE";
	if(surface->flags() & SDL_SRCALPHA) os << "\n- " << guiStrings.lookUpString("START_SDL_BLIT_USES_ALPHA_BLENDING_STRING");
//	Surface blit uses alpha blending";
	if(surface->flags() & SDL_PREALLOC) os << "\n- " << guiStrings.lookUpString("START_SDL_SURFACE_USES_PREALLOCATED_MEMORY_STRING");
	if(SDL_MUSTLOCK(surface->getSurface())) os << "\n- " << guiStrings.lookUpString("START_SDL_SURFACE_NEEDS_LOCKING_STRING");
	return(os.str());
}

const Text& GUI::lookUpText(const std::string& text_id) const {
	return guiStrings.lookUpText(text_id);
}
const std::string& GUI::lookUpString(const std::string& text_id) const {
	return guiStrings.lookUpString(text_id);
}


void GUI::loadConfigurationFile()
{
	// TODO sound?

	assert(dc);
	Size res_size = dc->getSize();
	eBitDepth bit_depth = dc->getBitDepth();
	bool full_screen = dc->isFullScreen();
	toInitLog(guiStrings.lookUpString("START_LOAD_UI_CONFIGURATION_STRING"));

	loadConfigurationFile(res_size, bit_depth, full_screen);

	if(res_size != dc->getSize() || bit_depth != dc->getBitDepth() || full_screen != dc->isFullScreen()) {
		dc->setScreen(res_size, bit_depth, dc->flags() | SDL_FULLSCREEN);
	}
}

void GUI::loadConfigurationFile(Size& res_size, eBitDepth& bit_depth, bool& full_screen) 
{
	std::ifstream pFile(configurationFile.c_str());
	try {
		checkStreamIsOpen(pFile, "GUI::loadConfigurationFile()", configurationFile);
	} catch(MyException e) {
		initDefaults();
		saveToFile();
		return;
	}
		
	std::fstream::pos_type old_pos = pFile.tellg();
	char line[1024];
	while(pFile.getline(line, sizeof line))
	{
		checkStreamForFailure(pFile, "GUI::loadConfigurationFile()", configurationFile);
		
		std::string text = line;
		size_t start = text.find_first_not_of("\t ");
		if((start==std::string::npos)||(text[0]=='#')||(text[0]=='\0')) {
			continue; // ignore line
		}
		size_t stop=text.find_first_of("\t ", start);
		if(stop==std::string::npos) stop=text.size();
		std::string index=text.substr(start, stop);
		std::string value;
		std::map<std::string, std::list<std::string> >::iterator i;
		if(index=="@SETTINGS")
		{
			std::map<std::string, std::list<std::string> > block;
			pFile.seekg(old_pos);
			parse_block_map(pFile, block);


			if((i = block.find("Language")) != block.end()){
				i->second.pop_front();
				try {
			   		setLanguage(i->second.front());
				} catch(...) {
					setLanguage("ENGLISH");
					guiStrings.setLanguageChosen(false);
				}
			} else {
				setLanguage("ENGLISH");
				guiStrings.setLanguageChosen(false);
			}

			if((i = block.find("Resolution")) != block.end()){
				i->second.pop_front();
				Uint16 rw = (Uint16)atoi(i->second.front().c_str());
				i->second.pop_front();
				Uint16 rh = (Uint16)atoi(i->second.front().c_str());
			   	res_size = Size(rw, rh);
			}
			if((i=block.find("Bit depth"))!=block.end()){
				i->second.pop_front();
			   	bit_depth = (eBitDepth)(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Fullscreen"))!=block.end()){
				i->second.pop_front();
			   	full_screen = (atoi(i->second.front().c_str()) != 0);
			}



			if((i=block.find("Desired framerate"))!=block.end()){
				i->second.pop_front();
			   	setDesiredFramerate(atoi(i->second.front().c_str()));
			}		
			if((i=block.find("Desired CPU usage"))!=block.end()){
				i->second.pop_front();
			   	setDesiredCPU(atoi(i->second.front().c_str()));
			}		
			if((i=block.find("Glowing buttons"))!=block.end()){
				i->second.pop_front();
			   	setGlowingButtons(atoi(i->second.front().c_str()) != 0);
			}
			if((i=block.find("Transparency"))!=block.end()){
				i->second.pop_front();
			   	setTransparency(atoi(i->second.front().c_str()) != 0);
			}
			if((i=block.find("Smooth movements"))!=block.end()){
				i->second.pop_front();
			   	setSmoothMovements(atoi(i->second.front().c_str()) != 0);
			}
			if((i=block.find("Unload graphics"))!=block.end()){
				i->second.pop_front();
			   	setUnloadGraphics(atoi(i->second.front().c_str()) != 0);
			}
			if((i=block.find("Tooltips"))!=block.end()){
				i->second.pop_front();
				setToolTips(atoi(i->second.front().c_str()) != 0);
			}

#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
			if((i=block.find("Music"))!=block.end()){
				i->second.pop_front();
			   	setMusic(atoi(i->second.front().c_str()) != 0);
			}
			if((i=block.find("Sound effects"))!=block.end()){
				i->second.pop_front();
			   	setSound(atoi(i->second.front().c_str()) != 0);
			}
			if((i=block.find("Music volume"))!=block.end()){
				i->second.pop_front();
			   	setMusicVolume(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Sound volume"))!=block.end()){
				i->second.pop_front();
			   	setSoundVolume(atoi(i->second.front().c_str()));
			}
			if((i=block.find("Sound channels"))!=block.end()){
				i->second.pop_front();
			   	setSoundChannels(atoi(i->second.front().c_str()));
			}
#endif

			
		}
		old_pos = pFile.tellg();
	}// END while
} // schoen :)


//void GUI::animateWaitCursor();



void GUI::saveToFile() const
{
	std::ofstream pFile(configurationFile.c_str(), std::ios_base::out | std::ios_base::trunc);

	checkStreamIsOpen(pFile, "saveToFile", configurationFile);

	pFile << "@SETTINGS" << std::endl;
	pFile << "# ENGLISH, GERMAN, ..." << std::endl;
	pFile << "    \"Language\" = \"" << getLanguage() << "\"" << std::endl;
	pFile << "# e.g. 640x480, 800x600, 1024x768, 1280x1024, ..." << std::endl;
	if(dc != NULL) {
		pFile << "    \"Resolution\" = \"" << getResolution().toString() << "\"" << std::endl;
	} else {
		pFile << "    \"Resolution\" = \"640\" \"480\"" << std::endl;
	}
	pFile << "# 0 = 256 colors, 1 = 64k colors, 2 = 16mio colors, 3 = 4mrd colors" << std::endl;
	if(dc != NULL) {
		pFile << "    \"Bit depth\" = \"" << (int)getBitDepth() << "\"" << std::endl;
	} else {
		pFile << "    \"Bit depth\" = \"3\"" << std::endl;
	}


	pFile << "    \"Desired framerate\" = \"" << getDesiredFramerate() << "\"" << std::endl;
	pFile << "# Desired CPU usage" << std::endl;
	pFile << "    \"Desired CPU usage\" = \"" << getDesiredCPU() << "\"" << std::endl;
	pFile << "" << std::endl;
	if(dc != NULL) {
		pFile << "    \"Fullscreen\" = \"" << (int)isFullScreen() << "\"" << std::endl;
	} else {
		pFile << "    \"Fullscreen\" = \"false\"" << std::endl;
	}
	pFile << "# glowing effects" << std::endl;
	pFile << "    \"Glowing buttons\" = \"" << (int)isGlowingButtons() << "\"" << std::endl;
	pFile << "# moving rectangles, 2 = all objects move smoothly, 1 = some objects move smoothly, 0 = all objects jump directly to their destination" << std::endl;
	pFile << "    \"Smooth movements\" = \"" << (int)isSmoothMovements() << "\"" << std::endl;
	pFile << "# Transparency, let's burn some cpu power (NOT YET IMPLEMENTED)" << std::endl;
	pFile << "    \"Transparency\" = \"" << (int)isTransparency() << "\"" << std::endl;
	pFile << "# Unload graphics if they are not needed, for low-memory systems" << std::endl;
	pFile << "    \"Unload graphics\" = \"" << (int)isUnloadGraphics() << "\"" << std::endl;
	pFile << "    \"Tooltips\" = \"" << (int)isToolTips() << "\"" << std::endl;
	pFile << "@END" << std::endl;

#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
	pFile << "# use music (1: on, 0: off)" << std::endl;	
	pFile << "    \"Music\" = \"" << (int)isMusic() << "\"" << std::endl;
	pFile << "# use sound (1: on, 0: off)" << std::endl;
	pFile << "    \"Sound effects\" = \"" << (int)isSound() << "\"" << std::endl;
	pFile << "# music volume (0% - 100%)" << std::endl;
	pFile << "    \"Music volume\" = \"" << (int)getMusicVolume() << "\"" << std::endl;
	pFile << "# sound volume (0% - 100%)" << std::endl;
	pFile << "    \"Sound volume\" = \"" << (int)getSoundVolume() << "\"" << std::endl;
	pFile << "# max number of simultaneously played sounds" << std::endl;
	pFile << "    \"Sound channels\" = \"" << (int)getSoundChannels() << "\"" << std::endl;
#endif
	pFile.close();
}

const bool GUI::setResolution(const Size& size) {
	return dc->setResolution(size);
}


// ------ EVENTS ------

void GUI::setMouse(const Point& p, SDL_Object* root_object)
{
	if((p == mouse) && (!UI_Button::isWasResetted())) {
		return;
	}

	UI_Button::setWasResetted(false);
	mouse = p;
	
	if(UI_Button::isCurrentButtonHasAlreadyLeft())
	{
		if(UI_Button::getCurrentButton() && UI_Button::getCurrentButton()->getAbsoluteRect().isTopLeftCornerInside(p))
		{
			UI_Button::getCurrentButton()->mouseHasEnteredArea();
			UI_Button::setCurrentButtonHasAlreadyLeft(false);
		}
		else {
			return;
		}
	}
	
	if(UI_Button::getCurrentButton() && !UI_Button::getCurrentButton()->getAbsoluteRect().isTopLeftCornerInside(p))
	{
		UI_Button::getCurrentButton()->mouseHasLeftArea();
		if(!UI_Button::isCurrentButtonPressed()) {
			UI_Button::resetButton();
		}
		else {
			UI_Button::setCurrentButtonHasAlreadyLeft();
		}
	}
	
// ignore mousemove if button is still pressed		
	if(UI_Button::getCurrentButton())
	{
		if(UI_Button::isCurrentButtonPressed())
		{
			UI_Button::getCurrentButton()->doHighlight();
//			UI_Button::getCurrentButton()->mouseHasMoved();
		}
		return;
	}
	if(!UI_Button::isMoveByMouse())
	{
//		if(SDL_Object::focus==NULL) // TODO
//		{
			SDL_Object::setHighlighted(root_object->checkHighlight(mouse)); // TODO
//		} //else
//		if((!temp_button)&&(UI_Object::editFieldActive()))
//			temp_button = UI_Object::getEditField()->checkHighlight());
		if(SDL_Object::getHighlighted() && SDL_Object::getHighlighted() != UI_Button::getCurrentButton())
		{
			UI_Button::resetButton();
			UI_Button::setCurrentButton( (UI_Button*)SDL_Object::getHighlighted());
			UI_Button::getCurrentButton()->mouseHasEnteredArea();
			UI_Button::setCurrentButtonHasAlreadyLeft(false);
		}

		if(isToolTips())
		{
// first we have to check the object with checkToolTip.
// The result is either the object itself or one of its children that owns a tooltip
			
			UI_Object* temp = toolTipParent;
			UI_Object* temp2 = NULL;
			toolTipParent = NULL;

			temp2 = ((UI_Object*)root_object)->checkToolTip(mouse);
			if(temp2 != NULL) {
				toolTipParent = temp2;
			}
// toolTipParent changed or tooltip has to be deleted?
			if(((toolTipParent != temp) || (tooltip == NULL)))
			{
				delete tooltip;
				if(!toolTipParent) {
					tooltip = NULL;
				}
				else 
				{	
//					if(toolTipParent->getToolTipEString()!=NULL_STRING)
//						tooltip = new UI_ToolTip(root_object, toolTipParent->getToolTipEString());
//					else
// TODO!					tooltip = new UI_ToolTip(root_object, toolTipParent->getToolTipString());
				}
			} else if(tooltip) {
				Point tp = mouse + Size(8, 8);
				if(tp.getX() + tooltip->getWidth() + 10 >= getMaxX()) {
					tp.setX(getMaxX() - tooltip->getWidth() - 10);
				}
				if(tp.getY() + tooltip->getHeight() + 10 >= getMaxY()) {
					tp.setY(getMaxY() - tooltip->getHeight() - 10);
				}
				tooltip->setPosition(tp);
			}

		}
	}

	if(!isToolTips() && tooltip)
	{
		delete tooltip;
		tooltip = NULL;
	}
}


void GUI::leftDown(const Point& mouse, SDL_Object* root_object)
{
	if(UI_Button::isWasResetted()) {
		return;
	}
	if(UI_Button::getCurrentButton() != NULL)
	{
		UI_Button::getCurrentButton()->mouseLeftButtonPressed(mouse);
		UI_Button::setCurrentButtonPressed();
	} else {
		SDL_Object::resetFocus();
	}
}

void GUI::leftUp(const Point& mouse, SDL_Object* root_object)
{
	if((UI_Button::getCurrentButton()!=NULL)&&(UI_Button::isCurrentButtonPressed())) {
		UI_Button::getCurrentButton()->mouseLeftButtonReleased(mouse);
	}
	UI_Button::setCurrentButtonPressed(false);
	UI_Button::setCurrentButtonHasAlreadyLeft(false);
	setMouse(mouse, root_object);

	if(UI_Button::isMoveByMouse()==true)
	{
		UI_Button::setMoveByMouse(false);
		UI_Button::setMouseMovePoint(Point(0, 0));
	}
	if(!UI_Button::getCurrentButton()) {
		SDL_Object::resetFocus();
	}

}

void GUI::rightDown(const Point& mouse, SDL_Object* root_object)
{
	if(UI_Button::isWasResetted()) {
		return;
	}
	if(UI_Button::getCurrentButton()!=NULL)
	{
		UI_Button::getCurrentButton()->mouseRightButtonPressed(mouse);
		UI_Button::setCurrentButtonPressed();
	}
}

void GUI::rightUp(const Point& mouse, SDL_Object* root_object)
{
	if((UI_Button::getCurrentButton()!=NULL)&&(UI_Button::isCurrentButtonPressed())) {
		UI_Button::getCurrentButton()->mouseRightButtonReleased(mouse);
	}
	UI_Button::setCurrentButtonPressed(false);
	UI_Button::setCurrentButtonHasAlreadyLeft(false);
	setMouse(mouse, root_object);
}

void GUI::wheelToTop()
{
	if(currentWindow) {
		currentWindow->moveScrollBarToTop();
	}
}

void GUI::wheelToBottom()
{
	if(currentWindow) {
		currentWindow->moveScrollBarToBottom();
	}
}

void GUI::wheelUp()
{
	if(currentWindow) {
		currentWindow->wheelUp();
	}
}

void GUI::wheelDown()
{
	if(currentWindow) {
		currentWindow->wheelDown();
	}
}

/*void GUI::setCursor(Cursor* cursor) {
	SDL_SetCursor((SDL_Cursor*)cursor);
}*/

// ------------------ SOUND AND MUSIC BEGIN -----------------------
#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)

#ifdef _FMOD_SOUND
const bool ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::ostringstream os;
		os << "FMOD error! ( "<< result << ") " << FMOD_ErrorString(result);
		toErrorLog(os.str());
		return false;
	}
	return true;
}
#endif







Music* GUI::loadMusic(const std::string file_name) const
{
#ifdef _FMOD_SOUND
	assert(soundEngine);
	FMOD::Sound* music_data;
	FMOD_RESULT result = soundEngine->createSound(file_name.c_str(), FMOD_LOOP_NORMAL | FMOD_SOFTWARE, 0, &music_data);
	if(!ERRCHECK(result)) {
		return NULL;
	}
//		throw MyException("ERROR (GUI::loadMusic()): Could not load " + file_name + ".");
//	}
#elif _SDL_MIXER_SOUND
	Mix_Music* music_data = Mix_LoadMUS(file_name.c_str());
	if(!music_data) 
	{
		return NULL;
//		std::ostringstream os;
//		os << "ERROR (GUI::loadMusic()): Could not load " << file_name << " : " << Mix_GetError();
//		throw MyException(os.str());
	}
#endif
	return new Music(music_data);
}

Sound* GUI::loadSound(const std::string file_name) const
{
#ifdef _FMOD_SOUND
	assert(soundEngine);
	FMOD::Sound* sound_data;
	FMOD_RESULT result = soundEngine->createSound(file_name.c_str(), FMOD_SOFTWARE, 0, &sound_data);
	if(!ERRCHECK(result)) {
		return NULL;
		//throw MyException("ERROR (GUI::loadSound()): Could not load " + file_name + ".");
	}
#elif _SDL_MIXER_SOUND
	Mix_Chunk* sound_data = Mix_LoadWAV(file_name.c_str());
	if(!sound_data) 
	{
		return NULL;
//		std::ostringstream os;
//		os << "ERROR (GUI::loadSound()): Could not load " << file_name << " : " << Mix_GetError();
//		throw MyException(os.str());
	}
#endif
	return new Sound(sound_data);
}




void GUI::releaseSoundEngine()
{
#ifdef _FMOD_SOUND
	if(musicChannel) {
		musicChannel->stop();
	}

	if(soundEngine) {
		toInitLog("* " + guiStrings.lookUpString("END_CLOSING_SOUND_ENGINE_STRING"));
		soundEngine->close();
		toInitLog("* " + guiStrings.lookUpString("END_RELEASING_SOUND_ENGINE_STRING"));
		soundEngine->release();
		delete soundEngine;
		soundEngine = NULL;
	}
#elif _SDL_MIXER_SOUND
	Mix_HaltMusic();
	toInitLog("* " + guiStrings.lookUpString("END_CLOSING_SOUND_ENGINE_STRING"));
	Mix_CloseAudio();
#endif
	soundInitialized = false;
}



void GUI::printSoundInformation() const
{
#ifdef _FMOD_SOUND
	FMOD_RESULT result;
	int driver_num;
	result = soundEngine->getNumDrivers(&driver_num);
	ERRCHECK(result);

	std::ostringstream os; os << "* Available sound drivers: ";
	for(unsigned int i = driver_num; i--;)
	{
		char driver_name[256];
		result = soundEngine->getDriverInfo(i, driver_name, 256, 0);
        ERRCHECK(result);
		os << driver_name << " ";
	}
	toInitLog(os.str());
	os.str("");
	int current_driver;
	result = soundEngine->getDriver(&current_driver);
	ERRCHECK(result);

	os << "* Driver used: ";
	if(current_driver == -1)
	{
		os << "Primary or main sound device as selected by the operating system settings";
		if(driver_num == 1)
		{
			char driver_name[256];
			result = soundEngine->getDriverInfo(current_driver, driver_name, 256, 0);
			ERRCHECK(result);
			os << "(probably '" << driver_name << "')";
		}
	}
	else
	{
		char driver_name[256];
		result = soundEngine->getDriverInfo(current_driver, driver_name, 256, 0);
		ERRCHECK(result);
		os << driver_name;
	}
	toInitLog(os.str());
#elif _SDL_MIXER_SOUND
	int audio_rate, audio_channels;
	Uint16 audio_format;
	int success = Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	assert(success);
/*#ifdef _SCC_DEBUG
	if(!success) {
		std::ostringstream os;
		os << "ERROR (GUI::printSoundInformation()): " << Mix_GetError();
		throw MyException(os.str());
	}
#endif*/
	int bits = audio_format & 0xFF;
	std::ostringstream os; 
	os << "* Opened audio at " << audio_rate << "Hz " << bits << "bit " << ((audio_channels>1)?"stereo":"mono");// << ", " << audio_buffers << " bytes audio buffer";
	toInitLog(os.str());
#endif
}

#ifdef _SDL_MIXER_SOUND
void transitionMusic(void)
{
	GUI::transitionMusic();
}

void soundChannelDone(int channel)
{
	GUI::soundChannelDone(channel);
}

// callback function
void GUI::transitionMusic(void)
{
	if(currentMusic != NULL)
	{
		Mix_HaltMusic(); // Just in case there was a misunderstanding, halt the music
		Mix_FreeMusic(currentMusic);
		currentMusic = nextMusic;
		nextMusic = NULL;
		if(currentMusic != NULL)
		{
			int success = Mix_FadeInMusic(currentMusic, -1, MUSIC_TRANSITION_DURATION/2);
			assert(success == 0);
/*#ifdef _SCC_DEBUG
			if(success != 0) {
				std::ostringstream os;
				os << "ERROR (GUI::playMusic()): " << Mix_GetError();
				throw MyException(os.str());
			}
#endif*/
		}
	}
}

void GUI::soundChannelDone(int channel)
{
	for(std::list<int>::iterator i = soundChannel.begin(); i != soundChannel.end();)
	{
		if(*i == channel) {
			i = soundChannel.erase(i);
		}
		else ++i;
	}
}
#endif


void GUI::initSoundEngine()
{
	// TODO start a 'watchdog' thread (FMOD waits indefinitely if the sound is currently used!)
	toInitLog(guiStrings.lookUpString("START_INIT_SOUND_STRING"));

#ifdef _FMOD_SOUND
	unsigned int version;
	
	if(!ERRCHECK(FMOD::System_Create(&soundEngine))) { 
		throw MyException(guiStrings.lookUpString("START_INIT_FMOD_SYSTEM_CREATE_ERROR_STRING"));
	}

	assert(soundEngine);
	if(!ERRCHECK(soundEngine->getVersion(&version))) {
		throw MyException(guiStrings.lookUpString("START_INIT_FMOD_GET_VERSION_ERROR_STRING"));
	}

	if (version < FMOD_VERSION)
    {
		std::ostringstream os;
		os << guiStrings.lookUpString("START_INIT_FMOD_VERSION_ERROR_STRING") << "[" << version << " < " << FMOD_VERSION << "]";
		throw MyException(os.str());
	}

	printSoundInformation();

	if(!ERRCHECK(soundEngine->init(32, FMOD_INIT_NORMAL, 0))) {
		throw MyException(guiStrings.lookUpString("START_INIT_FMOD_SYSTEM_INIT_ERROR_STRING"));
	}
#elif _SDL_MIXER_SOUND
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048)==-1) 
	{
		std::ostringstream os;
		os << "ERROR (GUI::initSoundEngine()): " << Mix_GetError();
		throw MyException(os.str());
	}		
	Mix_ChannelFinished(::soundChannelDone);
#endif
	soundInitialized = true;
}




void GUI::processSoundChannels()
{
#ifdef _FMOD_SOUND
	for(std::list<FMOD::Channel*>::iterator i = soundChannel.begin(); i!=soundChannel.end();)
	{
		bool is_playing = false;
		(*i)->isPlaying(&is_playing);
		if(!is_playing) {
			i = soundChannel.erase(i);
		}
		else ++i;
	}
	for(std::list<std::pair<Sound*, float> >::iterator i = soundsToPlay.begin(); i != soundsToPlay.end(); ++i)
		if((soundChannel.size() < getSoundChannels()))
		{
			FMOD::Channel* mychannel = NULL;
			soundEngine->playSound(FMOD_CHANNEL_FREE, (FMOD::Sound*)(i->first), 0, &mychannel);
			mychannel->setPan(i->second);
			mychannel->setVolume((float)(getSoundVolume()/100.0));
			soundChannel.push_back(mychannel);
		}
#elif _SDL_MIXER_SOUND
	for(std::list<std::pair<Sound*, float> >::iterator i = soundsToPlay.begin(); i != soundsToPlay.end(); ++i)
		if((soundChannel.size() < getSoundChannels()))
		{
			int mychannel = -1;
			mychannel = Mix_PlayChannel(-1, (Mix_Chunk*)(i->first), 0);
			if(mychannel == -1) {
				std::ostringstream os;
				os << "ERROR (GUI::processSoundChannels()): " << Mix_GetError();
				throw MyException(os.str());
			} else
			{
//				mychannel->setPan(i->second);
//				mychannel->setVolume((float)(getSoundVolume())/100.0);
				soundChannel.push_back(mychannel);
			}
		}
#endif
}



void GUI::stopMusic()
{
	playMusic(NULL);
}



void GUI::clearSoundChannels()
{
	soundChannel.clear();
}

void GUI::clearSoundsToPlay()
{
	soundsToPlay.clear();
}


void GUI::playSound(Sound* play_sound, const unsigned int x)
{
#ifdef _FMOD_SOUND
	assert(soundEngine);
#endif
	soundsToPlay.push_back(std::pair<Sound*, float>(play_sound, 2*((float)(2*x) - (float)getMaxX())/(float)(3*getMaxX())));
}



void GUI::playMusic(Music* play_music, const bool loop)
{

#ifdef _FMOD_SOUND
	assert(soundEngine);

	if(musicChannel) {
		musicChannel->stop();
	}
	soundEngine->playSound(FMOD_CHANNEL_FREE, (FMOD::Sound*)play_music, 0, &musicChannel);

#elif _SDL_MIXER_SOUND

	int success = -1;

	// music currently playing?
	if(currentMusic != NULL)
	{
		// new music?
		if(play_music != NULL) {
			if(loop) {
				nextMusic = currentMusic;
			} else {
				nextMusic = NULL;
			}
			// transition between both music plays
			Mix_HookMusicFinished(::transitionMusic);
			success = Mix_FadeOutMusic(MUSIC_TRANSITION_DURATION/2);
		} else 
			// empty music => Stop music, just fade out
		{
			success = Mix_FadeOutMusic(MUSIC_TRANSITION_DURATION);
		}
	} else 
	// no music was playing
	{
		if(play_music != NULL)
		{
			currentMusic = (Mix_Music*)play_music;
			if(loop) {
				nextMusic = currentMusic;
			} else {
				nextMusic = NULL;
			}

			Mix_HookMusicFinished(::transitionMusic);
			success = Mix_FadeInMusic(currentMusic, -1, MUSIC_TRANSITION_DURATION);
		} else {
			// no music played, no music to play
			success = 0;
		}
	}
	if(success != 0) {
		std::ostringstream os;
		os << "ERROR (GUI::playMusic()): " << Mix_GetError();
		toErrorLog(os.str());
		//throw MyException(os.str());
	}
#endif
}

std::list<int> GUI::soundChannel;
Mix_Music* GUI::currentMusic;
Mix_Music* GUI::nextMusic;
#endif

// ------------------ SOUND AND MUSIC END -----------------------


void GUI::processGUI() {
	if(!windowSelected) {
		currentWindow = NULL;
	}
	
	assert(dc);
	SDL_Object::processAll();
	SDL_Object::redrawnObjects = 0;
	SDL_Object::updateAreasToUpdate();
	SDL_Object::drawAll(dc);
	dc->updateScreen();
	// frameRateControl.updateConfiguration(); TODO
//inline void GUI::poll(const eTicks etick) {
//	frameRateControl.poll(etick);
//}

// ------ SOUND ENGINE -------
#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
		if((isSound() || isMusic()) && (!soundInitialized))
		{
			try {
				initSoundEngine();
			} catch(MyException e) {
				setSound(false);
				setMusic(false);
				soundInitialized = false;
				toInitLog(guiStrings.lookUpString("START_INIT_NOSOUND_STRING"));
			}
		} else if(!isSound() && !isMusic() && soundInitialized) {
			releaseSoundEngine();
		} else 
		if(!soundInitialized) {
			if(!isMusic()) {
				 stopMusic();
			}

			if(isSound()) {
				processSoundChannels();
			} else {
				clearSoundChannels();
			}
#ifdef _FMOD_SOUND
			soundEngine->update();
#endif
		}
		clearSoundsToPlay();
#endif		
//		m.poll(SOUND_TICKS); TODO
// ------ END SOUND ENGINE -------
}

GUI* GUI::gui = NULL;
std::string GUI::GUI_VERSION = "2.00";