#ifndef COMMANDENGINE_H_
#define COMMANDENGINE_H_

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "characters/player.h"

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
	std::string processCommand(std::string command);
	const bool isCellCompleted();

private:
	void processActionMap();
	std::string getHelp();
	std::string MovePlayer(std::string move_direction);
	std::string ExamineTarget(std::string target);
	std::string TakeTarget(std::string target);
	std::string GiveTarget(std::string target);
	std::string UseTarget(std::string target);

	Player* player;
	bool* is_game_running;
	std::vector<Action> action_map;

};

#endif COMMANDENGINE_H_