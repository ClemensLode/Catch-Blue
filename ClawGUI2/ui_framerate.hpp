#ifndef _UI_FRAMERATE_HPP
#define _UI_FRAMERATE_HPP

#include <framerate.hpp>
#include <list>
#include <assert.h>

#define TICK_INTERVALL 2

enum eTicks
{
	DRAW_TICKS,
	GENERATION_TICKS,
	PROCESS_TICKS,
	MESSAGE_TICKS,
	SOUND_TICKS,
	IDLE_TICKS,
	
	MAX_TICK_TYPES
};



class UI_FrameRate
{
	public:
		UI_FrameRate(const bool is_optimizing, const unsigned int desired_framerate, const unsigned int desired_cpu);
		~UI_FrameRate();

		void delay();
		void poll(const eTicks etick);
		const bool allowCalculation();

		const std::list<unsigned int> getPercentList() const;
		const unsigned int getFramesPerSecond() const;
		const unsigned int getGenerationsPerSecond() const;

		void setDesiredFramerate(const unsigned int rate);
		void setDesiredCPU(const unsigned int rate);
		void setOptimizing(const bool is_optimizing);



	private:
		FPS* fps;
		long unsigned int currentTicks;
		long unsigned int ticks[MAX_TICK_TYPES][TICK_INTERVALL];
		unsigned int frames_count[TICK_INTERVALL];
		unsigned int framesPerSecond;
		unsigned int generationsPerSecond;
		signed int percent[MAX_TICK_TYPES];
		long unsigned int totalTicks;
		unsigned int tickIntervall;
		unsigned int framesPerGeneration;
		unsigned int refresh;

		void calculateFramerate();
};

inline void UI_FrameRate::setDesiredFramerate(const unsigned int rate) {
	assert(fps);
	fps->setDesiredFramerate(rate);
}

inline void UI_FrameRate::setDesiredCPU(const unsigned int rate) {
	assert(fps);
	fps->setDesiredCPU(rate);
}

inline void UI_FrameRate::setOptimizing(const bool is_optimizing) {
	assert(fps);
	fps->setAdaptFramesPerGeneration(is_optimizing);
}

inline const unsigned int UI_FrameRate::getFramesPerSecond() const {
	return(framesPerSecond);
}

inline const unsigned int UI_FrameRate::getGenerationsPerSecond() const {
	return(generationsPerSecond);
}

#endif
