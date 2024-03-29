#include "LootView.h"


void LootView::chestLootOptionsView(int input) {
	char in;
	string entity;

	if (input == 1)
		entity = "chest";
	else
		entity = "hostile";

	cout << "-+-+-+-+-+-+-+-++-+-+-+-+-+-+-+-+-+-+-+-" << endl;

	cout << "Do you want to loot the content of the " << entity << " [Y/n]? " << endl;
	cin >> in;

	bool cond = false;
	if (in == 'y' || in == 'Y')
		cond = true;

	if (in == 'n' || in == 'N')
		cond = false;

	if (input == 1)
	{
		Logger::instance()->appendToNewLine("GAMECONTROLLER: Player is looting chest.", "GAMECONTROLLER");
		LootController::instance()->lootChest(cond);
	}
	else {
		Logger::instance()->appendToNewLine("GAMECONTROLLER: Player is looting chest.", "GAMECONTROLLER");
		LootController::instance()->lootHostile(cond);
	}
}