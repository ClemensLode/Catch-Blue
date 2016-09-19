#ifndef _UI_MENU_HPP
#define _UI_MENU_HPP

#include "ui_menuentry.hpp"
#include "objectsize.hpp"
#include "windowcolors.hpp"

enum eMenuType
{
	CUSTOM_MENU,
	ONE_COLOUMN_MENU,
	TWO_COLOUMNS_MENU,
	HORIZONTAL_MENU
};

class UI_Menu : public UI_Object
{
	public:
		// if all items to 'choose_menu' are used then it is a custom menu and reloadOriginalSize, updateItemPositions etc will not be called
		UI_Menu(SDL_Object* menu_parent, 
				const Rect& rect, 
				const Size distance_bottom_right, 
				const ePositionMode position_mode, 
				const bool choose_menu, 
				ObjectSize* object_size, 
				ButtonColors* button_colors,
				WindowColors* window_colors,
				std::vector<Text*> strings,
				eMenuType menu_type = CUSTOM_MENU 
				);
		~UI_Menu();
		void process();
		void draw(DC* dc) const;
		const Uint16 getHeight() const;
		const signed int getPressedItem();
		const signed int getMarkedItem() const;

		void forcePressItem(const unsigned int number);
		void close();
		void open();		
		const bool isOpen() const;
		const bool menuHasChanged() const;
		void setMenuHasChanged(const bool has_changed = true);
		void updateItemSizes(const Uint16 width);
		void updateItemPositions();
		void reloadOriginalSize();

	protected:
		void setMenuLevel(const unsigned int menu_level);
		std::list<UI_MenuEntry*> menuEntries;
		unsigned int menuLevel;
		signed int pressedItem;
		signed int markedItem;
		Uint16 height;
		bool chooseMenu;

	private:
		Point p1, p2;
		bool menuChanged;
		ButtonColors* buttonColors;
		ObjectSize* objectSize;
		eMenuType menuType;
		WindowColors* windowColors;
	
		UI_Menu& operator=(const UI_Menu& object);
		UI_Menu(const UI_Menu& object);
};

inline const bool UI_Menu::menuHasChanged() const {
	return(menuChanged);
}
	
inline const signed int UI_Menu::getMarkedItem() const {
	return(markedItem);
}


#endif // _UI_MENU_HPP

