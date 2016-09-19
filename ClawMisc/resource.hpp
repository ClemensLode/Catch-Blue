#ifndef _RESOURCE_HPP
#define _RESOURCE_HPP

#include <vector>
#include <assert.h>

#define MISC_API
/*#ifdef MISC_EXPORTS
    #define MISC_API __declspec(dllexport)
#else
    #define MISC_API __declspec(dllimport)
#endif
*/

class MISC_API Resource 
{
	public:
		// called by the instances when created
		// Pointer mit Adresse des Pointers in der jeweiligen Klasse
		void addObserver(Resource const**const observer);
		// called by the instances when deleted
		void removeObserver(Resource const*const*const observer);
		// called by the GUI, provides the new object, needs to be cast by the observer.
		void updateObservers(Resource const*const new_object) const;
		const std::string& toString() const;
		~Resource();
	protected:
		Resource();
	private:
		// we don't want to change the resource, we don't want to change the saved pointers to the pointers
		// we DO want to change the pointers to the resource.
		std::vector<Resource const**const> observers;
};


#endif