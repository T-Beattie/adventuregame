#include "map.h"

void Map::buildMap()
{
	std::ifstream f("basic_map.json");
	json data = json::parse(f);

	// Set Header information
	map_name = data["map_name"];
	map_description = data["description"];

	// Parse Cells from JSON into individual Cell objects, bind them to the map	
	for (auto dict : data["cells"]) {
		Cell new_cell(
			dict["location"][0],
			dict["location"][1],
			dict["description"],
			dict["event"],
			dict["paths"]
		);
		std::string coords = std::to_string(new_cell.x) + "_" + std::to_string(new_cell.y);
		_map.push_back(new_cell);
		_newMap.insert({ coords,new_cell });
	}	
	int i = 0;
}

const std::string Map::getCellLocation(const int x, const int y)
{
	int index = y * 2 + x;
	Cell current_cell = _map.at(index);
	return current_cell.description;
}

Cell* Map::getCell(int x, int y)
{
	std::string coords = std::to_string(x) + "_" + std::to_string(y);
	//int index = y * 2 + x;
	//if (index <= _map.size())
		//return &_map.at(index);
	if (_newMap.count(coords))
		return &_newMap[coords];
	return nullptr;
}


