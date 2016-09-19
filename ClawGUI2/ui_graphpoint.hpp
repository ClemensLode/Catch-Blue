#ifndef _UI_GRAPHPOINT_HPP
#define _UI_GRAPHPOINT_HPP
#include <string>
#include <sdl.h>

class UI_GraphPoint
{
	public:
		UI_GraphPoint();
		UI_GraphPoint(const Sint16 target_x, const Sint16 number_y1, const Sint16 number_y2, const std::string& point_text = "");
		~UI_GraphPoint();
		
		void adjustTargetY(const Sint16 number_y1, const Sint16 number_y2);
		void setTargetY(const Sint16 number_y1, const Sint16 number_y2);
		void setTargetX(const Sint16 target_x);
		void setText(const std::string& point_text);

		const bool move();
		Sint16 current_y1;
		Sint16 start_y1;
		Sint16 target_y1;
		Sint16 current_y2;
		Sint16 start_y2;
		Sint16 target_y2;
		
		Sint16 x;
		
		Uint8 highlight;
		
		std::string text;
};

#endif

