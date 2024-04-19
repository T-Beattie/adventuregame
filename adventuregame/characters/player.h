#pragma once
#include "character.h"
#include "../map/map.h"

enum direction {
	north,
	south,
	east,
	west
};

class Player : public Character {
public:
	Player() = delete;
	Player(std::string name, std::string description, Map map) : Character(name, description), _map(map) { 
		x = map.start_x;
		y = map.start_y;
		current_cell = _map.getCell(x, y); 		
	};
	std::string move(direction direction);
	std::string getLocation();
	const std::string showInventory();
	void removeItemFromInventory(std::string item);

	Interactable* focus = nullptr;
	Cell* current_cell;
	std::map<std::string, Interactable> inventory;

private:
	int x{ 0 }, y{ 0 };
	Map _map;
};