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
	//std::cout << "At Location [ " << x << ", " << y << "] - " << current_cell->description << std::endl;
	
	return "At Location - " + current_cell->description;
}
