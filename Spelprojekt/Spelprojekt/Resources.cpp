#include "Resources.h"

Resources::Resources()
{
}

Resources::~Resources()
{
	for (auto i : resourceMap)
		delete i.second;

	resourceMap.clear();
}

void Resources::RemoveResource(std::string key)
{
	auto findItem = resourceMap.find(key);
	if (findItem != resourceMap.end())
		resourceMap.erase(findItem);
}
