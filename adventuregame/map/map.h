#ifndef MAP_MAP_H_
#define MAP_MAP_H_

#include <map>
#include<string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "interactable.h"

using json = nlohmann::json;

// A class to hold a Cells event
class Event {
public:
	std::string type;										// Event type
	std::map<std::string, std::string> completed;			// Complete conditions for the event
	std::map<std::string, std::string> blocks_command;		// What commands are blocked until the event is completed
	std::map<std::string, Interactable> interactables;		// What interactable objects are in the event
	std::string completed_event_text;						// The completion message for the event
};

// Cell holds information on the cells title and description to display to screen. It also holds the positional data and paths the player can take from the cell
class Cell {
public:
	Cell() = default;
	Cell(int x, int y, std::string title, std::string description, Event event, std::list<std::string> paths) : 
		x(x), y(y), title(title), description(description), event(event), paths(paths) {};
	std::string description;	
	std::string title;
	int x, y;												// Positional coordinate data of the Cell
	Event event;											// The event tied to the Cell
private:		
	std::list<std::string> paths;							// Possible paths to travel down
};

// A class to store the map data which is a map of Cells and the players start position
class Map {	
private:
	std::map<std::string, Cell> _map;						// A private map of all Cells in the map

public:
	std::string map_name;
	std::string map_description;
	int start_x{ 0 };										// Initial X start position on game start for the player
	int start_y{ 0 };										// Initial Y start position on game start for the player

	Map(){};
	void BuildMap();										// Method to populate the variables in the class
	Cell *GetCell(int x, int y);							// Method to return a reference to the cell at [x, y]
};

#endif MAP_MAP_H_