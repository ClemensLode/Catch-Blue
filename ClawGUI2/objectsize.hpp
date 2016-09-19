#ifndef _OBJECTSIZE_HPP
#define _OBJECTSIZE_HPP

#include <size.hpp>
#include <resource.hpp>

class ObjectSize : public Resource
{
	public:
		ObjectSize(Uint16 width, Uint16 height);
		~ObjectSize();
		operator Size() const;
		Size operator->() const;

		const Uint16 getWidth() const {return size.getWidth();}
		const Uint16 getHeight() const {return size.getHeight();}
	private:
		Size size;
};

inline ObjectSize::operator Size() const { 
	return size;
}
		
inline Size ObjectSize::operator->() const {
	return size;
}

#endif