/**
 * Main class provides parameters, initializes the GUI and contains the main loop
 *
 * @author Clemens Lode, clemens at lode.de
 */

#include <gui.hpp>
#include <sstream>
#include <ctime>

#include "raster.hpp"

int main(int argc, char *argv[])
{
	// initialize random number generator
	clock_t start = clock();
	srand(start);

	// parse arguments
	std::list<std::string> parameter_list;
	std::string chosen_driver;
	for(signed int i = 1;i<argc;++i) {
		parameter_list.push_back(argv[i]);
	}

// ------- INIT GRAPHIC ENGINE ------
	std::string window_title = "Eliminiert die Blauen! (ClawGUI: v" + GUI::getVersion() + ", ClawSDL: v" + DC::getVersion() + ") - Clemens Lode";
	GUI* gui = GUI::createInstance(parameter_list, window_title);
	if(!gui) {
		return(EXIT_FAILURE);
	}
	toInitLog("* " + gui->lookUpString("START_INIT_GRAPHIC_ENGINE_CORE_STRING"));
	toInitLog(gui->lookUpString("START_COMMANDO_LINE_1_STRING"));
	toInitLog(gui->lookUpString("START_COMMANDO_LINE_2_STRING"));
	toInitLog(gui->lookUpString("START_SYSTEM_READY_STRING"));

// ------ END INIT GRAPHIC ENGINE ------



	bool done = false;
	SDL_Event event;

	bool pause = true;
	bool step = false;
	unsigned int tick = 1;
	Grid* grid = new Grid();
	grid->resetState();

	Size fieldSize(gui->getMaxX() / Configuration.getMaxX(),
				   gui->getMaxY() / Configuration.getMaxY());
	Raster* raster = new Raster(Size(gui->getMaxX(), gui->getMaxY()), fieldSize, grid);
	

// MAIN LOOP
	while(!done)
	{
		bool reset = false;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					done = true;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_F1:pause = true;break;
						case SDLK_F2:pause = false;break;
						case SDLK_F3:break;
						case SDLK_F4:reset = true;break;
						case SDLK_SPACE:
							step = true;break;
						case SDLK_KP_ENTER:
						case SDLK_RETURN:
							if(event.key.keysym.mod & (KMOD_LALT | KMOD_RALT | KMOD_ALT)) {
								gui->setFullScreen(true);
								toInitLog(gui->lookUpString(gui->isFullScreen()?"START_SET_FULLSCREEN_MODE_STRING":"START_SET_WINDOW_MODE_STRING"));
							}
						break;
						case SDLK_ESCAPE:
							done = true;
							break;
						default:break;
					}
					break;
				default:break;
			}
		}

		if(reset) {
			grid->resetState();
			raster->redrawWholeObject();
		}
// ------ DRAWING AND PROCESSING ------
		if(step || reset || ((tick>Configuration.getSimulationSpeed()) && (!pause))) {
			// process agents by one step
			grid->doStep();
			step = false;
			tick = 0;
			raster->redrawWholeObject();
		}
		// draw on screen
		gui->processGUI();
		tick++;

/// ------ END DRAWING AND PROCESSING ------
	}

	toInitLog("* " + gui->lookUpString("END_CLOSING_SDL_STRING"));

	delete grid;
	delete raster;
	GUI::deleteInstance();

	return(EXIT_SUCCESS);
}				

