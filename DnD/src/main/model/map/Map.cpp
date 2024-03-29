//! @file 
//! @brief Implementation file for the Map
//!

#include "Map.h"
#include "../../view/map/godmode/GodmodeMapView.h"

IMPLEMENT_SERIAL(Map, CObject, 1)

Map::Map(int width, int height) {
	this->width = width;
	this->height = height;
	
	initMap();
}

Map::~Map() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			delete map[y][x];
		}

		map[y].clear();
	}	
	
	map.clear();
	pathCells.clear();
	entityCells.clear();
	chestCells.clear();
	wallCells.clear();
	passed.clear();
}

Map::Map() {}

//! initiate the map vector
void Map::initMap() {
	map.resize(height, vector<Cell*>(width));

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			map[y][x] = new PathCell();
			map[y][x]->setPosX(x);
			map[y][x]->setPosY(y);
			pathCells.push_back(dynamic_cast<PathCell*>(map[y][x]));
		}
	}

	//Init passed vector
	passed.resize(height, vector<bool>(width, false));
}

//! Print the map
void Map::print() {
	//Print delimiter
	cout << "=";
	for (int i = 0; i < width; i++) {
		cout << "=";
	}
	cout << "=";
	cout << endl;

	//Print map
	for (int y = 0; y < height; y++) {
		cout << "|";
		for (int x = 0; x < width; x++) {
			char tpe = map[y][x]->getType();

			if (tpe == CellHelper::ENTITY_TYPE) {
				tpe = dynamic_cast<EntityCell*>(map[y][x])->printType();
			}

			cout << tpe;
		}
		cout << "|" << endl;
	}
	
	//Print delimiter
	cout << "=";
	for (int i = 0; i < width; i++) {
		cout << "=";
	}
	cout << "=";
	cout << endl;
}

//! Change the cell on the map
//@Param x: x position of the cell
//@param y: y position of the cell
//@param cell: new cell object 
void Map::fillCell(int x, int y, Cell* cell) {

	char oldType = map[y][x]->getType();
	char newType = cell->getType();

	switch (oldType) {
	case CellHelper::CHEST_TYPE:
		chestCells.erase(std::remove(chestCells.begin(), chestCells.end(), map[y][x]), chestCells.end());
		break;
	case CellHelper::ENTITY_TYPE:
		entityCells.erase(std::remove(entityCells.begin(), entityCells.end(), map[y][x]), entityCells.end());
		break;
	case CellHelper::PATH_TYPE:
		pathCells.erase(std::remove(pathCells.begin(), pathCells.end(), map[y][x]), pathCells.end());
		break;
	case CellHelper::WALL_TYPE:
		wallCells.erase(std::remove(wallCells.begin(), wallCells.end(), map[y][x]), wallCells.end());
		break;
	case CellHelper::ENTRANCE_TYPE:
		startingCell = NULL;
		break;
	case CellHelper::EXIT_TYPE:
		exitCell = NULL;
		break;
	}

	switch (newType) {
	case CellHelper::CHEST_TYPE:
		chestCells.push_back(dynamic_cast<ChestCell*>(cell));
		break;
	case CellHelper::ENTITY_TYPE:
		entityCells.push_back(dynamic_cast<EntityCell*>(cell));
		break;
	case CellHelper::PATH_TYPE:
		pathCells.push_back(dynamic_cast<PathCell*>(cell));
		break;
	case CellHelper::WALL_TYPE:
		wallCells.push_back(dynamic_cast<WallCell*>(cell));
		break;
	}

	if (cell->getType() == CellHelper::ENTRANCE_TYPE) {
		if (startingCell != NULL) {
			GodmodeMapView::warningMsgChangingEntranceCell();
			int previousStartingX = startingCell->getPosX();
			int previousStartingY = startingCell->getPosY();
			map[previousStartingY][previousStartingX] = new PathCell();

			delete startingCell;
			
		}

		startingCell = dynamic_cast<EntranceCell*>(cell);
	}else if (cell->getType() == CellHelper::EXIT_TYPE) {
		if (exitCell != NULL) {
			GodmodeMapView::warningMsgChangingExitCell();
			int previousExitX = exitCell->getPosX();
			int previousExitY = exitCell->getPosY();
			map[previousExitY][previousExitX] = new PathCell();

			delete exitCell;
		}

		exitCell = dynamic_cast<ExitCell*>(cell);
	}

	Cell* currentCell = map[y][x];
	map[y][x] = NULL;
	delete currentCell;

	cell->setPosX(x);
	cell->setPosY(y);
	map[y][x] = cell;
}

//! Validates the map
//! There is three validations happening
//! 1. There must be an entrance and an exit cell
//! 2. There must be a valid path between the entrance and exit cell
//! 3. All walkable cell must be accessible from the entrance cell
bool Map::validateMap() {
	if (startingCell == NULL || exitCell == NULL) {
		return false;
	}

	resetPassed();

	int startX = startingCell->getPosX();
	int startY = startingCell->getPosY();

	bool validPath = isPathValid(startX, startY, startX, startY);

	if (!validPath) {
		return false;
	}

	resetPassed();

	bool allWalkableCellsAccessible = isAllWalkableCellsAccessible();

	if (!allWalkableCellsAccessible) {
		return false;
	}

	return true;
}

bool Map::isAllWalkableCellsAccessible() {
	int startX = startingCell->getPosX();
	int startY = startingCell->getPosY();

	bool done = walkThroughMap(startX, startY, startX, startY);
	bool valid = true;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (map[y][x]->walkable() != passed[y][x]) {
				valid = false;
				break;
			}
		}
	}

	return valid;
}

//! Walk through every cell that is not walkable.
bool Map::walkThroughMap(int x, int y, int prevX, int prevY) {
	if (!passed[y][x]) {
		passed[y][x] = true;
	}

	//Down
	if (y + 1 < height && (y + 1 != prevY || x != prevX)) {
		if (!passed[y + 1][x]) {
			if (map[y + 1][x]->getType() != CellHelper::WALL_TYPE) {

				if (walkThroughMap(x, y + 1, x, y)) {
					return true;
				}
			}
		}
	}

	//Right
	if (x + 1 < width && (x + 1 != prevX || y != prevY)) {
		if (!passed[y][x + 1]) {
			if (map[y][x + 1]->getType() != CellHelper::WALL_TYPE) {
				if (walkThroughMap(x + 1, y, x, y)) {
					return true;
				}
			}
		}
	}

	//Up
	if (y - 1 >= 0 && (y - 1 != prevY || x != prevX)) {
		if (!passed[y - 1][x]) {
			if (map[y - 1][x]->getType() != CellHelper::WALL_TYPE) {
				if (walkThroughMap(x, y - 1, x, y)) {
					return true;
				}
			}
		}
	}

	//Left
	if (x - 1 >= 0 && (x - 1 != prevX || y != prevY)) {
		if (!passed[y][x - 1]) {
			if (map[y][x - 1]->getType() != CellHelper::WALL_TYPE) {
				if (walkThroughMap(x - 1, y, x, y)) {
					return true;
				}
			}
		}

	}

	return false;
}

//! Recursive method to walk through the map. If it reaches an exit cell, it will return true. If not, return false.
bool Map::isPathValid(int x, int y, int prevX, int prevY) {
	if (!passed[y][x]) {
		passed[y][x] = true;
	}

	if (map[y][x]->getType() == CellHelper::EXIT_TYPE) {
		return true;
	}

	//Down
	if (y + 1 < height && (y + 1 != prevY || x != prevX)) {
		if (!passed[y + 1][x]) {
			if (map[y + 1][x]->getType() != CellHelper::WALL_TYPE) {

				if (isPathValid(x, y + 1, x, y)) {
					return true;
				}
			}
		}
	}

	//Right
	if (x + 1 < width && (x + 1 != prevX || y != prevY)) {
		if (!passed[y][x + 1]) {
			if (map[y][x + 1]->getType() != CellHelper::WALL_TYPE) {
				if (isPathValid(x + 1, y, x, y)) {
					return true;
				}
			}
		}
	}

	//Up
	if (y - 1 >= 0 && (y - 1 != prevY || x != prevX)) {
		if (!passed[y - 1][x]) {
			if (map[y - 1][x]->getType() != CellHelper::WALL_TYPE) {
				if (isPathValid(x, y - 1, x, y)) {
					return true;
				}
			}
		}
	}

	//Left
	if (x - 1 >= 0 && (x - 1 != prevX || y != prevY)) {
		if (!passed[y][x - 1]) {
			if (map[y][x - 1]->getType() != CellHelper::WALL_TYPE) {
				if (isPathValid(x - 1, y, x, y)) {
					return true;
				}
			}
		}

	}

	return false;
}

void Map::resetPassed() {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			passed[y][x] = false;
		}
	}
}

void Map::Serialize(CArchive& archive) {
	// call base class function first
	// base class is CObject in this case
	CObject::Serialize(archive);

	// now do the stuff for our specific class
	if (archive.IsStoring()) {
		archive << height << width;

		startingCell->Serialize(archive);
		exitCell->Serialize(archive);
		
		chestSize = chestCells.size(); 
		entitySize = entityCells.size(); 
		wallSize = wallCells.size(); 
		pathSize = pathCells.size();

		archive << chestSize << entitySize << wallSize << pathSize;

		if(!chestCells.empty())
			for (int a = 0; a < chestCells.size(); a++) {
				chestCells[a]->Serialize(archive);
			}

		if (!entityCells.empty())
			for (int b = 0; b < entityCells.size(); b++) {
				entityCells[b]->Serialize(archive);
			}

		if (!wallCells.empty())
			for (int c = 0; c < wallCells.size(); c++) {
				wallCells[c]->Serialize(archive);
			}

		if (!pathCells.empty())
			for (int d = 0; d < pathCells.size(); d++) {
				pathCells[d]->Serialize(archive);
			}
	}
	else {
		archive >> height >> width;

		map.resize(height, vector<Cell*>(width));
		passed.resize(height, vector<bool>(width, false));

		startingCell = new EntranceCell();
		startingCell->Serialize(archive);
		exitCell = new ExitCell();
		exitCell->Serialize(archive);

		map[startingCell->getPosY()][startingCell->getPosX()] = static_cast<Cell*>(startingCell);
		map[exitCell->getPosY()][exitCell->getPosX()] = static_cast<Cell*>(exitCell);


		archive >> chestSize >> entitySize >> wallSize >> pathSize;

		if (chestSize != 0)
			for (int a = 0; a < chestSize; a++) {
				Cell* cell = new ChestCell();
				cell->Serialize(archive);
				chestCells.push_back(dynamic_cast<ChestCell*>(cell));
				map[cell->getPosY()][cell->getPosX()] = cell;
			}

		if (entitySize != 0)
			for (int b = 0; b < entitySize; b++) {
				Cell* cell = new EntityCell();
				cell->Serialize(archive);
				entityCells.push_back(dynamic_cast<EntityCell*>(cell));
				map[cell->getPosY()][cell->getPosX()] = cell;
			}

		if (wallSize != 0)
			for (int c = 0; c < wallSize; c++) {
				Cell* cell = new WallCell();
				cell->Serialize(archive);
				wallCells.push_back(dynamic_cast<WallCell*>(cell));
				map[cell->getPosY()][cell->getPosX()] = cell;
			}

		if (pathSize != 0)
			for (int d = 0; d < pathSize; d++) {
				Cell* cell = new PathCell();
				cell->Serialize(archive);
				pathCells.push_back(dynamic_cast<PathCell*>(cell));
				map[cell->getPosY()][cell->getPosX()] = cell;
			}
	}
}

void Map::movePlayer(char side) {
	std::stringstream log;
	log << "Move player to side: " << side;
	Logger::instance()->appendToNewLine(log.str(), "MAP");

	int currentX = playerCell->getPosX();
	int currentY = playerCell->getPosY();
	int newX, newY;

	switch (side) {
	case 'U':
		newX = currentX;
		newY = currentY-1;
		break;
	case 'R': 
		newX = currentX+1;
		newY = currentY;
		break;
	case 'D': 
		newX = currentX;
		newY = currentY+1; 
		break;
	case 'L': 
		newX = currentX-1;
		newY = currentY;
		break;
	}

	playerCell->setPosX(newX);
	playerCell->setPosY(newY);

	map[currentY][currentX] = cachedCell;
	cachedCell = map[newY][newX];
	map[newY][newX] = playerCell;
}

void Map::interact(char side) {
	std::stringstream log;
	log << "Player interact with his side: " << side;
	Logger::instance()->appendToNewLine(log.str(), "MAP");

	int currentX = playerCell->getPosX();
	int currentY = playerCell->getPosY();
	int newX, newY;

	switch (side) {
	case 'U':
		newX = currentX;
		newY = currentY - 1;
		break;
	case 'R':
		newX = currentX + 1;
		newY = currentY;
		break;
	case 'D':
		newX = currentX;
		newY = currentY + 1;
		break;
	case 'L':
		newX = currentX - 1;
		newY = currentY;
		break;
	}

	map[newY][newX]->interact();
}

void Map::startMap(Interactable* fighter) {
	std::stringstream log;
	log << "Set player on map";
	Logger::instance()->appendToNewLine(log.str(), "MAP");

	playerCell = new EntityCell();
	playerCell->setEntity(fighter);
	playerCell->setType(CellHelper::PLAYER_TYPE);

	int posX = startingCell->getPosX();
	int posY = startingCell->getPosY();

	playerCell->setPosX(posX);
	playerCell->setPosY(posY);

	cachedCell = map[posY][posX];
	map[posY][posX] = playerCell;
}

bool Map::getNextCellInteractibility(char side) {
	int currentX = playerCell->getPosX();
	int currentY = playerCell->getPosY();
	int newX, newY;

	switch (side) {
	case 'U':
		newX = currentX;
		newY = currentY - 1;
		break;
	case 'R':
		newX = currentX + 1;
		newY = currentY;
		break;
	case 'D':
		newX = currentX;
		newY = currentY + 1;
		break;
	case 'L':
		newX = currentX - 1;
		newY = currentY;
		break;
	}
	if (!Map::validateOutofBound(newX, newY)) {
		return false;
	}
	return map[newY][newX]->interactable();
}

bool Map::getNextCellWalkability(char side) {
	int currentX = playerCell->getPosX();
	int currentY = playerCell->getPosY();
	int newX, newY;

	switch (side) {
	case 'U':
		newX = currentX;
		newY = currentY - 1;
		break;
	case 'R':
		newX = currentX + 1;
		newY = currentY;
		break;
	case 'D':
		newX = currentX;
		newY = currentY + 1;
		break;
	case 'L':
		newX = currentX - 1;
		newY = currentY;
		break;
	}
	if (!Map::validateOutofBound(newX, newY)) {
		return false;
	}
	return map[newY][newX]->walkable();
}

//! method to validate rather the destination cell is out of bound of the map
//!@param newX: X coordinate for the destination cell
//!@param newY: Y coordinate for the destination cell
//!@return: a boolean representing rather if the destination cell is out of bound (true) or not (false)
bool Map::validateOutofBound(int newX, int newY) {
	if (newX < 0 || newY < 0 || newX > this->width - 1 || newY > this->width - 1) {
		return false;
	}
}

int Map::getNextDirectionX(char direction) {
	int nextX = playerCell->getPosX();

	switch (direction){
	case 'R':
		nextX = nextX + 1;
		break;
	case 'L':
		nextX = nextX - 1;
		break;
	}

	return nextX;
}

int Map::getNextDirectionY(char direction) {
	int nextY = playerCell->getPosY();

	switch (direction) {
	case 'D':
		nextY = nextY + 1;
		break;
	case 'U':
		nextY = nextY - 1;
		break;
	}

	return nextY;
}

int Map::getHeight() {
	return height;
}

int Map::getWidth() {
	return width;
}