#ifndef _SDL_MUSIC_HPP
#define _SDL_MUSIC_HPP

#if defined(_FMOD_SOUND) || defined(_SDL_MIXER_SOUND)

#ifdef _FMOD_SOUND
#include <fmod.hpp>
#elif _SDL_MIXER_SOUND
#include <SDL_mixer.h>
#endif

#include <resource.hpp>

class Music : public Resource
{
	public:
#ifdef _FMOD_SOUND
		Music(FMOD::Sound* t);
		operator FMOD::Sound*();
		(FMOD::Sound*) operator->();	
#elif _SDL_MIXER_SOUND
		Music(Mix_Music* t);
		operator Mix_Music*();
		(Mix_Music*) operator->();
#endif
	private:
#ifdef _FMOD_SOUND
		FMOD::Sound* music;
#elif _SDL_MIXER_SOUND
		Mix_Music* music;
#endif
};

#ifdef _FMOD_SOUND

	inline Music::Music(FMOD::Sound* t) {
		music = t;
	}

	inline Music::operator FMOD::Sound*() {
		return music;
	}

	inline FMOD::Sound* Music::operator->() {
		return music;
	}
#elif _SDL_MIXER_SOUND
	inline Music::Music(Mix_Music* t) {
		music = t;
	}

	inline Music::operator Mix_Music*() {
		return music;
	}

	inline Mix_Music* Music::operator->() {
		return music;
	}
#endif


#endif
#endif