#ifndef _RESOURCEHANDLER_HPP
#define _RESOURCEHANDLER_HPP

#include "resource.hpp"
#include "text.hpp"
#include <vector>
#include <map>
#include <string>

class MISC_API ResourceHandler : public Resource {

public:

	ResourceHandler();
	~ResourceHandler();

	const Resource& lookUpResource(const std::string& id) const;
	const Text& lookUpText(const std::string& id) const;
	const std::string& lookUpString(const std::string& id) const;
//	const Text& lookUpStringByID(const unsigned int id) const;
//	const unsigned int lookUpIDbyString(const std::string& id) const;
	const std::string lookUpFormattedString(const std::string& id, const std::string& text) const;
	const std::string lookUpFormattedString(const std::string& id, const unsigned int i) const;
	const std::string lookUpFormattedString(const std::string& id, const unsigned int i, const unsigned int j) const;
	const std::string lookUpFormattedString(const std::string& id, const unsigned int i, const unsigned int j, const unsigned int k) const;


	void addResource(const std::string& id, const Resource* resource);

	// z.B. update("english", "german") => weise alle englischen deutsche Begriffe zu
	void updateList(const std::string& id1, const std::string& id2); 
	void updateItems(std::map<std::string, const Resource*> list);

	const bool contains(const std::string& entry) const;
private:
	std::map<std::string, const Resource*> resourceList;
};




#endif