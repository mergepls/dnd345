#pragma once
#include "Cell.h"

class WallCell : public Cell
{
public:
	WallCell();
	~WallCell();
	bool interactable();
	bool walkable();
};

