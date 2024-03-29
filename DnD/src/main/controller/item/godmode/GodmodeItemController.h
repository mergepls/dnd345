#pragma once

#include "../../../view/item/godmode/GodmodeItemView.h"
#include "../../../model/item/ItemContainer.h"
#include "../../../model/item/Armor.h"
#include "../../../model/item/Boots.h"
#include "../../../model/item/Helmet.h"
#include "../../../model/item/Ring.h"
#include "../../../model/item/Shield.h"
#include "../../../model/item/Weapon.h"
#include "../../../view/GameModeView.h"
#include "../../../helper/Logger.h"


class GodmodeItemController
{
	static GodmodeItemController *s_instance;
public:
	static GodmodeItemController* instance();
	void addItem(int, string);
	void removeItem(string, int);
	vector<Item*> getItemsOfType(int);
	void saveItemInventory(string);
	void loadItemInventory(int, bool);
	void loadSaveFile(string);
	ItemContainer* getContainer();
	void menuHelper(int selection);
	string getLoadedFile();
	void setLoadedFile(int);
	void resetController();
private:
	ItemContainer* container;
	GodmodeItemController();
	int loadedFile = 0;
};

