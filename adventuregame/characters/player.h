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
	Player(std::string name, std::string description, Map map) : Character(name, description), _map(map) { current_cell = _map.getCell(x, y); };
	void move(direction direction);
	std::string getLocation();
	const void showInventory();

	std::string focus;
	Cell* current_cell;
	std::vector<std::string> inventory;

private:
	int x{ 0 }, y{ 0 };
	Map _map;
};