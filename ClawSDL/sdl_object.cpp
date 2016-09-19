/*rootObjectList: processAll!

process selber ruft children auf, test auf isShown
oder alle in process? Bewegungen wenn nicht gezeigt...
beim hiden auf zielrect setzen!*/

#include "sdl_object.hpp"

//#define CONNECT_RECTS

std::list<ZRect> oldRectList;
std::list<ZRect> newRectList;
std::list<SDL_Object*> rootObjectList;


SDL_Object::SDL_Object(SDL_Object* parent_object, const Rect& relative_rect, const Size& distance_bottom_right, const ePositionMode position_mode, const eAutoSize auto_size) :
	children(NULL),
	childrenWereChanged(false),

	positionMode(position_mode),
	
	pufferValid(false),
	moved(false),
	drawPuffer(NULL),
	drawType(SOLID_OBJECT),
	
	relativeRect(relative_rect),
	startRect(relative_rect),
	targetRect(relative_rect),
	originalRect(relative_rect),
	distanceBottomRight(distance_bottom_right),
	oldRect(relative_rect),

	autoSize(auto_size),
	shown(true),

	parent(NULL),
	positionParent(NULL),
	prevBrother(NULL),
	nextBrother(NULL)
{
	ID = ID_Counter; ID_Counter++;
	zCoordinate = 0;
	prevBrother = this;
	nextBrother = this;
	setParent(parent_object);
	if(parent == NULL) {
		rootObjectList.push_back(this);
	}
	Show();
}

SDL_Object::SDL_Object(SDL_Object* parent_object, SDL_Object* position_parent_object, const Rect& relative_rect, const Size& distance_bottom_right, const ePositionMode position_mode, const eAutoSize auto_size) :
	children(NULL),
	childrenWereChanged(false),
	positionMode(position_mode),

	pufferValid(false),
	moved(false),
	drawPuffer(NULL),
	drawType(SOLID_OBJECT),

	relativeRect(relative_rect),
	startRect(relative_rect),
	targetRect(relative_rect),
	originalRect(relative_rect),
	distanceBottomRight(distance_bottom_right),
	oldRect(relative_rect),

	autoSize(auto_size),
	shown(true),

	parent(NULL),
	positionParent(NULL),
	prevBrother(NULL),
	nextBrother(NULL)
{
	ID = ID_Counter; ID_Counter++;
	zCoordinate = 0;
	prevBrother = this;
	nextBrother = this;
	setParent(parent_object);
	setPositionParent(position_parent_object);

	if(parent == NULL) {
		rootObjectList.push_back(this);
	}
	Show();
}

SDL_Object::~SDL_Object()
{
	Show(false);

	if(SDL_Object::focus == this) {
		SDL_Object::resetFocus();
	}
	if(SDL_Object::highlighted == this) {
		SDL_Object::resetHighlighted();
	}
	if(parent == NULL) {
		removeRootObject(this);
	} else {
		removeFromFamily();
	}

}

void SDL_Object::removeRootObject(SDL_Object* object) {
	for(std::list<SDL_Object*>::iterator i = rootObjectList.begin(); i != rootObjectList.end();) {
		if(*i == object)
		{
			i = rootObjectList.erase(i);
			break;
		} else i++;
	}
}

void SDL_Object::reloadOriginalSize()
{

//=> Bool Variable und dann in process pruefen (nach show) damit erst dann upgedatet wird wenn das Teil wieder gezeigt wird.  TODO
	
	SDL_Object* tmp = children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->reloadOriginalSize();
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}

}

// ---- PUBLIC FUNCTIONS ----
void SDL_Object::adjustSize(const eAdjustMode adjust_mode, const Size& size)
{
	if(autoSize == NOTHING) {
		return;
	}

	Sint16 left = originalRect.getLeft();
	Uint16 full_width;
	if(getParent() != NULL)
	{
		if(left + distanceBottomRight.getWidth() > getParent()->getWidth()) {
			full_width = 0;
		} else {
			full_width = getParent()->getWidth() - left - distanceBottomRight.getWidth();
		}
	} 
	else {
		full_width = getWidth();
	}

	if(adjust_mode == PARENT_WAS_CHANGED)
	{
// called by PARENT after PARENT's size was changed
		switch(autoSize)
		{
			case NOTHING:break;
			case NO_AUTO_SIZE:break;
			case AUTO_SIZE:break;
			case AUTO_HEIGHT_FULL_WIDTH:
			case AUTO_DOUBLE_HEIGHT_FULL_WIDTH:
			case FULL_WIDTH:setWidth(full_width);break;
			case AUTO_HEIGHT_CONST_WIDTH:break;
			case AUTO_DOUBLE_HEIGHT_CONST_WIDTH:break;
			case CONST_SIZE:break;
			default:break; // TODO ERROR
		}
	} else
	if(adjust_mode == CHILD_WAS_CHANGED)
	{
		childrenWereChanged = true;
// called if the object is dependant on the child's size (e.g. Button - StaticText)
// TODO maybe update other children too
		switch(autoSize)
		{
			case NOTHING:break;
			case NO_AUTO_SIZE:setSize(originalRect.getSize());break;
			case AUTO_SIZE:setSize(size+Size(6, 0));break;
			case AUTO_HEIGHT_FULL_WIDTH:setSize(Size(full_width, size.getHeight()));break;
			case AUTO_DOUBLE_HEIGHT_FULL_WIDTH:setSize(Size(full_width, 2*size.getHeight()));break;
			case FULL_WIDTH:setSize(Size(full_width, getHeight()));break;
			case AUTO_HEIGHT_CONST_WIDTH:setSize(Size(originalRect.getWidth(), size.getHeight()));break;
			case AUTO_DOUBLE_HEIGHT_CONST_WIDTH:setSize(Size(originalRect.getWidth(), 2*size.getHeight()));break;
			case CONST_SIZE:setSize(Size(originalRect.getWidth(), originalRect.getHeight()));break;
			default:break; // TODO ERROR
		}
	}
}


void SDL_Object::adjustRelativeRect(const Rect& edge)
{
	if(edge != targetRect)
	{
		if(edge.getTopLeft() != targetRect.getTopLeft()) {
			startRect.setTopLeft(getRelativePosition());
		}
		if(edge.getSize() != targetRect.getSize()) {
			startRect.setSize(getSize());
		}
 		targetRect = edge;
	}
}




// ---- PRIVATE FUNCTIONS
	
void SDL_Object::adjustPosition()
{
	if(positionMode == DO_NOT_ADJUST) {
		return;
	}

	Sint16 left = originalRect.getLeft() + distanceBottomRight.getWidth();
	Sint16 top = originalRect.getTop() + distanceBottomRight.getHeight();
	Uint16 full_width;
	Uint16 full_height;
	if(getParent() != NULL)
	{
		full_width = getParent()->getWidth() - left;// - distanceBottomRight.getWidth();
		full_height = getParent()->getHeight() - top;// - distanceBottomRight.getHeight();
	} 
	else
	{
		full_width = getWidth();
		full_height = getHeight();
	}
	
	Sint16 hor_center = left + ((Sint16)full_width - (Sint16)getWidth())/2;
	Sint16 right = (Sint16)full_width - left - (Sint16)getWidth();

	Sint16 ver_center = top + ((Sint16)full_height - (Sint16)getHeight())/2;
	if(ver_center < top) {
		ver_center = top;
	}
	Sint16 bottom = (Sint16)full_height - top - (Sint16)getHeight();

 	switch(positionMode)
	{
		case DO_NOT_ADJUST:break;
		case TOTAL_CENTERED:setTargetPosition(hor_center, ver_center);break;
		case HORIZONTALLY_CENTERED:setTargetLeft(hor_center);break;
		case VERTICALLY_CENTERED:setTargetTop(ver_center);break;
		case TOP_LEFT:setTargetPosition(left, top);break;
		case TOP_CENTER:setTargetPosition(hor_center, top);break;
		case TOP_RIGHT:setTargetPosition(right, top);break;
		case CENTER_RIGHT:setTargetPosition(right, ver_center);break;
		case BOTTOM_RIGHT:setTargetPosition(right, bottom);break;
		case BOTTOM_CENTER:setTargetPosition(hor_center, bottom);break;
		case BOTTOM_LEFT:setTargetPosition(left, bottom);break;
		case CENTER_LEFT:setTargetPosition(left, ver_center);break;
		default:break;
	}
}






void SDL_Object::removeFromFamily()
{
	// has a parent and this object is at the head of the children linked list?
	if((parent)&&(parent->children == this))
	{
		if (nextBrother == this) { // the only child?
			parent->children = NULL;  // if so, parent now has now no children
		} else {
			parent->children = nextBrother;  // next sibling is now the oldest
		}
	} 

	parent = NULL;
	
	if (nextBrother != this) {  // does this object have siblings?
		nextBrother->prevBrother = prevBrother;
		prevBrother->nextBrother = nextBrother;
	}
	// no more brothers
	nextBrother = prevBrother = this;
}

void SDL_Object::setPositionParent(SDL_Object* parent)
{
	positionParent = parent;
	// TODO evtl newrect update
}

void SDL_Object::setParent(SDL_Object* parent) 
{
	setPositionParent(parent);
	removeFromFamily();
	if(this->parent == NULL && parent != NULL) {
		removeRootObject(this);
	}
	this->parent = parent;
	if(this->parent != NULL) {
		// TODO kleine Schritte wenn Element Kinder enthält und Gebiete größer der der Kinder abdeckt naja... TODO
		setZ(this->parent->getZ() + 20);
		this->parent->addChild(this);
	} 
	else {
		setZ(0);
	}
}

void SDL_Object::addChild(SDL_Object* child)
{
	// TODO evtl noch child auf "hasMoved" setzen?
	childrenWereChanged = true;
	if (!children) {
		children = child;
	}
	else 
	{
		SDL_Object *eldest_sibling, *youngest_sibling;

		eldest_sibling = children;
		youngest_sibling = eldest_sibling->prevBrother;

		child->nextBrother = eldest_sibling;
		child->prevBrother = youngest_sibling;

		eldest_sibling->prevBrother = child;
		youngest_sibling->nextBrother = child;
	}
}



/**
 * Process all root objects
 */
void SDL_Object::processAll()
{
	for(std::list<SDL_Object*>::const_iterator i = rootObjectList.begin(); i != rootObjectList.end(); ++i) {
		(*i)->process();
	}
}


void SDL_Object::process()
{
	if(!isShown()) {
		return; 
	}

	if(SDL_Object::smoothMovements) {
		relativeRect.moveSmooth(startRect, targetRect);
	} else {
		relativeRect = targetRect;
		startRect = relativeRect;
	}

	if(relativeRect.getTopLeft() == targetRect.getTopLeft()) {
		startRect.setTopLeft(relativeRect.getTopLeft());
	}
	if(relativeRect.getSize() == targetRect.getSize()) {
		startRect.setSize(relativeRect.getSize());
	}

	adjustPosition();

	if((oldRect.getSize() != getSize()))
	{
		if(getParent()) {
			getParent()->adjustSize(CHILD_WAS_CHANGED, getSize());
		}
				
		SDL_Object* tmp = children;  // process all children of gadget
		if (tmp) {
			do {
				tmp->adjustSize(PARENT_WAS_CHANGED, getSize());
				tmp->adjustPosition();
				tmp = tmp->nextBrother;
			} while (tmp != children);
		}		
	}
		
	SDL_Object* tmp = children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->process();
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}	

	checkRectHasChanged();
}



void SDL_Object::checkRectHasChanged()
{
	if( (!isShown()) || (oldRect == getAbsoluteRect()) ) {
		return;
	}
//nicht z vom objekt sondern vom darunterliegenden objekt! -> wird später geprüft
	checkAreaVisibility(ZRect(oldRect, getZ(), ID), false);

	if(oldRect.getSize() != getSize()) {
		SDL_FreeSurface(drawPuffer);
		drawPuffer = NULL;
		makePufferInvalid();
	} else {
		checkAreaVisibility(ZRect(getAbsoluteRect(), getZ(), ID), true); 
	}
	if(oldRect.getTopLeft() != getAbsolutePosition()) {
		moved = true;
	}
	// TODO evtl Fall machen wenn sich Rect nur vergrößert aber stehenbleibt

	oldRect = getAbsoluteRect();
}

void SDL_Object::resetData()
{ 
	SDL_Object* tmp = children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->resetData();
			tmp = tmp->nextBrother;
																				
		} while (tmp != children);
	}
}

void SDL_Object::redrawWholeObject() {
	if(!isShown()) {
		return;
	}

	makePufferInvalid();

	// todo evtl raus
	SDL_Object* tmp = children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->redrawWholeObject();
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}	
}


void SDL_Object::draw(DC* dc) const
{
	assert(dc);
}










// don't call this function during initialization! oldrect will be initialized wrongly
void SDL_Object::setZ(const signed int zcoord)
{
	checkAreaVisibility(ZRect(getAbsoluteRect(), getZ(), ID), false);
	zCoordinate = zcoord;
	checkAreaVisibility(ZRect(getAbsoluteRect(), getZ(), ID), true);

	SDL_Object* tmp = children;
	if (tmp) {
		do {
			tmp->setZ(zcoord + 10);
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}
}


const bool SDL_Object::isMoving() const
{
	return((isShown()) && (relativeRect.getLeft() != targetRect.getLeft()));
}



void SDL_Object::Show(const bool show)
{
	// show value has changed
	if(show != shown) {

		shown = show;

		if(SDL_Object::focus == this) {
			SDL_Object::resetFocus();
		}
		if(SDL_Object::highlighted == this) {
			SDL_Object::resetHighlighted();
		}

		checkAreaVisibility(ZRect(getAbsoluteRect(), getZ(), ID), show);
		
		if(getParent()) {
			getParent()->childrenWereChanged = true;
		}
	} 
	
}


void SDL_Object::makeFirstChild() {
	if((!getParent())||((getParent()->children == this)&&(getParent()->children->getNextBrother() == this))) {
		return;
	}
	SDL_Object* old_parent = getParent();
	removeFromFamily();
	setParent(old_parent);
}

void SDL_Object::setTargetPosition(const Point &position) {
	targetRect.setTopLeft(position);
	Sint16 x = getRelativePosition().getX();
	Sint16 y = getRelativePosition().getY();

	if(!((x >= startRect.getLeft() && x <= targetRect.getLeft()) || (x <= startRect.getLeft() && x >= targetRect.getLeft()) )) {
		startRect.setLeft(x);
	}
	if(!((y >= startRect.getTop() && y <= targetRect.getTop()) || (y <= startRect.getTop() && y >= targetRect.getTop()) )) {
		startRect.setTop(y);
	}
}



// -------- BEGIN DRAWING --------





void SDL_Object::checkAreaVisibility(ZRect rect, bool show) {
	if(show) {
// contents of 'rect' were changed (because the object moved or because the object was redrawn)
		newRectList.push_back(rect);
//		toInitLog("New Rect: " + rect.rect.toString());
	} else {
// contents of 'rect' are now visible again, re-copy all objects at the top
		oldRectList.push_back(rect);
//		toInitLog("Old Rect: " + rect.rect.toString());
	}
}


void SDL_Object::makePufferInvalid() {
	checkAreaVisibility(ZRect(getAbsoluteRect(), getZ(), ID), true); 
	pufferValid = false;
}


void SDL_Object::updateAreasToUpdate()
{
//	rootObjectList.sort(sort_objects_by_z());
/*
	Update areas

- if size/position has changed: add old and new rect OK
- if puffer invalid: add new rect                    OK

- remove same new rects with same or larger z

- remove intersections of old rects with new rects with larger z
*/

//	ZRect::eraseOverlappingRects(newRectList);
//	ZRect::eraseOverlappingRects(oldRectList);

	//durch alle Objekte laufen, oldRectList überprüfen und das größte z zuweisen

	for(std::list<SDL_Object*>::const_iterator i = rootObjectList.begin(); i != rootObjectList.end(); ++i) {
		(*i)->checkHighestZ(oldRectList, newRectList);
	}
    ZRect::eraseOverlappingRects(oldRectList, newRectList);

/*
Aktualisiere Gebiete:

- if size/position has changed: add old and new rect
- if puffer invalid: add new rect

- remove same new rects with same z
- remove intersections of old rects with new rects with larger z
// - merge new rects?
// - merge old rects?


Copy contents:

puffer invalid:
- remove all areas with larger z (neues rect)

puffer valid:
- copy only intersection with larger z (altes rect)
*/

}

void SDL_Object::checkHighestZ(std::list<ZRect>& old_rect_list, std::list<ZRect>& new_rect_list) {

	std::list<ZRect> add_to_list;

	// Finde die Rects der Elemente die am vordersten sind
	for(std::list<ZRect>::iterator i = old_rect_list.begin(); i != old_rect_list.end(); ) {

		if(i->rect.isTouched(getAbsoluteRect())) {
			
			// old_rect vor Objekt?
			
			if(ZRect::isInFrontOf(*i, ZRect(Rect(), getZ(), ID)))
				// Rechteck ist näher am Betrachter => Dahinterliegendes (i) muss neugezeichnet werden
				// Suche größtes dahinterliegendes!
		{
			//		std::ostringstream os;
			//		os << "Old rect touches " << getAbsoluteRect().toString() << " with Z " << i->z << ">=" << getZ() << " und ID " << i->ID << ">" << ID;
			//		toInitLog(os.str());
						
			
					//TODO größte ID?
					// jeder braucht noch TempID und tempz, sonst wird z.B. ID 2 durch ID 0 upgedated und ID 1 übersprungen!
				//	os.str("");
				//	os << "Update ID " << i->ID << " with ID " << ID;
				//	toInitLog(os.str());
					add_to_list.push_back(ZRect(i->rect.commonRect(getAbsoluteRect()), i->z, ID));
					
					// füge Restteil ein
					std::list<Rect> wr = i->rect.withoutRect(getAbsoluteRect());
					for(std::list<Rect>::iterator j = wr.begin(); j != wr.end(); j++) {
						add_to_list.push_back(ZRect(*j, i->z, i->ID));
				//		toInitLog("Add Z " + j->toString());
					}
					i = old_rect_list.erase(i);
			} else if(ZRect::isInFrontOf(ZRect(Rect(), getZ(), ID), *i))
			{
				//new_rect_list.push_back(ZRect(i->rect.commonRect(getAbsoluteRect()), getZ(), ID));
				add_to_list.push_back(ZRect(i->rect.commonRect(getAbsoluteRect()), getZ(), ID));
			//	toInitLog("Add new Rect from Z: " + i->rect.commonRect(getAbsoluteRect()).toString() + "(rect: " + getAbsoluteRect().toString() + " / old rect: " + i->rect.toString() + ")");
			//	std::ostringstream os;
			//	os << ID << " > " << i->ID;
			//	toInitLog(os.str());
				std::list<Rect> wr = i->rect.withoutRect(getAbsoluteRect());
				for(std::list<Rect>::iterator j = wr.begin(); j != wr.end(); j++) {
					add_to_list.push_back(ZRect(*j, i->z, i->ID));
//					toInitLog("Add Z " + j->toString());
				}
				i = old_rect_list.erase(i);				
			} else {
				i++;
			}
		} else {
			i++;
		}
	}
	old_rect_list.insert(old_rect_list.end(), add_to_list.begin(), add_to_list.end());


	// es können mehrere Rects ein Gebiet mit NewRect beanspruchen! Nur vordersten nehmen!
	// verdeckte newrects außerdem abschneiden! TODO
	
	for(std::list<ZRect>::iterator i = new_rect_list.begin(); i != new_rect_list.end(); ) {
		if(i->rect.isTouched(getAbsoluteRect()) && 
			ZRect::isInFrontOf(ZRect(Rect(), getZ(), ID), *i)) {
			// Rechteck ist weiter entfernt, Davorliegendes muss nicht neugezeichnet werden
			// Entferne Teil von oldRectList
			//toInitLog("Remove " + i->rect.commonRect(getAbsoluteRect()).toString() + " from newRect " + i->rect.toString());
			// alte rect-Teile mit altem Z initialisieren
			std::list<Rect> l = i->rect.withoutRect(getAbsoluteRect());
			for(std::list<Rect>::iterator j = l.begin(); j != l.end(); j++) {
				new_rect_list.push_back(ZRect(*j, i->z, i->ID));
			}
			new_rect_list.push_back(ZRect(i->rect.commonRect(getAbsoluteRect()), getZ(), ID));
			i = new_rect_list.erase(i);
		} else {
			i++;
		}
	}



	SDL_Object* tmp = children;  // process all children of gadget
	if (tmp) {
		do {
			tmp->checkHighestZ(old_rect_list, new_rect_list);
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}	
}

const SDL_Object* SDL_Object::checkHighlight(const Point& mouse)
{
	if(!isShown()) {
		return(NULL);
	}

	SDL_Object* tmp = getChildren();  // process all children of gadget
	
	if(!tmp) {
		return(NULL); // return 0 as this is an object and no button!
	}

	const SDL_Object* result = NULL;
	do 
	{
		result = tmp->checkHighlight(mouse);
		tmp = tmp->getNextBrother();
	} while( tmp != getChildren() && (result == NULL) );
	
	return(result);
}

void SDL_Object::drawAll(DC* dc) {
	assert(dc);

	for(std::list<SDL_Object*>::const_iterator i = rootObjectList.begin(); i != rootObjectList.end(); i++ ) {		
		(*i)->putOnScreen(dc);
	}

	oldRectList.clear();
	newRectList.clear();
}

// put single object on screen
void SDL_Object::putOnScreen(DC* dc)
{
	assert(dc);

	if (!isShown()) {
		return;
	}
/*Copy contents:

puffer invalid / moved:
- remove all areas with larger z (neues rect)

puffer valid:
- copy only intersection with larger z (altes rect)
*/
//alle objekte mit newrect rein? mmh oder durch alle durchlaufen... bei highestZ... mmmh....
	std::list<Rect> blitRectList;

	// everything that isn't covered by other objects (newrects) needs to be redrawn
	if(!isPufferValid() || wasMoved()) {
		// assume to blit everything
		blitRectList.push_back(getAbsoluteRect());

	// now remove all areas which are covered by new objects with a larger Z	
		
		for(std::list<ZRect>::iterator i = newRectList.begin(); i != newRectList.end(); i++) {
			if(ZRect::isInFrontOf(*i, ZRect(Rect(), getZ(), ID))) {

				std::list<Rect> add_blit_rect_list;

			 // check all blitRectList parts
				for(std::list<Rect>::iterator j = blitRectList.begin(); j != blitRectList.end(); ) {
					if(i->rect.isTouched(*j)) {
						// remove the area which is overlapped by the new rectangle
						std::list<Rect> new_list = j->withoutRect(i->rect);
					//	std::ostringstream os;
					//	os << "Remove " << i->rect.toString() << " (Z = " << i->z << ", ID = " << i->ID << ") from " << j->toString() << " (Z = " << getZ() << ", ID = " << ID << ") => blitrect but " << j->commonRect(i->rect).toString() << " (" + new_list.size() << ")";
					//	toInitLog(os.str());
						add_blit_rect_list.insert(add_blit_rect_list.end(), new_list.begin(), new_list.end());
						j = blitRectList.erase(j);
					} else {
						j++;
					}
				}
				blitRectList.insert(blitRectList.end(), add_blit_rect_list.begin(), add_blit_rect_list.end());
				add_blit_rect_list.clear();
				
			}
		}		
	}

	// object was not moved or redrawn
	// no check required, except checking whether objects that were above
	// this object moved away.

	// only redraw parts that were formerly covered by other objects (oldrects)
	else {
		for(std::list<ZRect>::iterator i = oldRectList.begin(); i != oldRectList.end(); i++) {
			if(i->ID == ID) {
				blitRectList.push_back(i->rect);
			}
		}		
	}

	
//Situation: Liegt B über C und A über B und C, dann eliminiert darüberliegendes B das old_rect komplett...
//			   bzw. kein new_rect wird gesetzt!
	
//	Liegt old_rect über 
	
// use drawPuffer only with solid objects
	if(getDrawType() != ANTI_ALIASED_OBJECT)
	{
		bool clear_puffer = false;
		if(drawPuffer == NULL)
		{
			const SDL_PixelFormat* pixel_format = dc->getSurface()->format;
			drawPuffer = SDL_CreateRGBSurface( SDL_SWSURFACE, getWidth(), getHeight(), dc->getBits(), pixel_format->Rmask, pixel_format->Gmask, pixel_format->Bmask, pixel_format->Amask);
			makePufferInvalid();
			if(getDrawType() == TRANSPARENT_OBJECT) {
				SDL_SetColorKey(drawPuffer, SDL_SRCCOLORKEY , SDL_MapRGB(drawPuffer->format, 0, 0, 0));
			}
		} else {
			if(getDrawType() == TRANSPARENT_OBJECT) {
				clear_puffer = true;
			}
		}

		if(!isPufferValid())
		{
			if(clear_puffer) {
				SDL_FillRect(drawPuffer, NULL, 0);
			}
			dc->switchToSurface(drawPuffer);
			dc->drawFromPoint(Point(0,0));
		}
		else {
			dc->drawFromPoint(getAbsolutePosition());
		}
	}

	if(!isPufferValid())
	{
		draw(dc); // fill puffer or draw directly to screen
		if(getDrawType() != ANTI_ALIASED_OBJECT) {
			dc->switchToOriginalSurface();
		}
	}
	
	if(getDrawType() != ANTI_ALIASED_OBJECT)
	{
		for(std::list<Rect>::iterator i = blitRectList.begin(); i != blitRectList.end(); )
		{
			Point p = getAbsolutePosition();
			SDL_Rect source = DC::createRect(i->getLeft() - p.getX(), i->getTop() - p.getY(), i->getWidth(), i->getHeight());
			SDL_Rect target = DC::createRect(i->getLeft(), i->getTop(), 0, 0);
			SDL_BlitSurface(drawPuffer, &source, dc->getSurface(), &target);
			dc->addRectangle(*i);
			i = blitRectList.erase(i);
		}
	} 
	else if(blitRectList.size()>0)
	{ // TODO
		dc->addRectangle(getAbsoluteRect());
		dc->drawFromPoint(Point(0,0));
		draw(dc);
	}

	SDL_Object* tmp = children;
	if (tmp) {
		do {
			tmp->putOnScreen(dc);
			tmp = tmp->nextBrother;
		} while (tmp != children);
	}

	resetMoved();
	makePufferValid();

	blitRectList.clear();
}


// -------- END DRAWING --------




unsigned int SDL_Object::redrawnObjects = 0;
bool SDL_Object::smoothMovements = false;
SDL_Object* SDL_Object::focus = NULL;
const SDL_Object* SDL_Object::highlighted = NULL;
long unsigned int SDL_Object::ID_Counter = 0;