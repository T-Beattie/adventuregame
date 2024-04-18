#include "player.h"

void Player::move(direction direction)
{
	int temp_x = x;
	int temp_y = y;
	switch (direction)
	{
	case north:
		++temp_y;
		break;
	case south:
		--temp_y;
		break;
	case east:
		++temp_x;
		break;
	case west:
		--temp_x;
		break;
	default:
		break;
	}
	// need to check if can go here first!
	Cell *potential_cell = _map.getCell(temp_x, temp_y);
	if (potential_cell != nullptr) {
		x = temp_x;
		y = temp_y;
		current_cell = potential_cell;
	}		
	else
		std::cout << "There is no path in this direction, you must travel another way!" << std::endl;

	std::cout << getLocation() << std::endl;
}

std::string Player::getLocation()
{	
	return "At Location - " + current_cell->description;
}

const void Player::showInventory()
{
	std::cout << "---------------------\n";
	std::cout << "-     Inventory     -\n";
	std::cout << "---------------------\n";

	int count = 1;
	std::map<std::string, std::string>::iterator it;
	for (it = inventory.begin(); it != inventory.end(); it++)
	{
		std::cout << count << ". "  << it->first << " - " << it->second << std::endl;
		++count;
	}
	std::cout << "---------------------\n" << std::endl;
}

void Player::removeItemFromInventory(std::string item)
{
	inventory.erase(item);
}
