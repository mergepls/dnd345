#pragma once

#include <string>

using namespace std;

enum ItemType { ARMOR, SHIELD, WEAPON, BOOTS, RING, HELMET };
//1:1 mapping of ItemType enum to char array since cannot convert enum to string in c++
const char* const ItemTypes[] = { "ARMOR", "SHIELD", "WEAPON", "BOOTS", "RING", "HELMET" };

class Item
{
public:
	Item();
	~Item();
	const static string type;
};
