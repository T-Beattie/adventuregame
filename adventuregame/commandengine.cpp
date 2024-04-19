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

std::string CommandEngine::getHelp()
{
	std::string help_string{ "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Commands Help ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"};
	help_string += "       Keyword ( alternatives )		|                    Description                       |      Example\n";
	help_string += "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	help_string += "move (move, go, travel, walk, run)  | Lets you move in a N,E,S,W compass direction         | move north\n";
	help_string += "----------------------------------------------------------------------------------------------------------------\n";
	help_string += "use (use, apply, pull)              | Lets you use an object on your focus                 | use stone disk\n";
	help_string += "----------------------------------------------------------------------------------------------------------------\n";
	help_string += "examine (examine, inspect, check)   | Sets that object as your focus                       | examine pedestal\n";
	help_string += "----------------------------------------------------------------------------------------------------------------\n";
	help_string += "give (give, place)                  | Attempts to give your item to the focus              | give rock\n";
	help_string += "----------------------------------------------------------------------------------------------------------------\n";
	help_string += "take (take, grab)                   | Take an item you find and put it in your inventory   | take helm\n";
	help_string += "----------------------------------------------------------------------------------------------------------------\n";
	help_string += "inventory (invention, inv, bag)     | Displays your inventory                              | inventory\n";
	help_string += "----------------------------------------------------------------------------------------------------------------\n";
	help_string += "quit (quit, exit)                   | Exits the game                                       | quit\n";
	help_string += "----------------------------------------------------------------------------------------------------------------\n";
	help_string += "help (help)                         | Displays this information                            | help\n";
	return help_string;
}

std::string CommandEngine::processCommand(std::string command)
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
		return "Instructions unclear! remember to use: <Action> <Target> i.e move north";
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
	bool current_cell_event_complete = isCellCompleted();

	//check blockers for action+target in key 
	std::string full_command = action + "_" + target;

	bool is_blocked = blockers.contains(full_command) && !current_cell_event_complete;


	if (action == "inventory") {
		return player->showInventory();
	}

	if (action == "quit") {
		*is_game_running = false;
	}

	if (action == "move" && !is_blocked) {
		if (target == "north") {
			return player->move(north);
		}
		if (target == "south") {
			return player->move(south);
		}
		if (target == "east") {
			return player->move(east);
		}
		if (target == "west") {
			return player->move(west);
		}
	}
	else if (action == "move" && is_blocked) {
		return blockers[full_command];
	}

	if (action == "examine") {
		// check current cells interactab for the target

		bool has_target = player->current_cell->event.interactables.count(target);
		if (has_target) {
			Interactable* interactable_object = &player->current_cell->event.interactables[target];
			if (interactable_object->state == "0") {
				player->focus = interactable_object;
				return interactable_object->description["0"];
			}
			else if(interactable_object->state == "1" && interactable_object->description.contains("1")) {
				player->focus = interactable_object;
				return interactable_object->description["1"] ;
			}
			else if (interactable_object->state == "2" && interactable_object->description.contains("2")) {
				player->focus = interactable_object;
				return interactable_object->description["2"];
			}
			else if(interactable_object->type == "lever") {
				player->focus = interactable_object;
				return interactable_object->description["0"];
			}
		}
		else {
			return "There is no " + target + " that interests you in this area.";
		}
	}

	if (action == "take") {
		//check if target exists in take map
		bool has_target = player->current_cell->event.interactables.count(target);
		if (has_target) {
			Interactable* interactable_object = &player->current_cell->event.interactables[target];
			if (interactable_object->can_take && interactable_object->visible) {			

				// transfer the object from the event into the players inventory
				player->inventory[interactable_object->name] = *interactable_object;
				player->current_cell->event.interactables.erase(interactable_object->name);

				return "You take the " + target + " and store it in your bag.";
			}
			else if(!interactable_object->visible){
				return "There is no " + target + " to take.";
			}
			else
			{
				return "You cant take that!";
			}
			
		} else{
			return "There is no " + target +" to take.";
		}
	}

	if (action == "give") { 
		// check if target is in focus and in inventory
		bool target_in_focus = player->focus->give_actions.count(target);
		bool target_in_inventory = player->inventory.count(target);

		if (target_in_focus && target_in_inventory) {
			// display item 0 and change object state to 1
			std::string answer = player->focus->give_actions.at(target)["0"];
			if (player->focus->type == "normal") {
				// we know the state is an int.
				// check completion state, if has completion state allow to increment, else its 1.
				// make sure focus is less than or equal to completion state
				bool focus_has_completion_state = player->current_cell->event.completed.count(player->focus->name);
				if (focus_has_completion_state) {
					int completion_max = std::stoi(player->current_cell->event.completed[player->focus->name]);
					int current_state = std::stoi(player->focus->state);
					if (current_state < completion_max) {
						++current_state;
						player->focus->state = std::to_string(current_state);
					}
				}
				else
					player->focus->state = "1";
			}
			player->removeItemFromInventory(target);
			return answer;
		}else if (target_in_focus && !target_in_inventory && player->focus->state == "1") {
			return player->focus->give_actions.at(target)[player->focus->state];
		}
		else {
			return "You can't give " + player->focus->name + " that item" ;
		}
	}

	if (action == "use") {
		// check if focus in targets use commands
		bool has_target = player->current_cell->event.interactables.count(target);
		bool target_in_inventory = player->inventory.count(target);
		Interactable* interactable = nullptr;
		if (has_target) {
			if (player->current_cell->event.interactables.at(target).type == "normal")
				interactable = player->focus;
			else if (player->current_cell->event.interactables.at(target).type == "lever")
				interactable = &player->current_cell->event.interactables.at(target);
		}
		if (target_in_inventory)
			interactable = &player->inventory[target];
		if (interactable != nullptr) {
			bool focus_is_correct = false;
			if(player->focus != nullptr)
				focus_is_correct =interactable->use_actions.count(player->focus->name);
			if (focus_is_correct) {
				if (interactable->type == "normal") {
					std::string answer = interactable->use_actions.at(player->focus->name)[player->focus->state];
					if (interactable->use_actions.at(player->focus->name).count("give")) {
						player->focus->state = "1";
						std::string item_name = interactable->use_actions.at(player->focus->name)["give"];
						interactable->use_actions.at(player->focus->name).erase("give");
						player->inventory[item_name] = player->current_cell->event.interactables[item_name];
						player->inventory[item_name].visible = true;
						player->current_cell->event.interactables.erase(item_name);
					}
					return answer;
				}				
			}
			else if (interactable->type == "lever") {

				std::string answer = "You pull the lever:\n";
				// cycle to next key here:
				// get map keys as list. find index of current state in list, increment state by 1
				std::map<std::string, std::string> state_map = interactable->use_actions.at("lever");
				std::map<std::string, std::string>::iterator current_item_iterator;

				if (!state_map.empty()) {
					current_item_iterator = state_map.find(interactable->state);
				}

				if (current_item_iterator != state_map.end()) {
					++current_item_iterator;
					if (current_item_iterator == state_map.end()) {
						// Wrap around to the beginning if at the end
						current_item_iterator = state_map.begin();
					}
				}

				if (current_item_iterator != state_map.end()) {
					interactable->state = current_item_iterator->first;
				}

				return answer + interactable->use_actions.at("lever")[interactable->state];
			}
			else {
				return "You can't use your " + target + " on nothing, try examining an object to focus on it.";
			}
		}
	}
	if (action == "help")
		return getHelp();

	return "";
}

const bool CommandEngine::isCellCompleted()
{
	// cycle through current cells completion criteria and check the state of things
	Event current_event = player->current_cell->event;
	for (auto& item : current_event.completed)
	{
		std::string name = item.first;
		std::string state = item.second;

		if (player->current_cell->event.interactables[name].state != state)
			return false;
	}
	std::cout << current_event.completed_event_text << std::endl;
	return true;
}

