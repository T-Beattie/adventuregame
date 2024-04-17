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
	CommandEngine(Player& player);
	void processCommand(std::string command);
private:
	int findCommand(std::string command);
	void processActionMap();

	Player* player;
	std::vector<Action> action_map;

};