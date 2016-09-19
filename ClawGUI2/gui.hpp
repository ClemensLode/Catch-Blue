#ifndef _GUI_GUI_HPP
#define _GUI_GUI_HPP

#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
#include <sound.hpp>
#include <music.hpp>
#endif 

#include <misc.hpp>
#include <dc.hpp>
#include <set>
#include <math.h>
#include <SDL_Object.hpp>
#include <languagehandler.hpp>
#include "ui_tooltip.hpp"
#include "ui_window.hpp"
#include "ui_object.hpp"

#define MIN_DISTANCE 3
#define MAX_COMPARE_GAMES 2 // two per page at maximum
#define MAX_PLAYER 1

#define GUI_API
/*#ifdef GUI_EXPORTS
    #define GUI_API __declspec(dllexport)
#else
    #define GUI_API __declspec(dllimport)
#endif*/

class GUI_API GUI {
	public:
		static GUI* createInstance(const std::list<std::string>& parameter_list, const std::string& window_title);
		static void deleteInstance();
		void initDefaults();
		const bool isLanguageChosen() const;

		void setConfigurationFile(const std::string& configuration_file);

		void processGUI();

// get/set commands:

		const unsigned int getDesiredFramerate() const;
		const unsigned int getDesiredCPU() const;
		const bool setDesiredFramerate(const unsigned int desired_frame_rate);
		const bool setDesiredCPU(const unsigned int cpu_usage);

		static const unsigned int MIN_CPU_USAGE = 1;
		static const unsigned int MAX_CPU_USAGE = 99;
		static const unsigned int MIN_DESIRED_FRAMERATE = 1;
		static const unsigned int MAX_DESIRED_FRAMERATE = 1000;

		const bool isFullScreen() const;
		const bool isGlowingButtons() const;
		const bool isTransparency() const;
		const bool isSmoothMovements() const;
		const bool isUnloadGraphics() const;
		const bool isToolTips() const;
		const eBitDepth getBitDepth() const;
		const bool setBitDepth(const eBitDepth bitDepth);

		const std::string& getLanguage() const;
		const bool setLanguage(const std::string& language);

		const bool setResolution(const Size& size);
		const Size getResolution() const;

		const bool setFullScreen(const bool full_screen);
		const bool setGlowingButtons(const bool glowing_buttons);
		const bool setTransparency(const bool trans_parency);
		const bool setSmoothMovements(const bool smooth_movements);
		const bool setUnloadGraphics(const bool unload_graphics);
		const bool setToolTips(const bool tool_tips);
		



		const bool isMouseInside(const Rect& rect) const;
		void setMouse(const Point& p, SDL_Object* root_object);
		const Point& getMouse() const;

		void leftDown(const Point& mouse, SDL_Object* root_object);
		void leftUp(const Point& mouse, SDL_Object* root_object);
		void rightDown(const Point& mouse, SDL_Object* root_object);
		void rightUp(const Point& mouse, SDL_Object* root_object);

		void wheelUp();
		void wheelDown();

		void wheelToTop();
		void wheelToBottom();
	
		const Uint16 getMaxX() const;
		const Uint16 getMaxY() const;

		const bool isWindowSelected() const;
		UI_Window* getCurrentWindow() const; 
		void setWindowSelected(const bool selected = true);
		void setCurrentWindow(UI_Window* window);
		void resetWindow();	

		SDL_PixelFormat* getCurrentPixelFormat() const;

		static const std::string getVersion();

		void setToolTip(UI_ToolTip* tooltip);
		UI_ToolTip* getToolTip() const;
		void setToolTipParent(UI_Object* parent);
		UI_Object* getToolTipParent() const;

//		void setCursor(Cursor* cursor);


		void addMessage(const std::string& msg);
		const std::list<std::string> getRemainingMessages() const;
		void clearMessages();

		const Text& lookUpText(const std::string& text_id) const;
		const std::string& lookUpString(const std::string& text_id) const;


// ------------------ SOUND AND MUSIC START -----------------------
#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)
		const bool isMusic() const;
		const bool isSound() const;
		const bool setMusic(const bool use_music);
		const bool setSound(const bool use_sound);

		const unsigned int getMusicVolume() const;
		const unsigned int getSoundVolume() const;
		const bool setMusicVolume(const unsigned int music_volume);
		const bool setSoundVolume(const unsigned int sound_volume);

		void playSound(Sound* play_sound, const unsigned int x);
		void playMusic(Music* play_music, const bool loop = true);
		Sound* loadSound(const std::string file_name) const;
		Music* loadMusic(const std::string file_name) const;

		void stopMusic();

		void releaseSoundEngine();

#ifdef _SDL_MIXER_SOUND
		static void transitionMusic(void);
		static void soundChannelDone(int channel);
#endif

#endif
// ------------------ SOUND AND MUSIC END -----------------------

	private:
		GUI(const std::list<std::string>& parameter_list, const std::string& window_title);
	    ~GUI();
		static GUI* gui;
		void saveToFile() const;
		void loadConfigurationFile();
		void loadConfigurationFile(Size& res_size, eBitDepth& bit_depth, bool& full_screen);

		std::string printHardwareInformation();
		std::string printSurfaceInformation(DC* surface);

		LanguageHandler guiStrings;

		std::string configurationFile;

		DC* dc;
		SDL_Cursor* defaultCursor;
		static std::string GUI_VERSION;
		std::list<std::string> remainingMessages;

		UI_ToolTip* tooltip;
		UI_Object* toolTipParent;
		
		UI_Window* currentWindow;
		bool windowSelected;

		// TODO currentButton


		unsigned int mouseType;
		Point mouse;

		unsigned int desiredFramerate; // X Frames per generation
		unsigned int desiredCPU; // hold X FPS

		bool glowingButtons;
		bool transparency;
		bool unloadGraphics;
		bool toolTips;
		
	

//		const std::string lookUpFormattedString(const eString id, const std::string& text) const;
//		const std::string lookUpFormattedString(const eString id, const unsigned int i) const;
//		const std::string lookUpFormattedString(const eString id, const unsigned int i, const unsigned int j) const;
//		const std::string lookUpFormattedString(const eString id, const unsigned int i, const unsigned int j, const unsigned int k) const;


// ------------------ SOUND AND MUSIC BEGIN -----------------------
#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)

		void initSoundEngine();

		void printSoundInformation() const;
		void clearSoundsToPlay();
		void clearSoundChannels();
		void processSoundChannels();
		const unsigned int getSoundChannels() const;
		const bool setSoundChannels(const unsigned int channel_num);

		unsigned int musicVolume;
		unsigned int soundVolume;

		unsigned int soundChannels;
	
		bool soundInitialized;
		bool useMusic;
		bool useSound;
		std::list<std::pair<Sound*, float> > soundsToPlay;

#ifdef _FMOD_SOUND
		FMOD::Channel* musicChannel;
		FMOD::System* soundEngine;
		std::list<FMOD::Channel*> soundChannel;
#elif _SDL_MIXER_SOUND
		// need to be static because of callback functions!
		static Mix_Music* currentMusic;
		static Mix_Music* nextMusic;
		static std::list<int> soundChannel;
#endif
#endif
// ------------------ SOUND AND MUSIC END -----------------------
			
};

inline const Size GUI::getResolution() const {
	assert(dc);
	return dc->getSize();
}

inline const std::string& GUI::getLanguage() const {
	return guiStrings.getLanguage();
}

inline const bool GUI::setLanguage(const std::string& language) {
	return guiStrings.setLanguage(language);
}


inline const eBitDepth GUI::getBitDepth() const {
	assert(dc);
	return(dc->getBitDepth());
}

inline const bool GUI::setBitDepth(const eBitDepth bitDepth)
{
	assert(dc);
	return dc->setBitDepth(bitDepth);
}




inline const std::list<std::string> GUI::getRemainingMessages() const {
	return remainingMessages;
}

inline void GUI::clearMessages() {
	remainingMessages.clear();
}

inline const bool GUI::isToolTips() const {
	return(toolTips);
}

inline void GUI::setToolTip(UI_ToolTip* tooltip) {
	this->tooltip = tooltip;
}

inline UI_ToolTip* GUI::getToolTip() const {
	return this->tooltip;
}

inline void GUI::setToolTipParent(UI_Object* parent) {
	this->toolTipParent = parent;
}

inline UI_Object* GUI::getToolTipParent() const {
	return toolTipParent;
}


inline const std::string GUI::getVersion() {
	return GUI_VERSION;
}

inline SDL_PixelFormat* GUI::getCurrentPixelFormat() const {
	assert(dc);
	assert(dc->getSurface());
	return dc->getSurface()->format;
}

inline const bool GUI::isWindowSelected() const {
	return this->windowSelected;
}

inline UI_Window* GUI::getCurrentWindow() const {
	return this->currentWindow;
}

inline void GUI::setWindowSelected(const bool selected) {
	windowSelected = selected;
}

inline void GUI::setCurrentWindow(UI_Window* window) {
	currentWindow = window;
}

inline const Point& GUI::getMouse() const {
	return mouse;
}

inline const bool GUI::isMouseInside(const Rect& rect) const {
	return rect.isTopLeftCornerInside(mouse);
}

inline const Uint16 GUI::getMaxX() const {
	assert(dc);
	return dc->getMaxX();
}

inline const Uint16 GUI::getMaxY() const {
	assert(dc);
	return dc->getMaxY();
}




inline const unsigned int GUI::getDesiredFramerate() const
{
	assert(desiredFramerate >= MIN_DESIRED_FRAMERATE && desiredFramerate <= MAX_DESIRED_FRAMERATE);

	return desiredFramerate;
}

inline const unsigned int GUI::getDesiredCPU() const
{
	assert(desiredCPU >= MIN_CPU_USAGE && desiredCPU <= MAX_CPU_USAGE);

	return desiredCPU;
}


inline const bool GUI::isFullScreen() const {
	assert(dc);
	return(dc->isFullScreen());
}

inline const bool GUI::isGlowingButtons() const {
	return(glowingButtons);
}

inline const bool GUI::isTransparency() const {
	return(transparency);
}

inline const bool GUI::isUnloadGraphics() const {
	return(unloadGraphics);
}

inline const bool GUI::isSmoothMovements() const {
	return(SDL_Object::smoothMovements);
}

inline const bool GUI::setFullScreen(const bool full_screen) 
{
	return dc->setFullScreen(full_screen);
}

inline const bool GUI::setGlowingButtons(const bool glowing_buttons) 
{
	if(glowingButtons == glowing_buttons) {
		return(false);
	}
	glowingButtons = glowing_buttons;
	return(true);
}

inline const bool GUI::setTransparency(const bool trans_parency) 
{
	if(transparency == trans_parency) {
		return(false);
	}
	transparency = trans_parency;
	return(true);
}

inline const bool GUI::setSmoothMovements(const bool smooth_movements) 
{
	if(SDL_Object::smoothMovements == smooth_movements) {
		return(false);
	}
	SDL_Object::smoothMovements = smooth_movements;
	return(true);
}

inline const bool GUI::setUnloadGraphics(const bool unload_graphics) 
{
	if(unloadGraphics == unload_graphics) {
		return(false);
	}
	unloadGraphics = unload_graphics;
	return(true);
}

inline const bool GUI::setToolTips(const bool tool_tips) 
{
	if(toolTips == tool_tips) {
		return(false);
	}
	toolTips = tool_tips;
	return(true);
}

inline const bool GUI::setDesiredCPU(const unsigned int desired_cpu_usage)
{
	if(desiredCPU == desired_cpu_usage) {
		return(false);
	}
	assert(desired_cpu_usage >= 1 && desired_cpu_usage <= 100);

	desiredCPU = desired_cpu_usage;
	return(true);
}

inline const bool GUI::setDesiredFramerate(const unsigned int desired_frame_rate)
{
	if(desiredFramerate == desired_frame_rate) {
		return(false);
	}
	assert(desired_frame_rate >= MIN_DESIRED_FRAMERATE && desired_frame_rate <= MAX_DESIRED_FRAMERATE);

	desiredFramerate = desired_frame_rate;
	return(true);
}




#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)

inline const unsigned int GUI::getSoundChannels() const {
	return(soundChannels);
}

inline const unsigned int GUI::getMusicVolume() const {
	return(musicVolume);
}

inline const unsigned int GUI::getSoundVolume() const {
	return(soundVolume);
}


inline const bool GUI::isMusic() const {
	return(useMusic);
}

inline const bool GUI::isSound() const {
	return(useSound);
}

inline const bool GUI::setMusic(const bool use_music) 
{
	if(useMusic == use_music) {
		return(false);
	}
	useMusic = use_music;
	return(true);
}

inline const bool GUI::setSound(const bool use_sound) 
{
	if(useSound == use_sound) {
		return(false);
	}
	useSound = use_sound;
	return(true);
}

inline const bool GUI::setMusicVolume(const unsigned int music_volume)
{
	if(musicVolume == music_volume) {
		return(false);
	}
	musicVolume = music_volume;
	return(true);
}

inline const bool GUI::setSoundVolume(const unsigned int sound_volume) 
{
	if(soundVolume == sound_volume) {
		return(false);
	}
	soundVolume = sound_volume;
	return(true);
}

inline const bool GUI::setSoundChannels(const unsigned int channel_num)
{
	if(soundChannels == channel_num) {
		return(false);
	}
	soundChannels = channel_num;
	return(true);
}
#endif


#endif // _GUI_GUI_HPP



