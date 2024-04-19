#include "commandengine.h"


CommandEngine::CommandEngine(Player* ref_player, bool* game_running) : CommandEngine()
{
	player = ref_player;
	is_game_running = game_running;

	// load in our action map before starting game
	processActionMap();
}

void CommandEngine::processActionMap()
{
	// Load the json action_map file into the json data object
	std::ifstream f("action_map.json");
	json data = json::parse(f);

	// Loop through each action in the json object and add it to our action_map
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
	// Print out help information for the stuck adventurer

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

std::string CommandEngine::MovePlayer(std::string move_direction)
{
	if (move_direction == "north") 
		return player->Move(north);

	if (move_direction == "south") 
		return player->Move(south);
	
	if (move_direction == "east") 
		return player->Move(east);

	if (move_direction == "west") 
		return player->Move(west);
	
	return "";
}

std::string CommandEngine::ExamineTarget(std::string target)
{
	// check current cells interactable for the target
	bool has_target = player->current_cell->event.interactables.count(target);

	if (has_target) {
		// retrieve a reference to the target interactable object
		Interactable* interactable_object = &player->current_cell->event.interactables[target];

		// Set the players focus to the examined object
		player->focus = interactable_object;

		// if the state of the interactable is 0, return its first description
		if (interactable_object->state == "0" || interactable_object->type == "lever")
			return interactable_object->description["0"];

		// if the state of the interactable is 1 return its 2nd description
		else if (interactable_object->state == "1" && interactable_object->description.contains("1")) 
			return interactable_object->description["1"];

		// if the state of the interactable is 2 return its 2nd description
		else if (interactable_object->state == "2" && interactable_object->description.contains("2")) 		
			return interactable_object->description["2"];
	}
	else 
		return "There is no " + target + " that interests you in this area.";
}

std::string CommandEngine::TakeTarget(std::string target)
{
	// check current cells interactable for the target
	bool has_target = player->current_cell->event.interactables.count(target);

	if (has_target) {
		// retrieve a reference to the target interactable object
		Interactable* interactable_object = &player->current_cell->event.interactables[target];
		// Checks if the object is visible and we can take it
		if (interactable_object->can_take && interactable_object->visible) {
			// transfer the object from the event into the players inventory
			player->inventory[interactable_object->name] = *interactable_object;
			player->current_cell->event.interactables.erase(interactable_object->name);

			return "You take the " + target + " and store it in your bag.";
		}
		else if (!interactable_object->visible) 
			return "There is no " + target + " to take.";
		else
			return "You cant take that!";
	}
	else {
		return "There is no " + target + " to take.";
	}
}

std::string CommandEngine::GiveTarget(std::string target)
{
	// Check the players focus contains an action for the target
	bool target_in_focus = player->focus->give_actions.count(target);
	// Check if the target is in our inventory
	bool target_in_inventory = player->inventory.count(target);

	if (target_in_focus && target_in_inventory) {
		// Store the first target action description in the answer variable
		std::string answer = player->focus->give_actions.at(target)["0"];
		// If the type of interactable is normal and we give it an item we need to increment its state
		if (player->focus->type == "normal") {
			// Check if your focus has a completion state in the event
			bool focus_has_completion_state = player->current_cell->event.completed.count(player->focus->name);
			// If so, increment the state of the focus by 1
			// Else just set the the state to 1
			if (focus_has_completion_state) {
				// The max limit of the targets state
				int completion_max = std::stoi(player->current_cell->event.completed[player->focus->name]);
				// The current state of the target
				int current_state = std::stoi(player->focus->state);
				// If the current state is less than the max, Increment and cast to string to store back in the focused items state
				if (current_state < completion_max) {
					++current_state;
					player->focus->state = std::to_string(current_state);
				}
			}
			else
				player->focus->state = "1";
		}
		// Since we are giving the item over, we have to remove it from our inventory, currently this essentially deletes it from the game
		player->RemoveItemFromInventory(target);
		return answer;
	}
	else if (target_in_focus && !target_in_inventory && player->focus->state == "1") 
		return player->focus->give_actions.at(target)[player->focus->state];
	else 
		return "You can't give " + player->focus->name + " that item";
}

std::string CommandEngine::UseTarget(std::string target)
{
	// Checks if the event has an interactable called target
	bool has_target = player->current_cell->event.interactables.count(target);
	// Checks if the target is an item in our inventory
	bool target_in_inventory = player->inventory.count(target);

	Interactable* interactable = nullptr;
	// If the interactable has the target, and its a normal interactable set our interactable reference to the player focus
	// If its a 'lever' type, set it to the interactable at on the Cells event
	if (has_target) {
		if (player->current_cell->event.interactables.at(target).type == "normal")
			interactable = player->focus;
		else if (player->current_cell->event.interactables.at(target).type == "lever")
			interactable = &player->current_cell->event.interactables.at(target);
	}
	// If the target is also in the inventory, override and set the interactable to the one in the players inventory
	if (target_in_inventory)
		interactable = &player->inventory[target];
	if (interactable != nullptr) {
		bool focus_is_correct = false;
		// If the player is focused on something, check that the focus is on the interactable
		if (player->focus != nullptr)
			focus_is_correct = interactable->use_actions.count(player->focus->name);
		
		if (focus_is_correct) {
			if (interactable->type == "normal") {
				// Retrieve the answer from the iteractables use_actons using the players focused state as a reference
				std::string answer = interactable->use_actions.at(player->focus->name)[player->focus->state];
				// If we use our target on the focus, we check if the focus can give us something in return, if so
				if (interactable->use_actions.at(player->focus->name).count("give")) {
					player->focus->state = "1";
					// Get the item from the focus
					std::string item_name = interactable->use_actions.at(player->focus->name)["give"];
					// Remove the item from the focus as we have it now
					interactable->use_actions.at(player->focus->name).erase("give");
					// Place the item taken from the focus into our inventory
					player->inventory[item_name] = player->current_cell->event.interactables[item_name];
					// Make sure to set the item to visible so we can use it else where
					player->inventory[item_name].visible = true;
					// Remove the item stored in the Cells events interactables list
					player->current_cell->event.interactables.erase(item_name);
				}
				return answer;
			}
		}
		// If there is no focus and interactable is of type 'lever'
		else if (interactable->type == "lever") {

			std::string answer = "You press the switch:\n";
			// Store the interactables use actions from the lever map in our state map
			std::map<std::string, std::string> state_map = interactable->use_actions.at("lever");
			std::map<std::string, std::string>::iterator current_item_iterator;

			// If state_map not empty, get the interactables current state and set it to the current_item_iterator
			if (!state_map.empty()) 
				current_item_iterator = state_map.find(interactable->state);
			
			// If we haven't reached the end of the state_map, increment the current item, and if we have make the current item the starting item
			if (current_item_iterator != state_map.end()) {
				++current_item_iterator;
				if (current_item_iterator == state_map.end()) {
					// Wrap around to the beginning if at the end
					current_item_iterator = state_map.begin();
				}
			}

			// Set the interactables sate to the current items state if we arent at the end
			if (current_item_iterator != state_map.end()) {
				interactable->state = current_item_iterator->first;
			}

			// Return the interactables state description
			return answer + interactable->use_actions.at("lever")[interactable->state];
		}
		else {
			return "You can't use your " + target + " on nothing, try examining an object to focus on it.";
		}
	}
	return "You can't use your " + target + " on nothing, try examining an object to focus on it.";
}

std::string CommandEngine::processCommand(std::string command)
{
	// Find the position of the first space
	size_t pos = command.find(' ');	

	std::string input_action, action;
	std::string input_target, target;

	// Split the string by the first space, putting the first element into input_action and the second into input_target
	// If no split can be found, the whole command goes into input_action
	if (pos != std::string::npos) {
		// Extract substrings
		input_action = command.substr(0, pos);
		input_target = command.substr(pos + 1);
	}
	else {
		input_action = command;
	}

	// now we want to process the action, check the action map for the keys

	// Use lambda functions to iterate and find the current input_Action in our action_map, 
	// checking synonyms, incase a user types in an alternative word
	auto it = std::find_if(std::begin(action_map), std::end(action_map),
		[&input_action](const auto& p)
		{
			return (std::find(p.synonyms.begin(), p.synonyms.end(), input_action) != p.synonyms.end());
		});

	// If we find one before we reach the end of the iterator, make our action equal to the actions actual name
	// Else return that instructions are unclear
	if (it != std::end(action_map))
	{
		action = it->action_name;
	}
	else {
		return "Instructions unclear! remember to use: <Action> <Target>. Type 'help' if you are stuck";
	}

	// For moving only, as its a special case, use lambda functions to iterate and find the current input_target in our action_map["move"], 
	// checking the move possible actons synonyms, incase a user types in an alternative word i.e go up instead of move north
	// Else we just make the target equal to the input_target, assuming interaction over movement
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

	// Grab a map of the current blocked commands, store in "command" : "reason"
	std::map<std::string, std::string> blockers = player->current_cell->event.blocks_command;
	// Check if the Cells event is complete
	bool current_cell_event_complete = isCellCompleted();

	// Combine the action and target so we can check the blocker map 
	std::string full_command = action + "_" + target;
	//  Check if full_command is in the blocker map and if the Cells event isnt complete
	bool is_blocked = blockers.contains(full_command) && !current_cell_event_complete;

	if (action == "inventory") {
		return player->ShowInventory();
	}

	if (action == "quit") {
		*is_game_running = false;
	}

	// Attempt to move, if we are blocked return the reason
	if (action == "move" && !is_blocked) {
		return MovePlayer(target);
	}
	else if (action == "move" && is_blocked) {
		return blockers[full_command];
	}

	if (action == "examine") 
		return ExamineTarget(target);

	if (action == "take") 
		return TakeTarget(target);

	if (action == "give")
		return GiveTarget(target);

	if (action == "use") 
		return UseTarget(target);

	if (action == "help")
		return getHelp();

	return "";
}

const bool CommandEngine::isCellCompleted()
{
	// cycle through current cells completion criteria
	Event current_event = player->current_cell->event;
	for (auto& item : current_event.completed)
	{
		std::string name = item.first;
		std::string state = item.second;

		// Checks each interactable in event to see if the state is equal to the state stored in the completion criteria map
		if (player->current_cell->event.interactables[name].state != state)
			return false;
	}
	// Displays the completed event text if the check passes
	std::cout << current_event.completed_event_text << std::endl;
	return true;
}

