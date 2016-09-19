#include "objectsize.hpp"

ObjectSize::ObjectSize(Uint16 width, Uint16 height):
	Resource(),
	size(Size(width, height))
{
}

ObjectSize::~ObjectSize()
{}