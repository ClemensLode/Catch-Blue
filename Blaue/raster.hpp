#ifndef _BLAUE_RASTER_HPP
#define _BLAUE_RASTER_HPP

/**
 * This class is responsible for drawing the whole grid
 * 
 * @author Clemens Lode, clemens at lode.de
 */

#include <ui_object.hpp>

#include "grid.hpp"

class Raster : public UI_Object
{
	public:
		Raster(const Size& raster_size, const Size& field_size, const Grid* grid);
		~Raster();

		void draw(DC* dc) const;

	private:
		Size fieldSize;
		const Grid* grid;
		Font* font;
};

#endif

