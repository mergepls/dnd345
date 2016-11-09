#include "ExitCell.h"

IMPLEMENT_SERIAL(ExitCell, CObject, 1)

ExitCell::ExitCell()
{
	type = CellHelper::EXIT_TYPE;
}


ExitCell::~ExitCell()
{
}

bool ExitCell::interactable() {
	return true;
}

bool  ExitCell::walkable() {
	return true;
}

void ExitCell::Serialize(CArchive& archive) {
	// call base class function first
	// base class is CObject in this case
	CObject::Serialize(archive);

	// now do the stuff for our specific class
	if (archive.IsStoring()) {
		archive << posX << posY << type;
	}

	/*for (int y = 0; y < height; y++) {
	for (int x = 0; x < width; x++) {
	archive << map[y][x];
	}
	}*/
	/*else
	archive >> m_name >> m_number;*/
}