#pragma once
#include <string>
#include <list>
#include <map>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

#include "characters/player.h"


// for convenience
using json = nlohmann::json;

struct Action {
	std::string action_name;
	std::vector<std::string> synonyms;
	std::map<std::string, std::vector<std::string>> actions;
};

class CommandEngine {
public:
	CommandEngine() = default;
	CommandEngine(Player* player, bool* game_running);
	void processCommand(std::string command);
private:
	void processActionMap();

	Player* player;
	bool* is_game_running;
	std::vector<Action> action_map;

};