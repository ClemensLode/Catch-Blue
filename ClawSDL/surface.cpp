#include "surface.hpp"

Surface::Surface(const SDL_Surface* surface):
	surface(surface)
{}


Surface::Surface():
	surface(NULL) 
{}

Surface::~Surface() {
}