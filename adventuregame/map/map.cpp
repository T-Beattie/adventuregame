#include "map.h"

void Map::BuildMap()
{
	// Load in the json map into a stream and parse it to a json object
	std::ifstream f("basic_map.json");
	json data = json::parse(f);

	// Set Header information
	map_name = data["map_name"];
	map_description = data["description"];
	start_x = data["start_location"][0];
	start_y = data["start_location"][1];

	// Parse Cells from JSON into individual Cell objects, bind them to the map	
	for (auto dict : data["cells"]) {
		// build up the event for the Cell
		Event cell_event;		

		// check if the json contains an event for this cell, if not insert an empty event
		if (!dict["event"].empty()) {
			auto event = dict["event"];

			// Populate the events header data
			cell_event.type = event["type"];
			cell_event.completed = event["completed"];
			cell_event.completed_event_text = event["completed_text"];
			cell_event.blocks_command = event["blocks_commands"];

			// Build up an interactables map for the event
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

				cell_event.interactables[new_interactable.name] = new_interactable;
			}
		}

		// Create a new Cell with the contents of the cell_event
		Cell new_cell(			
			dict["location"][0],
			dict["location"][1],
			dict["title"],
			dict["description"],
			cell_event,
			dict["paths"]
		);
		// retrieve the coordinates from the Cell to serve as a look up key for the map
		std::string coords = std::to_string(new_cell.x) + "_" + std::to_string(new_cell.y);
		_map.insert({ coords,new_cell });
	}	
}

// Given a int X and Y return a reference to a Cell within the Map
Cell* Map::GetCell(int x, int y)
{
	std::string coords = std::to_string(x) + "_" + std::to_string(y);

	if (_map.count(coords))
		return &_map[coords];
	return nullptr;
}


