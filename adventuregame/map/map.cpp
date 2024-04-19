#include "map.h"

void Map::buildMap()
{
	std::ifstream f("basic_map.json");
	json data = json::parse(f);

	// Set Header information
	map_name = data["map_name"];
	map_description = data["description"];
	start_x = data["start_location"][0];
	start_y = data["start_location"][1];

	// Parse Cells from JSON into individual Cell objects, bind them to the map	
	for (auto dict : data["cells"]) {
		// build up the event

		Event new_event;		

		if (!dict["event"].empty()) {
			auto event = dict["event"];
			new_event.type = event["type"];
			//new_event.completed = event["completed"];
			new_event.completed_event_text = event["completed_text"];
			new_event.blocks_command = event["blocks_commands"];

			for (auto& item : event["interactables"].items())
			{
				Interactable new_interactable;
				new_interactable.name = item.key();
				new_interactable.type = item.value()["type"];
				new_interactable.description = item.value()["description"];
				new_interactable.state = item.value()["state"];
				new_interactable.visible = item.value()["visible"];
				new_interactable.can_take = item.value()["can_take"];
				if (item.value().count("give")) 
					new_interactable.give_actions = item.value()["give"];
				if (item.value().count("use"))
					new_interactable.use_actions = item.value()["use"];

				new_event.interactables[new_interactable.name] = new_interactable;
			}
		}

		Cell new_cell(			
			dict["location"][0],
			dict["location"][1],
			dict["title"],
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


