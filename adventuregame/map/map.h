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

class Cell {
public:
	Cell() = default;
	Cell(int x, int y, std::string description, std::string event, std::list<std::string> paths) : 
		x(x), y(y), description(description), event(event), paths(paths) {};
	std::string description;
	int x, y;
private:	
	std::string event;
	std::list<std::string> paths;
};

class Map {
	
private:
	std::vector<Cell> _map;	
	std::map<std::string, Cell> _newMap;

public:
	std::string map_name;
	std::string map_description;

	Map(){};
	void buildMap();
	const std::string getCellLocation(const int x, const int y);
	Cell *getCell(int x, int y);
};