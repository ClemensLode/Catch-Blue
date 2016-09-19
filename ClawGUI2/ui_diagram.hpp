#ifndef _UI_DIAGRAM_HPP
#define _UI_DIAGRAM_HPP

#include "ui_graphpoint.hpp"
#include "ui_statictext.hpp"
#include <string>
#include "windowcolors.hpp"

enum eGraphType {
	GRAPH_LINE,
	GRAPH_FILLED_LINE,
	GRAPH_BOX,
	GRAPH_SECOND_BOX, // need_supply!

	MAX_GRAPH_TYPES
};

class UI_Diagram : public UI_Object
{
	public:
		UI_Diagram(SDL_Object* diagram_parent, 
				const Rect& diagram_rect, 
				const Size distance_bottom_right, 
				const WindowColors* window_colors,
				const ePositionMode position_mode);
		~UI_Diagram();

		void resetData();
		void process();
		void draw(DC* dc) const;

		void reloadOriginalSize();

		void showValues(const bool show = true);

		void addItem(const std::string item_text, const Color* text_color, const Color* value_color);
		void addItem(const Text* item_text, const Color* text_color, const Color* value_color);
		void addGraph(const Pen* graph_pen, const Pen* graph_dashed_pen, const Brush* graph_brush, const eGraphType graph_type);
		void setItemValues(const std::vector<std::string>& value_list);

		std::vector<std::list<UI_GraphPoint> >& getGraphList();
		std::vector<unsigned int> graphMax;

	private:
		std::vector<std::list<UI_GraphPoint> > graphList;
		std::vector<eGraphType> graphType;
		std::vector<const Pen*> graphPen;
		std::vector<const Pen*> graphDashedPen;
		std::vector<const Brush*> graphBrush;
	
		std::vector<UI_StaticText*> itemText;
		std::vector<UI_StaticText*> itemValue;

		const WindowColors* windowColors;
	
		UI_Diagram(const UI_Diagram& object);
		UI_Diagram& operator=(const UI_Diagram& object);
};

inline std::vector<std::list<UI_GraphPoint> >& UI_Diagram::getGraphList() {
	return(graphList);
}


#endif // _UI_DIAGRAM_HPP
