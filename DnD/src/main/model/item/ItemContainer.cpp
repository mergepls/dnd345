#include "ItemContainer.h"



ItemContainer::ItemContainer()
{
	//iterate through all item types defined in ItemType enum
	for (int i = 0; i <= HELMET; i++) 
	{
		string str(ItemTypes[i]);
		items.insert(pair<string, vector<Item*>>(str, vector<Item*>()));
	}
}


ItemContainer::~ItemContainer()
{
	//TODO: delete arraylist in map and then the map itself
}


void ItemContainer::addItem(Item* item)
{
	//TODO: check if valid item type
	items[item->type].push_back(item);
}

void ItemContainer::removeItem(Item* item)
{
	//TODO: check if valid item type
	for (vector<Item*>::iterator it = items[item->type].begin(); it != items[item->type].end(); ++it)
	{
		if (*it == item) 
		{
			items[item->type].erase(it);
		}
	}
}

Item* ItemContainer::getItem(Item* item)
{
	//TODO: check if valid item type
	for (vector<Item*>::iterator it = items[item->type].begin(); it != items[item->type].end(); ++it)
	{
		if (*it == item)
		{
			return *it;
		}
	}
	return NULL;
}
