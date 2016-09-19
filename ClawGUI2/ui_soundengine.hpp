#ifndef _UI_SOUNDENGINE_HPP
#define _UI_SOUNDENGINE_HPP

#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)

#include "resourcehandler.hpp"

//TODO sound identifier
/*enum eSound
{
	NULL_SOUND,
	MOUSEOVER_SOUND,
	SWISHIN_SOUND,
	SWISHOUT_SOUND,
	SWISHLOCK_SOUND,
	CLICKED_SOUND,
	CLICK_SOUND,
	COMPLETE_SOUND,
	ERROR_SOUND,
	RING_SOUND,
	INTRO_SOUND,
	MAX_SOUNDS
};

enum eMusic
{
	NULL_MUSIC,
	LALA_MUSIC,
	DIABLO_MUSIC,
	MAX_MUSIC
};
*/


#include <string>
#include <list>
#include <sound.hpp>
#include <music.hpp>

class UI_Sound
{
	public:
		UI_Sound();
		~UI_Sound();
		
		const bool init();
		
		const bool loadSoundDataFile(const std::string& sound_dir);
#ifdef _FMOD_SOUND
		FMOD::Sound* lookUpSound(const eSound id);
		FMOD::Sound* lookUpMusic(const eMusic id);
		const bool ERRCHECK(FMOD_RESULT result) const;
#elif _SDL_MIXER_SOUND
		Mix_Chunk* lookUpSound(const eSound id);
		Mix_Music* lookUpMusic(const eMusic id);
#endif
		void printSoundInformation() const;
		void playSound(const eSound id, const unsigned int x);
		void playMusic(const eMusic id, const bool loop = true);
		
		void releaseSoundEngine();

		void clearChannels();
		void clearSoundsToPlay();
		void update();
		void processChannels();
		void stopMusic();
		static void transitionMusic(void);
	private:
#ifdef _FMOD_SOUND
		FMOD::Channel* musicChannel;
		std::list<std::pair<FMOD::Sound*, float> > soundsToPlay;
		FMOD::System* sound;
		FMOD::Sound* soundList[MAX_SOUNDS];
		FMOD::Sound* musicList[MAX_MUSIC];
		std::list<FMOD::Channel*> soundChannel;
#elif _SDL_MIXER_SOUND
		static Mix_Music* music;
		static Mix_Music* nextMusic;
		std::list<std::pair<Mix_Chunk*, float> > soundsToPlay;
		Mix_Chunk* soundList[MAX_SOUNDS];
		Mix_Music* musicList[MAX_MUSIC];
		static std::list<int> soundChannel;
		static void channelDone(int channel);
#endif
		static eMusic currentMusic;
		
		
		ResourceHandler sounds;
		ResourceHandler music;
	
		std::string configurationFile;
};

#endif
#endif // _UI_SOUND_HPP

