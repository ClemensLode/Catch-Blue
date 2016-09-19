#include "ui_framerate.hpp"
#include <sstream>

UI_FrameRate::UI_FrameRate(const bool is_optimizing, const unsigned int desired_framerate, const unsigned int desired_cpu):
	fps(new FPS()),
	currentTicks(0), //lol, muss 0 sein
	framesPerSecond(10),
	totalTicks(500),
	tickIntervall(0),
	framesPerGeneration(0),
	refresh(0),
	generationsPerSecond(0)
{
	for(unsigned int i = MAX_TICK_TYPES;i--;) {
		for(unsigned int j = TICK_INTERVALL;j--;) {
			ticks[i][j] = 0;
		}
	}
	for(unsigned int j = TICK_INTERVALL;j--;) {
		frames_count[j]=0;
	}
	for(unsigned int i = MAX_TICK_TYPES; i--;) {
		percent[i] = 0;
	}

	setOptimizing(is_optimizing);
	setDesiredCPU(desired_cpu);
	setDesiredFramerate(desired_framerate);

	refresh = fps->getFramesPerGeneration();
}

UI_FrameRate::~UI_FrameRate()
{
	delete fps;
}


void UI_FrameRate::poll(const eTicks etick)
{
	ticks[etick][tickIntervall] = SDL_GetTicks() - currentTicks;
	currentTicks = SDL_GetTicks();
}

void UI_FrameRate::calculateFramerate()
{
	tickIntervall++;
	if(tickIntervall >= TICK_INTERVALL) {
		tickIntervall = 0;
	}
	
	totalTicks = 0;
	unsigned int total_this_ticks[MAX_TICK_TYPES];
	for(unsigned int i = MAX_TICK_TYPES;i--;)
	{
		total_this_ticks[i]=0;
		for(unsigned int j = TICK_INTERVALL;j--;) {
			total_this_ticks[i] += ticks[i][j];
		}
		totalTicks+=total_this_ticks[i];
		
	}
	if(totalTicks == 0) {
		totalTicks = 1;
	}
	unsigned int total_frames = 0;
	for(unsigned int j = TICK_INTERVALL;j--;) {
		total_frames += frames_count[j];
	}
	total_frames /= TICK_INTERVALL;

	if(framesPerSecond > total_frames) {
		framesPerSecond -= (framesPerSecond - total_frames)/5 + 1;
	} else {
		if(framesPerSecond < total_frames) {
			framesPerSecond += (total_frames - framesPerSecond)/5 + 1;
		}
	}
	if(framesPerSecond < 0) { 
		framesPerSecond = 0;
	}
		
	for(unsigned int i = MAX_TICK_TYPES;i--;)
	{
		if(percent[i] > total_this_ticks[i] * 100 / totalTicks) {
			percent[i] -= (percent[i] - total_this_ticks[i] * 100 / totalTicks)/5 + 1;
		} else {
			if(percent[i] < total_this_ticks[i] * 100 / totalTicks) {
				percent[i] += (total_this_ticks[i] * 100 / totalTicks - percent[i])/5 + 1;
			}
		}
		if(percent[i] < 0) {
			percent[i] = 0;
		}
		
		if(percent[i] > 100) {
			percent[i] = 100;
		}
	}
	
	framesPerSecond = 0;
	for(unsigned int j = TICK_INTERVALL;j--;) {
		framesPerSecond += frames_count[j];
	}
	framesPerSecond /= TICK_INTERVALL;
}

void UI_FrameRate::delay()
{
	calculateFramerate();
	framesPerGeneration = fps->getFramesPerGeneration();
	frames_count[tickIntervall] = fps->getCurrentFramerate();
	fps->setTotalTicks(1+totalTicks/TICK_INTERVALL);
	fps->delay();
	refresh += 100;
}

const bool UI_FrameRate::allowCalculation()
{
	bool allow = (refresh >= framesPerGeneration);
	if(framesPerGeneration > refresh) {
		refresh = 0;
	} else {
		refresh -= framesPerGeneration;
	}
	generationsPerSecond++;
	return(allow);
}


const std::list<unsigned int> UI_FrameRate::getPercentList() const
{
	std::list<unsigned int> t;
	for(unsigned int i = 0; i < MAX_TICK_TYPES; i++) {
		t.push_back(percent[i]);
	}
	return(t);
}



