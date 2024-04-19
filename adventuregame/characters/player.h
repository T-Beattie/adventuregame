#ifndef CHARACTER_PLAYER_H_
#define CHARACTER_PLAYER_H_

#include "character.h"
#include "../map/map.h"

// Enum for simple movement directions
enum direction {
	north,
	south,
	east,
	west
};

class Player : public Character {
public:
	Player() = default;
	// Constructor to take in a reference to the map so the player can understand where it is
	Player(std::string name, std::string description, Map* map) : Character(name, description), _map(map) { 
		x = map->start_x;
		y = map->start_y;
		current_cell = _map->GetCell(x, y); 									// sets the current cell to be the starting cell
	};
	std::string Move(direction direction);
	const std::string ShowInventory();											// read only method to display the inventory
	void RemoveItemFromInventory(std::string item); 

	Interactable* focus = nullptr;												// A pointer to the object the player is focusing on
	Cell* current_cell;															// A pointer to the current cell the player is in
	std::map<std::string, Interactable> inventory;

private:
	int x{ 0 }, y{ 0 };
	Map* _map;
};
#endif CHARACTER_PLAYER_H_