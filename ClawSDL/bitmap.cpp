#include "bitmap.hpp"

Bitmap::Bitmap(const std::string& file_name) {
	bitmap = IMG_Load(file_name.c_str());
	if(!bitmap) {
		throw MyException("ERROR (Bitmap::Bitmap()): Loading file " + file_name + " (" + IMG_GetError() + ").");
	}
	name = file_name;
}

Bitmap::~Bitmap() {
	if(bitmap) {
		SDL_FreeSurface(bitmap);
		bitmap = NULL;
	}
	Resource::~Resource();
}
