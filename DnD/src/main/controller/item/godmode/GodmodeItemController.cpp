#include "GodmodeItemController.h"


//! default constructor that initializes an item container
GodmodeItemController::GodmodeItemController() 
{
	container = new ItemContainer();
}

//! singleton instantiation method
GodmodeItemController* GodmodeItemController::instance() {
	if (!s_instance)
		s_instance = new GodmodeItemController();

	return s_instance;
}

//! method that handles user input to add an item to the item container
//! @param selection: the user input of which type of item to create
//! @param name: the name of the item
void GodmodeItemController::addItem(int selection, string name)
{
	Item* item;
	switch (selection) 
	{
		case 1:
			item = new Armor(name);
			GodmodeItemController::instance()->container->addItem(item);
			break;
		case 2:
			item = new Shield(name);
			GodmodeItemController::instance()->container->addItem(item);
			break;
		case 3:
			item = new Weapon(name);
			GodmodeItemController::instance()->container->addItem(item);
			break;
		case 4:
			item = new Boots(name);
			GodmodeItemController::instance()->container->addItem(item);
			break;
		case 5:
			item = new Ring(name);
			GodmodeItemController::instance()->container->addItem(item);
			break;
		case 6:
			item = new Helmet(name);
			GodmodeItemController::instance()->container->addItem(item);
			break;
	}
}

//! method that handles user input to remove an item from the item container
//! @param type: the object type to be deleted
//! @param index: the index of the item to be deleted inside the vector 
void GodmodeItemController::removeItem(string type, int index)
{
	Item* item = container->getItem(type, index - 1);
	container->removeItem(item);
}

//! method that returns a vector of the specific item type
//! @param selection: a user input that is mapped to a item type
//! @return the vector of the specified type
vector<Item*> GodmodeItemController::getItemsOfType(int selection)
{
	return GodmodeItemController::instance()->container->getItemsOfType(ItemTypes[selection - 1]);
}

void GodmodeItemController::saveItemInventory()
{
	CFile theFile;
	theFile.Open(_T("ItemInventory.txt"), CFile::modeCreate | CFile::modeWrite);
	CArchive archive(&theFile, CArchive::store);

	container->Serialize(archive);

	archive.Close();
	theFile.Close();
}

void GodmodeItemController::loadItemInventory()
{
	CFile theFile;
	theFile.Open(_T("ItemContainer.txt"), CFile::modeRead);
	CArchive archive(&theFile, CArchive::load);

	ItemContainer* cont = new ItemContainer();
	cont->Serialize(archive);

	container = cont;

	archive.Close();
	theFile.Close();
}

GodmodeItemController* GodmodeItemController::s_instance = GodmodeItemController::instance();
