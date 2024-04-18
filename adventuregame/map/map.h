#pragma once
#include <vector>
#include <map>
#include <list>
#include<string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

// for convenience
using json = nlohmann::json;


class Event {
public:
	std::string type;
	bool completed{ false };
	std::map<std::string, std::string> blocks_command;
	std::map<std::string, std::map<std::string, std::string>> examine_actions;
	std::map<std::string, std::string> take_actions;
	std::map<std::string, std::map<std::string, std::map<std::string, std::string>>> use_actions;
	std::map<std::string, std::map<std::string, std::map<std::string, std::string>>> give_actions;

};

class Cell {
public:
	Cell() = default;
	Cell(int x, int y, std::string description, Event event, std::list<std::string> paths) : 
		x(x), y(y), description(description), event(event), paths(paths) {};
	std::string description;
	int x, y;
	Event event;
private:		
	std::list<std::string> paths;
};

class Map {
	
private:
	std::map<std::string, Cell> _map;

public:
	std::string map_name;
	std::string map_description;

	Map(){};
	void buildMap();
	Cell *getCell(int x, int y);
};