#include "FileHelper.h"

const string FileHelper::MAP_FILE_FOLDER = "mapfiles";
const string FileHelper::FIGHTER_FILE_FOLDER = "fighterfiles";
const string FileHelper::HOSTILE_FILE_FOLDER = "hostilefiles";
const string FileHelper::FRIENDLY_FILE_FOLDER = "friendlyfiles";
const string FileHelper::CHEST_FILE_FOLDER = "chestfiles";
const string FileHelper::ITEM_CONTAINER_FILE_FOLDER = "itemcontainerfiles";

vector<string> FileHelper::getFilenamesInDirectory(string foldername) {
	vector<string> filenames;

	string currentDir = __FILE__;
	size_t pos = currentDir.find("src\\main\\helper\\filehelper.cpp");
	string correctDir = currentDir.substr(0, pos)+ foldername + "\\*";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(correctDir.c_str(), &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
				filenames.push_back(FindFileData.cFileName);
		} while (FindNextFile(hFind, &FindFileData));

		FindClose(hFind);
	}

	return filenames;
}

string FileHelper::getDirectoryPath(string foldername) {
	string currentDir = __FILE__;
	size_t pos = currentDir.find("src\\main\\helper\\filehelper.cpp");
	string correctDir = currentDir.substr(0, pos) + foldername + "\\";

	return correctDir;
}