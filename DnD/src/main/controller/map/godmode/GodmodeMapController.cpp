//! @file 
//! @brief Implementation file for the PlayModeController
//!

#include "GodmodeMapController.h"
#include "../../interactable/godmode/GodmodeChestController.h"
#include "../../interactable/godmode/GodmodeActiveController.h"
#include "../../../model/interactable/header/Fighter.h"
#include "../../../helper/FileHelper.h"
#include "../../../helper/Logger.h"

//! Controller to receive request from GodmodeMapView::fileOptionsMenuView()
//! @param input: user input
void GodmodeMapController::mapFileSelection(int input) {
	switch (input) {
	case 1:
		GodmodeMapView::mapCreationInputView();
		break;
	case 2:
		s_instance->getSavedMapFiles();
		//s_instance->loadMap();
		break;
	case 3:
		//GameModeView::displayView(3);
		break;
	default: 
		GodmodeMapView::warningMsgInvalidInput();
		GodmodeMapView::fileOptionsMenuView();
		break;
	}
}

//! Controller to receive request to create a new map
//! @param height: height of map
//! @param width: width of map
void GodmodeMapController::newMap(int width, int height) {
	if (width == 0 || height == 0) {
		std::stringstream log;
		log << "Failure to create a new map of size: (" << width << ',' << height << ')';
		Logger::instance()->appendToNewLine(log.str(), "MAP");

		GodmodeMapView::warningMsgInvalidInput();
		GodmodeMapView::mapCreationInputView();
	}
	else {
		std::stringstream log;
		log << "Creating a new map of size: (" << width << ',' << height << ')';
		Logger::instance()->appendToNewLine(log.str(), "MAP");

		if (map != NULL)
			delete map;

		map = new Map(width, height);
		map->print();
		GodmodeMapView::mapOptionsMenuView();
	}
}

//! Controller to receive request to change a cell from the map
//! @param y: y position of the cell
//! @param x: x position of the Cell
//! param charType: type of Cell. For all possible types, see CellHelper object.
void GodmodeMapController::fillCell(int x, int y, char charType, int option) {
	if (x < 0 || x > map->getWidth() || y < 0 || y > map->getHeight()) {
		std::stringstream log;
		log << "Failure to fill cell on map - Invalid cell coordinates: (" << x << ',' << ") - Entity Type: " << charType;
		Logger::instance()->appendToNewLine(log.str(), "MAP");
		
		GodmodeMapView::warningMsgWrongCoordinates();
		GodmodeMapView::mapFillOptionsMenuView();
		return;
	}

	if (charType == NULL) {
		std::stringstream log;
		log << "Failure to fill cell on map - Cell coordinates: (" << x << ',' << ") - Entity Type: INVALID";
		Logger::instance()->appendToNewLine(log.str(), "MAP");

		GodmodeMapView::warningMsgWrongCellType();
		GodmodeMapView::mapFillOptionsMenuView();
		return;
	}

	switch (charType) {
	case CellHelper::WALL_TYPE:
		s_instance->setCell(x, y, new WallCell());
		break;
	case  CellHelper::ENTRANCE_TYPE:
		s_instance->setCell(x, y, new EntranceCell());
		break;
	case  CellHelper::EXIT_TYPE:
		s_instance->setCell(x, y, new ExitCell());
		break;
	case  CellHelper::CHEST_TYPE:
		s_instance->setCell(x, y, new ChestCell());
		break;
	case  CellHelper::ENTITY_TYPE:
		if (option == 1) {
			s_instance->setCell(x, y, new EntityCell(false));
		}
		else if (option == 2) {
			s_instance->setCell(x, y, new EntityCell(true));
		}
		break;
	case  CellHelper::PATH_TYPE:
		s_instance->setCell(x, y, new PathCell());
		break;
	default:
		GodmodeMapView::warningMsgInvalidInput();
		GodmodeMapView::mapFillOptionsMenuView();
		break;
	}
}

//! Controller to receive request to edit the map in some ways
//! @param input: user input
void GodmodeMapController::mapOptions(int input) {
	switch (input) {
	case 1:
		GodmodeMapView::mapFillOptionsMenuView();
		break;
	case 2:
		GodmodeMapController::instance()->validateMap();
		break; 
	case 3:
		GodmodeMapController::instance()->validateSaveRequirements();
		//GodmodeMapController::instance()->saveMap();
		break;
	case 4:
		//GameModeView::displayView(3);
		break;
	default:
		GodmodeMapView::warningMsgInvalidInput();
		GodmodeMapView::mapOptionsMenuView();
		break;
	}
}

//! Controller to receive request to save the map
void GodmodeMapController::saveMap(string filename) {
	std::stringstream log;
	log << "Saving map to file " << filename;
	Logger::instance()->appendToNewLine(log.str(), "MAP");

	CFile theFile;
	string fileDirectory = FileHelper::getDirectoryPath(FileHelper::MAP_FILE_FOLDER) + filename;
	theFile.Open(_T(fileDirectory.c_str()), CFile::modeCreate | CFile::modeWrite);
	CArchive archive(&theFile, CArchive::store);

	map->Serialize(archive);

	archive.Close();
	theFile.Close();

	GodmodeMapView::warningMsgMapSaved();
	GodmodeMapView::mapOptionsMenuView();
}

void GodmodeMapController::validateSaveRequirements() {
	bool valid = map->validateMap();

	if (!valid) {
		GodmodeMapView::warningMsgInvalidMap();
		GodmodeMapView::mapOptionsMenuView();
		return;
	}
	GodmodeMapView::mapAskSaveFileName();
}

//! Controller to receive request to validate the map
void GodmodeMapController::validateMap() {
	bool valid = map->validateMap();

	std::stringstream log;
	log << "Validating map...";
	Logger::instance()->appendToNewLine(log.str(), "MAP");

	if (valid)
		GodmodeMapView::warningMsgValidMap();
	else
		GodmodeMapView::warningMsgInvalidMap();

	GodmodeMapView::mapOptionsMenuView();
}

//! Method called by fillCell, which is turn called by the view. 
void GodmodeMapController::setCell(int x, int y, Cell* cell) {
	std::stringstream log;
	log << "Setting new cell on map: (" << x << "," << y << ") Cell Type: " << cell->getType();
	Logger::instance()->appendToNewLine(log.str(), "MAP");

	switch (cell->getType()) {
		//case CellHelper::WALL_TYPE:
		//	//dynamic_cast<WallCell*>(cell)->set
		//	break;
		//case  CellHelper::ENTRANCE_TYPE:
		//	s_instance->setCell(x, y, new EntranceCell());
		//	break;
		//case  CellHelper::EXIT_TYPE:
		//	s_instance->setCell(x, y, new ExitCell());
		//	break;
	case  CellHelper::CHEST_TYPE:
	{
		//GodmodeChestController::instance()->loadChestWithoutView(); //TODO
		//Interactable* chest = GodmodeChestController::instance()->getChest();
		GodmodeChestController::instance()->getSavedFiles(-1);
		dynamic_cast<ChestCell*>(cell)->setChest(GodmodeChestController::instance()->getChest());
		break;
	}
	case  CellHelper::ENTITY_TYPE:
	{
		EntityCell* ecell = dynamic_cast<EntityCell*>(cell);
		
		if (ecell->getIsFriendly()) {
			GodmodeFriendlyNpcController::instance()->getSavedFiles(-1);
			ecell->setEntity(GodmodeFriendlyNpcController::instance()->getFriendlyNpc());
		}
		else {
			GodmodeActiveController::instance()->getSavedActiveFiles(1, -1);
			ecell->setEntity(GodmodeActiveController::instance()->getActive());
		}

		//GodmodeActiveController::instance()->loadHostileNpcWithoutView();  //TODODO
		//Interactable* entity = GodmodeActiveController::instance()->getActive();
		//dynamic_cast<EntityCell*>(cell)->setEntity(entity);
		break;
	}
	}
	//case  CellHelper::PATH_TYPE:
	//	s_instance->setCell(x, y, new PathCell());
	//	break;
	//}

	map->fillCell(x, y, cell);
	map->print();

	GodmodeMapView::mapOptionsMenuView();
}

//! Controller to receive request to print the map
void GodmodeMapController::print() {
	std::stringstream log;
	log << "Display map";
	Logger::instance()->appendToNewLine(log.str(), "MAP");

	map->print();
}

//! Controller to receive request to load map
void GodmodeMapController::loadMap(int input) {
	if (input < 0 || input >= filenames.size()) {
		GodmodeMapView::mapChooseSaveMapFileView(filenames);
	} else {
		std::stringstream log;
		log << "Loading map from file " << filenames[input];
		Logger::instance()->appendToNewLine(log.str(), "MAP");

		CFile theFile;
		string filePath = FileHelper::getDirectoryPath(FileHelper::MAP_FILE_FOLDER) + filenames[input];
		theFile.Open(_T(filePath.c_str()), CFile::modeRead);
		CArchive archive(&theFile, CArchive::load);

		map = new Map();
		map->Serialize(archive);

		archive.Close();
		theFile.Close();

		print();
		GodmodeMapView::warningMsgMapLoaded();
		GodmodeMapView::mapOptionsMenuView();
	}
}

void GodmodeMapController::getSavedMapFiles() {
	filenames = FileHelper::getFilenamesInDirectory(FileHelper::MAP_FILE_FOLDER);

	GodmodeMapView::mapChooseSaveMapFileView(filenames);
}

GodmodeMapController* GodmodeMapController::instance() {
	if (!s_instance)
		s_instance = new GodmodeMapController();

	return s_instance;
}

void GodmodeMapController::resetController() {
	std::stringstream log;
	log << "Reset map controller";
	Logger::instance()->appendToNewLine(log.str(), "MAP");

	delete map;
	map = NULL;

	filenames.clear();
	vector<string>().swap(filenames);
}

GodmodeMapController* GodmodeMapController::s_instance = GodmodeMapController::instance();