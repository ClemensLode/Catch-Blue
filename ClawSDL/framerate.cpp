#include "framerate.hpp"
#include <iostream>
#include <misc.hpp>
#include <assert.h>

FPS::FPS():
	desiredFramerate(FPS_DEFAULT),
	currentFramerate(FPS_DEFAULT),
	desiredCPU(100),
	framesPerGeneration(100),
	framecount(0),
	rateTicks(1000.0 / FPS_DEFAULT),
	startTicks(SDL_GetTicks()),
	lastTicks(SDL_GetTicks()),
	totalTicks(400),
	delayTicks(100),
	adaptFramesPerGeneration(true),
	averagecounter(0)
{
	for(unsigned int i = FPS_AVERAGE_VALUES; i--;)
		average[i] = 20;
}

FPS::~FPS()
{ }

void FPS::setTotalTicks(const long unsigned int total_ticks) 
{
#ifdef _SCC_DEBUG
	if(total_ticks > FPS_MAX_TOTAL_TICKS || total_ticks < FPS_MIN_TOTAL_TICKS) {
		throw MyWarning("WARNING (FPS::setTotalTicks()): Exceptional short or long delay, total_ticks out of range.");
	}
#endif
	if(total_ticks > FPS_MAX_TOTAL_TICKS) {
		totalTicks = FPS_MAX_TOTAL_TICKS;
	} else if(total_ticks < FPS_MIN_TOTAL_TICKS) {
		totalTicks = FPS_MIN_TOTAL_TICKS;
	} else {
		totalTicks = total_ticks;
	}
}


const bool FPS::setDesiredFramerate(const unsigned int desired_frame_rate)
{
	if(desiredFramerate == desired_frame_rate)
		return(false);
#ifdef _SCC_DEBUG
	if(desired_frame_rate > FPS_UPPER_LIMIT || desired_frame_rate < FPS_LOWER_LIMIT) {
		throw MyWarning("WARNING (FPS::setDesiredFramerate()): Desired Framerate usage value out of range.");
	}
#endif
	if(desired_frame_rate > FPS_UPPER_LIMIT) {
		desiredFramerate = FPS_UPPER_LIMIT;
	} else if(desired_frame_rate < FPS_LOWER_LIMIT) {
		desiredFramerate = FPS_LOWER_LIMIT;
	} else {
		desiredFramerate = desired_frame_rate;
	}

	framecount = 0;
	rateTicks = (1000.0 / desiredFramerate);
	return(true);
}

const bool FPS::setDesiredCPU(const unsigned int desired_cpu_usage)
{
	if(desiredCPU == desired_cpu_usage) {
		return(false);
	}
#ifdef _SCC_DEBUG
	if((desired_cpu_usage<1)||(desired_cpu_usage>100)) {
		throw MyWarning("WARNING (FPS::setDesiredCPU()): Desired CPU usage value out of range.");
	}
#endif

	if(desired_cpu_usage < 1) {
		desiredCPU = 1;
	} else if(desired_cpu_usage > 100) {
		desiredCPU = 100;
	} else {
		desiredCPU = desired_cpu_usage;
	}
	return(true);
}

void FPS::delay()
{
// Increase/Reduce the frames per generation
	long int difference = SDL_GetTicks() - startTicks;
	assert(difference >= 0);
	if(difference < 0) {
		startTicks = SDL_GetTicks();
		difference = 1;
	} else if(difference == 0) {
		difference = 1;
		startTicks ++;
	} else {
   		startTicks += difference;
	}

	if(averagecounter < 2) {
   		++averagecounter;
	}
	for(unsigned int i = averagecounter; i--;) {
   		average[i+1] = average[i];
	}
	average[0] = difference;
	long int av = 1;
	for(unsigned int i = averagecounter; i--;) {
   		av += average[i];
	}
	currentFramerate = 1000 * averagecounter / av;
	long unsigned int cpu_delay = (totalTicks - desiredCPU * totalTicks / 100);

	if(adaptFramesPerGeneration)
	{
		if((currentFramerate > desiredFramerate) && (framesPerGeneration > 1))
			--framesPerGeneration;
		else 
		if((currentFramerate < desiredFramerate) && (framesPerGeneration < 1000)) //100?
			++framesPerGeneration;
	} else if(currentFramerate > desiredFramerate)
	{
        Uint32 current_ticks;
		Uint32 target_ticks;
        Uint32 the_delay = 0;

        framecount++;
        current_ticks = SDL_GetTicks();
        target_ticks = lastTicks + (Uint32) (framecount * rateTicks);

        if (current_ticks <= target_ticks) {
			the_delay = target_ticks - current_ticks;
			SDL_Delay(the_delay);
        } else {
			framecount = 0;
            lastTicks = SDL_GetTicks();
		}
	}
	if((delayTicks < cpu_delay) && (delayTicks < 1000)) {
		delayTicks+=(cpu_delay - delayTicks)/5 + 1;
	}
	else if((delayTicks > cpu_delay)) {
		delayTicks -= (delayTicks - cpu_delay)/2 + 1; // reduce delayTicks very fast because smooth drawing has priority over processor usage!
	}
	SDL_Delay(delayTicks);
}

