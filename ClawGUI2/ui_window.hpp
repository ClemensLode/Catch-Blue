#ifndef _UI_WINDOW_HPP
#define _UI_WINDOW_HPP

#include "ui_scrollbar.hpp"
//#include "ui_panel.hpp"
#include "windowcolors.hpp"
#include "ui_title.hpp"

enum eIsAutoAdjust
{
	NO_AUTO_SIZE_ADJUST,
	AUTO_SIZE_ADJUST
};

struct SaveBoxParameter
{
	Text* saveboxText;
	Text* descriptionText;
	Text* okString;
	Text* cancelString;

	std::string inputProposal;
};

enum eTransparentWindow
{
	NOT_TRANSPARENT_WINDOW,
	TRANSPARENT_WINDOW
};


class UI_Window : public UI_Object
{
	public:
		UI_Window(SDL_Object* window_parent, 
				Text* window_title_string, 
				const Rect rect, 
				const Uint16 max_height, 
				WindowColors* window_colors,
				const eIsScrolled window_is_scrolled = NOT_SCROLLED, 
				const ButtonColors* scroll_up_button_colors = NULL,
				const ButtonColors* scroll_down_button_colors = NULL,
				const eIsAutoAdjust win_is_auto_adjust = NO_AUTO_SIZE_ADJUST, 
				const Rect win_client_area = Rect(0, 0, 1280, 1024), // TOO
				const eTransparentWindow transparent_window = NOT_TRANSPARENT_WINDOW);
		~UI_Window();
		// TODO protected machen (->datawindow)

		//UI_Panel* clientPanel;
	

	// returns position and size of the client area
		const Rect& getRelativeClientRect() const;
		const Rect getAbsoluteClientRect() const;
		const Point getRelativeClientRectPosition() const;
		const Point getAbsoluteClientRectPosition() const;
		const Uint16 getClientRectHeight() const;
		const Uint16 getClientRectWidth() const;
		const Size& getClientRectSize() const;
		const Sint16 getRelativeClientRectUpperBound() const;
		const Sint16 getRelativeClientRectLeftBound() const;
		const Sint16 getRelativeClientRectLowerBound() const;
		const Sint16 getRelativeClientRectRightBound() const;
		const Sint16 getAbsoluteClientRectUpperBound() const;
		const Sint16 getAbsoluteClientRectLeftBound() const;
		const Sint16 getAbsoluteClientRectLowerBound() const;
		const Sint16 getAbsoluteClientRectRightBound() const;
		const Uint16 getClientTargetHeight() const;
		const Uint16 getClientTargetWidth() const;

		void setRelativeClientRectPosition(const Point p);

		void process();

		void drawWindow(DC* dc) const;
		
		void draw(DC* dc) const;

		UI_Object* checkToolTip(const Point& mouse) const;
		const SDL_Object* checkHighlight(const Point& mouse);

// 		reconfigure rectangles depending on current theme settings
//		void updateRectangles(const Rect rect, const Uint16 max_height);
		
		static const bool getChangedFlag();
		static void setChangedFlag(const bool flag = true);
		static void changeAccepted();

		void setTitleParameter(const std::string& p);


		static unsigned int rectnumber;
		const bool fitItemToRelativeClientRect(const Rect& rectangle, const bool adjust = false);
		const bool fitItemToAbsoluteClientRect(const Rect& rectangle, const bool adjust = false);

		UI_ScrollBar* getScrollBar() const;
		void setScrollBarPosition(const Point p);
		const Uint16 getMaxHeight() const;

		void setMaxHeight(const Uint16 max_height);

		void wheelUp();
		void wheelDown();
		void moveScrollBarToTop();
		void moveScrollBarToBottom();

		void reloadOriginalSize();
//		static signed int gotoHelpChapter;
//		void addHelpButton(eHelpChapter help_chapter);

		static bool needSaveBox;
		static bool saveBoxCanceled;
		static bool saveBoxDone;
		static std::string saveBoxString;
		static SaveBoxParameter saveBoxParameter;
	protected:
		UI_Window& operator=(const UI_Window& object);
		UI_Window(const UI_Window& object);
	private:
	// do windows size changes smoothly		
		void adjustClientRect();
		Uint16 filledHeight;
		WindowColors* windowColors;
		bool doAdjustments;

		eTransparentWindow transparentWindow;
// no set/get for title as this is unique and does not change
		UI_Title* title;
		std::string titleParameter;
		
// client area
		Rect clientRect;
		Rect clientStartRect;
		Rect clientTargetRect;
// rectangle for max size of client area

		Rect originalClientRect;

		Uint16 maxHeight;

		void calculateClientRect();
//		void drawToolTip(DC* dc, const Point p, const std::string* tip) const;

		eIsAutoAdjust isAutoAdjust;
// has this window a ScrollBar?
		eIsScrolled isScrollable;

		bool highlighted;

		static bool changedFlag; 
		
// this windows may have a scroll bar		
		UI_ScrollBar* scrollBar;
//		UI_Button* helpButton;
//		eHelpChapter helpChapter;
	
};
		
inline void UI_Window::setScrollBarPosition(const Point p) {
	scrollBar->setPosition(p);
}
		
inline void UI_Window::setRelativeClientRectPosition(const Point p) 
{
	clientRect.setTopLeft(p);
	scrollBar->setStartY(p.getY());
}

inline const Uint16 UI_Window::getMaxHeight() const {
	return(maxHeight);
}

inline UI_ScrollBar* UI_Window::getScrollBar() const {
	return(scrollBar);
}

inline const Rect& UI_Window::getRelativeClientRect() const {
	return(clientRect);
}

inline const Point UI_Window::getRelativeClientRectPosition() const {
	return(clientRect.getTopLeft());
}

inline const Point UI_Window::getAbsoluteClientRectPosition() const {
	return(clientRect.getTopLeft()+getAbsolutePosition());
}

inline const Size& UI_Window::getClientRectSize() const {
	return(clientRect.getSize());
}

inline const Rect UI_Window::getAbsoluteClientRect() const {
	return(Rect(getAbsoluteClientRectPosition(), getClientRectSize()));
}

inline const Uint16 UI_Window::getClientRectHeight() const {
	
	return(clientRect.getHeight());
}

inline const Uint16 UI_Window::getClientRectWidth() const {
	return(clientRect.getWidth());
}

inline const Sint16 UI_Window::getRelativeClientRectUpperBound() const {
	return(clientRect.getTop());
}

inline const Sint16 UI_Window::getRelativeClientRectLeftBound() const {
	return(clientRect.getLeft());
}

inline const Sint16 UI_Window::getRelativeClientRectLowerBound() const {
	return(clientRect.getBottom());
}

inline const Sint16 UI_Window::getRelativeClientRectRightBound() const {
	return(clientRect.getRight());
}

inline const Sint16 UI_Window::getAbsoluteClientRectUpperBound() const {
	return(getAbsoluteClientRectPosition().getY());
}

inline const Sint16 UI_Window::getAbsoluteClientRectLeftBound() const {
	return(getAbsoluteClientRectPosition().getX());
}

inline const Sint16 UI_Window::getAbsoluteClientRectLowerBound() const {
	return(getAbsoluteClientRectPosition().getY() + getClientRectHeight() -1 ); // TODO?
}

inline const Sint16 UI_Window::getAbsoluteClientRectRightBound() const {
	return(getAbsoluteClientRectPosition().getX() + getClientRectWidth() -1 );
}

inline const Uint16 UI_Window::getClientTargetWidth() const {
	return(clientTargetRect.getWidth());
}

inline const Uint16 UI_Window::getClientTargetHeight() const {
	return(clientTargetRect.getHeight());
}

inline void UI_Window::setChangedFlag(const bool flag) {
	changedFlag = flag;
}

inline const bool UI_Window::getChangedFlag() {
	return(changedFlag);
}

inline void UI_Window::changeAccepted() {
	changedFlag = false;
}

inline void UI_Window::setMaxHeight(const Uint16 max_height) {
	maxHeight = max_height;
}




#endif

