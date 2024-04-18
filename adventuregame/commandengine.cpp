#include "commandengine.h"




CommandEngine::CommandEngine(Player* re_player, bool* game_running) : CommandEngine()
{
	player = re_player;
	is_game_running = game_running;
	processActionMap();
}


void CommandEngine::processActionMap()
{
	std::ifstream f("action_map.json");
	json data = json::parse(f);

	for (auto action : data) {
		Action new_action;
		new_action.action_name = action["action_name"];
		new_action.synonyms = action["synonyms"];
		if (action.count("actions")) {
			new_action.actions = action["actions"];
		}		
		action_map.push_back(new_action);
	}
}

void CommandEngine::processCommand(std::string command)
{
	// split this string by the first space only
	size_t pos = command.find(' ');
	std::string input_action, action;
	std::string input_target, target;

	if (pos != std::string::npos) {
		// Extract substrings
		input_action = command.substr(0, pos);
		input_target = command.substr(pos + 1);
	}
	else {
		input_action = command;
	}

	// now we want to process the action, check the action map for the keys

	// find action
	auto it = std::find_if(std::begin(action_map), std::end(action_map),
		[&input_action](const auto& p)
		{
			return (std::find(p.synonyms.begin(), p.synonyms.end(), input_action) != p.synonyms.end());
		});

	if (it != std::end(action_map))
	{
		action = it->action_name;
	}
	else {
		std::cout << "Instructions unclear! remember to use: <Action> <Target> i.e move north" << std::endl;
	}

	// action found, now process target
	if (action == "move") {
		for (auto action : action_map) {
			if (action.action_name == "move") {
				auto it = std::find_if(std::begin(action.actions), std::end(action.actions),
					[&input_target](const auto& p)
					{
						return (std::find(p.second.begin(), p.second.end(), input_target) != p.second.end());
					});

				if (it != std::end(action.actions))
				{
					target = it->first;
				}

				break;
			}
		}	

	}
	else {
		target = input_target;
	}

	// process event here
	// check if event has blocked paths
	// player need to check if it can move by checking the current cells event and seeing if it has blockers

	std::map<std::string, std::string> blockers = player->current_cell->event.blocks_command;
	bool current_cell_event_complete = player->current_cell->event.completed;

	//check blockers for action+target in key 
	std::string full_command = action + "_" + target;

	bool is_blocked = blockers.contains(full_command) && !current_cell_event_complete;


	if (action == "move" && !is_blocked) {
		if (target == "north") {
			player->move(north);
		}
		if (target == "south") {
			player->move(south);
		}
		if (target == "east") {
			player->move(east);
		}
		if (target == "west") {
			player->move(west);
		}
	}
	else if (action == "move" && is_blocked) {
		std::cout << blockers[full_command] << std::endl;
	}

	if (action == "examine") {
		// check examine action in current event
		std::map<std::string, std::map<std::string, std::string>>  examine_map = player->current_cell->event.examine_actions;
		bool has_target = examine_map.contains(target);
		if (has_target) {
			if (!player->current_cell->event.completed) {
				std::cout << examine_map[target]["0"] << std::endl;
			}
			else if(player->current_cell->event.completed && examine_map[target].contains("1")){
				std::cout << examine_map[target]["1"] << std::endl;
			}
			player->focus = target;
		}
		else {
			std::cout << "There is no " << target << " that interests you in this area." << std::endl;
		}
	}

	if (action == "take") {
		//check if target exists in take map
		std::map<std::string, std::string>* take_map = &player->current_cell->event.take_actions;
		bool has_target = take_map->contains(target);
		if (has_target) {
			std::cout << "You take the " << target << " and store it in your bag." << std::endl;
			// transfer the object from the event into the players inventory
			player->inventory[target] = take_map->at(target);
			take_map->erase(target);			
			std::map<std::string, std::map<std::string, std::string>>*examine_map = &player->current_cell->event.examine_actions;
			bool has_examine_target = examine_map->contains(target);
			if (has_examine_target) {
				examine_map->erase(target);
			}
		} else{
			std::cout << "There is no " << target << " to take." << std::endl;
		}
	}

	if (action == "give") {
		std::map<std::string, std::map<std::string, std::map<std::string, std::string>>>* give_map = &player->current_cell->event.give_actions;
		bool has_target = give_map->contains(target);
		bool target_in_inventory = player->inventory.count(target); 

		bool is_item_active;
		std::istringstream(give_map->at(target).at(player->focus)["active"]) >> std::boolalpha >> is_item_active;
		if (has_target && target_in_inventory && !is_item_active) {
			// check focus
			if (give_map->at(target).contains(player->focus)) {
				std::cout << give_map->at(target).at(player->focus)["0"] << std::endl;
				give_map->at(target).at(player->focus)["active"] = "true";
				player->removeItemFromInventory(target);
			}
		}
		else if (has_target && is_item_active) {
			std::cout << give_map->at(target).at(player->focus)["1"] << std::endl;
		}
	}

	if (action == "inventory") {
		player->showInventory();
	}

	if (action == "quit"){
		*is_game_running = false;
	}

	if (action == "use") {
		std::map<std::string, std::map<std::string, std::map<std::string, std::string>>>* use_map = &player->current_cell->event.use_actions;
		bool has_target = use_map->contains(target);
		bool target_in_inventory = player->inventory.count(target);


		if (has_target && target_in_inventory) {
			// check focus
			if (use_map->at(target).contains(player->focus)) {
				if (!player->current_cell->event.completed) {
					std::cout << use_map->at(target).at(player->focus)["0"] << std::endl;
					bool completed;
					std::istringstream(use_map->at(target).at(player->focus)["completes_puzzle"]) >> std::boolalpha >> completed;
					player->current_cell->event.completed = completed;

					if (use_map->at(target).at(player->focus).count("give")) {
						player->inventory[use_map->at(target).at(player->focus)["give"]] = "A useful item";
					}
				}
				else {
					std::cout << use_map->at(target).at(player->focus)["1"] << std::endl;
				}
					
			}
		}
		else if (has_target && !target_in_inventory && use_map->at(target).contains("none")) {

			if (target.find("lever") != std::string::npos) {
				std::cout << "dealing with a lever, lets work it" << '\n';
				// now write in lever logic
			}
		}
		else if (has_target && !target_in_inventory) {
			std::cout << "You do not have the item: " << target << ", in your inventory!" << std::endl;
		}
	}
}

