/**
 * This class is responsible for drawing the whole grid
 * 
 * @author Clemens Lode, clemens at lode.de
 */

#include "raster.hpp"

Raster::Raster(const Size& raster_size, const Size& field_size, const Grid* grid) : 
	UI_Object(NULL, Rect(Point(0,0), raster_size), Size(), DO_NOT_ADJUST),
	fieldSize(field_size),
	grid(grid)
{
	makePufferInvalid();
	font = new Font("FreeSans.ttf", 10, false, true);
}

Raster::~Raster()
{
	delete font;
}

void Raster::draw(DC* dc) const
{
	assert(dc);
	dc->clearScreen();

	// draw the basic grid
	dc->setPen(Pen(Color(dc->getSurface(), 255, 255, 255)));
	int sizex = dc->getMaxX() / Configuration.getMaxX();
	int sizey = dc->getMaxY() / Configuration.getMaxY();
	for(unsigned int i = 0; i <= Configuration.getMaxY(); i++) {
		dc->DrawLine(Point(0, sizey * i), Point(dc->getMaxX()-1, sizey * i));
	}
	for(unsigned int i = 0; i <= Configuration.getMaxX(); i++) {
		dc->DrawLine(Point(sizex * i, 0), Point(sizex * i, dc->getMaxY()-1));
	}
	dc->setPen(Pen(Color(dc->getSurface(), 255, 255, 255), 1));
	for(unsigned int y = 0; y < Configuration.getMaxY(); y++) {
		for(unsigned int x = 0; x < Configuration.getMaxX(); x++) {
			dc->DrawEmptyRectangle(Rect(x*sizex + 2, y*sizey + 2, sizex - 4, sizey - 4));
		}
	}

	dc->setFont(*font);
	dc->setTextForeground(Color(dc->getSurface(), 255, 255, 255));

	const std::vector<std::vector<Field> >& field_grid = grid->getFieldGrid();
	for(unsigned int x = 0; x < field_grid.size(); x++) {
		for(unsigned int y = 0; y < field_grid[x].size(); y++) {
			// determine the color for sight and hearing ranges
			unsigned int intensity_red_hearing = 0;
			unsigned int intensity_blue_hearing = 0;
			unsigned int intensity_red_seeing = 0;
			unsigned int intensity_blue_seeing = 0;

			const Field& f = field_grid[x][y];

			if(f.isSeen()) {
				for(std::vector<const BaseAgent*>::const_iterator i = f.getSeenBy().begin(); i != f.getSeenBy().end(); i++) {
					if(!(*i)->isAlive()) {
						continue;
					}
					if((*i)->getColor() == RED_AGENT) {
						intensity_red_seeing++;
					} else {
						intensity_blue_seeing++;
					}
				}
			}
			if(f.isHeard()) {
				for(std::vector<const BaseAgent*>::const_iterator i = f.getHeardBy().begin(); i != f.getHeardBy().end(); i++) {
					if(!(*i)->isAlive()) {
						continue;
					}
					if((*i)->getColor() == RED_AGENT) {
						intensity_red_hearing++;
					} else {
						intensity_blue_hearing++;
					}
				}
			}
			unsigned int max = intensity_red_hearing;
			if(intensity_blue_hearing > max) {
				max = intensity_blue_hearing;
			}
			if(intensity_red_seeing > max) {
				max = intensity_red_seeing;
			}
			if(intensity_blue_seeing > max) {
				max = intensity_blue_seeing;
			}
			if(max > 0) {
				intensity_red_hearing *= 100/max;
				intensity_blue_hearing *= 100/max;
				intensity_red_seeing *= 100/max;
				intensity_blue_seeing *= 100/max;
			}

			// draw the sight and hearing ranges
			dc->setBrush(Brush(Color(dc->getSurface(), intensity_red_hearing + intensity_red_seeing, 
				intensity_red_hearing + intensity_blue_hearing, 
				intensity_blue_hearing + intensity_blue_seeing)));
			dc->DrawRectangle(Rect(x * sizex + 4, y * sizey + 4, sizex - 8, sizey - 8));

			// draw obstacle
			if(!f.getAgent() && f.getContent() == OBSTACLE) {
				dc->setBrush(Brush(Color(dc->getSurface(), 255, 255, 255)));
				dc->DrawRectangle(Rect(x * sizex + 10, y * sizey + 10, sizex - 20, sizey - 20));
			} else if(f.getAgent()) {
				// draw red and blue agents
				if(f.getAgent()->getColor() == RED_AGENT) {
					dc->setBrush(Brush(Color(dc->getSurface(), 255, 
						f.getAgent()->hasMoved()*100, 
						f.getAgent()->hasMoved()*100)));
				} else {
					dc->setBrush(Brush(Color(dc->getSurface(), f.getAgent()->hasMoved()*100, f.getAgent()->hasMoved()*100, 255)));
				}
				dc->DrawRectangle(Rect(x * sizex + 10, y * sizey + 15, sizex - 20, sizey - 20));
				std::string action_string;
				switch(f.getAgent()->getState()) {
					case RED_AGENT_SEARCH_BLUE_AGENT:action_string = "search";break;
					case RED_AGENT_HUNT_BLUE_AGENT:action_string = "hunte";break;
					case RED_AGENT_ELIMINATE_BLUE_AGENT:action_string = "eliminate";break;
					case BLUE_AGENT_FLEE_FROM_RED_AGENT:action_string = "flee";break;
					case BLUE_AGENT_STAY_IN_HIDING:action_string = "hide";break;
					case BLUE_AGENT_HIDE_IN_HIDING_PLACE:action_string = "cover";break;
					case BLUE_AGENT_SEARCH_HIDING_PLACE:action_string = "search";break;
				}
				dc->DrawText(action_string, Point(x * sizex+5, y * sizey+5));
			}
		}
	}

// draw the paths
	for(unsigned int x = 0; x < field_grid.size(); x++) {
		for(unsigned int y = 0; y < field_grid[x].size(); y++) {
			for(std::list<AgePath>::const_iterator i = field_grid[x][y].getAgePathList().begin(); i != field_grid[x][y].getAgePathList().end(); i++) {
				unsigned int brightness = i->age * 255 / Configuration.getMaxPathAge();
				unsigned int thickness = i->age * 4 / Configuration.getMaxPathAge();
				dc->setPen(Pen(Color(dc->getSurface(), i->color == RED_AGENT?brightness:0, 
													   brightness/4, 
													   i->color == BLUE_AGENT?brightness:0), thickness));
				dc->DrawLine(Point(x * sizex + sizex / 2, y * sizey + sizey / 2), 
					         Point(i->p.x * sizex + sizex / 2, i->p.y * sizey + sizey / 2));
			}
		}
	}

	UI_Object::draw(dc);
}


