#include "zrect.hpp"

ZRect::ZRect(Rect rect, signed int z, long unsigned int ID) {
	this->rect = rect;
	this->z = z;
	this->ID = ID;
	assert(ID < 1000);
	assert(z >= 0);
}

ZRect::~ZRect() {
}

const bool ZRect::isInFrontOf(const ZRect& r1, const ZRect& r2) {
	if(r1.z > r2.z || (r1.z == r2.z && r1.ID > r2.ID)) {
		return true;
	} else {
		return false;
	}
}

// remove overlapping rects
void ZRect::eraseOverlappingRects(std::list<ZRect>& rect_list) {
// compare all rects to each other
	//std::list<ZRect> new_rect_list;

	for(std::list<ZRect>::iterator i = rect_list.begin(); i != rect_list.end(); ) {
		std::list<ZRect>::iterator j = i;
		j++;
		while(j != rect_list.end()) {		
			if(i->rect.isTouched(j->rect)) {
				 
				// remove the upper part from the lower part
				if(ZRect::isInFrontOf(*i, *j)) {
					std::list<Rect> without_rect_list = j->rect.withoutRect(i->rect);
					for(std::list<Rect>::iterator s = without_rect_list.begin(); s != without_rect_list.end(); s++) {
						rect_list.push_back(ZRect(*s, j->z, j->ID));
					}
					j = rect_list.erase(j);
				} else if(ZRect::isInFrontOf(*j, *i)) {
					std::list<Rect> without_rect_list = i->rect.withoutRect(j->rect);
					for(std::list<Rect>::iterator s = without_rect_list.begin(); s != without_rect_list.end(); s++) {
						rect_list.push_back(ZRect(*s, i->z, i->ID));
					}
					i = rect_list.erase(i);
					break;
				} else {
					j++;
				}
				
			} else {
				j++;
			}
		}
		i++;
	}
	
	//rect_list.insert(rect_list, new_rect_list.begin(), new_rect_list.end());
}


// ok...

// oldrect newrect
void ZRect::eraseOverlappingRects(std::list<ZRect>& old_rect_list, std::list<ZRect>& new_rect_list) {
	for(std::list<ZRect>::iterator i = new_rect_list.begin(); i != new_rect_list.end(); ) {
		for(std::list<ZRect>::iterator j = old_rect_list.begin(); j != old_rect_list.end(); ) {
			// old rect weiter hinten?
			if(isInFrontOf(*i, *j) && i->rect.isTouched(j->rect)) {
				std::list<Rect> without_rect_list = j->rect.withoutRect(i->rect);
//				printf("Remove from oldrect: %s\n", j->rect.commonRect(i->rect).toString().c_str());
				std::list<ZRect> new_without_rect_list;
				for(std::list<Rect>::iterator s = without_rect_list.begin(); s != without_rect_list.end(); s++) {
					new_without_rect_list.push_back(ZRect(*s, i->z, i->ID));
				}
				old_rect_list.insert(j, new_without_rect_list.begin(), new_without_rect_list.end());
				j = old_rect_list.erase(j);
			} else {
				j++;
			}
		}
		i++;
	}
}

