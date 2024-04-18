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
		// build up the event

		Event new_event;		

		if (!dict["event"].empty()) {
			auto event = dict["event"];
			new_event.type = event["type"];
			new_event.completed = event["completed"];
			new_event.blocks_command = event["blocks_commands"];

			new_event.examine_actions = event["actions"]["examine"];
			new_event.take_actions = event["actions"]["take"];
			new_event.use_actions = event["actions"]["use"];	
			new_event.give_actions = event["actions"]["give"];
		}

		Cell new_cell(
			dict["location"][0],
			dict["location"][1],
			dict["description"],
			new_event,
			dict["paths"]
		);
		std::string coords = std::to_string(new_cell.x) + "_" + std::to_string(new_cell.y);
		_map.insert({ coords,new_cell });
	}	
	int i = 0;
}

Cell* Map::getCell(int x, int y)
{
	std::string coords = std::to_string(x) + "_" + std::to_string(y);
	//int index = y * 2 + x;
	//if (index <= _map.size())
		//return &_map.at(index);
	if (_map.count(coords))
		return &_map[coords];
	return nullptr;
}


