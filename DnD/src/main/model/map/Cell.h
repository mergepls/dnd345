#pragma once
#include "../../helper/CellHelper.cpp"
#include <afx.h>

class Cell : public CObject
{
protected:
	char type;
	int posX;
	int posY;

public:
	Cell();
	~Cell();
	virtual bool interactable();
	virtual bool walkable();
	char getType();
	int getPosX();
	int getPosY();
	void setPosX(int x);
	void setPosY(int y);

	virtual void Serialize(CArchive& archive);
	DECLARE_SERIAL(Cell);
};
