#include "commandengine.h"




CommandEngine::CommandEngine(Player& re_player) : CommandEngine()
{
	player = &re_player;
	processActionMap();
}


int CommandEngine::findCommand(std::string command)
{
	/*auto it = std::find_if(std::begin(action_map), std::end(action_map),
		[&command](const auto& p)
		{
			return (std::find(p.second.begin(), p.second.end(), command) != p.second.end());
		});

	if (it != std::end(action_map))
	{
		return it->first;
	}
	return nothing;*/
	return 0;
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
		//process others
	}

	if (action == "move") {
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
}

