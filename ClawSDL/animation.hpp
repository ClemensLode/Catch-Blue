#ifdef _SMPEG

#ifndef _SDL_ANIMATION_HPP
#define _SDL_ANIMATION_HPP

#include <smpeg.h>	
#include <string>
#include "resource.hpp"

class Animation : public Resource
{
public:
	operator SMPEG*() const;
	SMPEG* operator->() const;
	SMPEG* getSurface() const { return animation;}
	const SMPEG_Info& getInfo() const;
	const std::string& getName() const;

private:
	std::string name;

	SMPEG* animation;
	SMPEG_Info info;

	bool used;
};

inline const SMPEG_Info& Animation::getInfo() const {
	return info;
}

inline const std::string& Animation::getName() const {
	return name;
}

inline Animation::operator SMPEG*() const { 
	return animation;
}
		
inline SMPEG* Animation::operator->() const {
	return animation;
}


#endif // _ANIMATION_HPP

#endif