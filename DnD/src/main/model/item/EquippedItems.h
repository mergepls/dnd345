#pragma once

#include <map>
#include <string>
#include "Item.h"
#include "ItemContainer.h"

class EquippedItems
{
public:
	//fix implementation so can only add items that are part of item container
	EquippedItems();
	~EquippedItems();
	map<string, Item*> equipped;
	void equipItem(Item*);
	void removeItem(string);
	Item* getItem(string);
private:
	ItemContainer* inventory;
};
