#include "player.h"

std::string Player::move(direction direction)
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
		focus = nullptr;
		return "";
	}		
	else
		return "There is no path in this direction, you must travel another way!";
}

std::string Player::getLocation()
{	
	return "At Location - " + current_cell->description;
}

const std::string Player::showInventory()
{
	std::string ostr;
	ostr += "~~~~~~~~~~~~~~~~~~~~~~\n";
	ostr += "~      Inventory     ~\n";
	ostr += "~~~~~~~~~~~~~~~~~~~~~~\n";

	int count = 1;
	std::map<std::string, Interactable>::iterator it;
	for (it = inventory.begin(); it != inventory.end(); it++)
	{
		ostr += count + ". "  + it->first + " - " + it->second.description["0"] + "\n";
		++count;
	}
	ostr += "---------------------\n";

	return ostr;
}

void Player::removeItemFromInventory(std::string item)
{
	inventory.erase(item);
}
