#pragma once
#include <vector>
#include <map>
#include <list>
#include<string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "interactable.h"

// for convenience
using json = nlohmann::json;


class Event {
public:
	std::string type;
	std::map<std::string, std::string> completed;
	std::map<std::string, std::string> blocks_command;
	std::map<std::string, Interactable> interactables;

};

class Cell {
public:
	Cell() = default;
	Cell(int x, int y, std::string title, std::string description, Event event, std::list<std::string> paths) : 
		x(x), y(y), title(title), description(description), event(event), paths(paths) {};
	std::string description;
	std::string title;
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
	int start_x{ 0 };
	int start_y{ 0 };

	Map(){};
	void buildMap();
	Cell *getCell(int x, int y);
};