#include "player.h"

// Attempts to move the player in a certain direction. Will not if the direction doesnt contain a Cell
std::string Player::Move(direction direction)
{
	int temp_x = x;
	int temp_y = y;

	// set our new coordinates
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

	// Check to see if there is a cell where we want to go
	Cell* potential_cell = _map->GetCell(temp_x, temp_y);
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

// Displays the contents of the players inventory on screen
const std::string Player::ShowInventory()
{
	std::string output_string{};
	output_string += "~~~~~~~~~~~~~~~~~~~~~~\n";
	output_string += "~      Inventory     ~\n";
	output_string += "~~~~~~~~~~~~~~~~~~~~~~\n";

	int count = 1;
	std::map<std::string, Interactable>::iterator it;
	for (it = inventory.begin(); it != inventory.end(); it++)
	{
		output_string += count + ". "  + it->first + " - " + it->second.description["0"] + "\n";
		++count;
	}
	output_string += "---------------------\n";

	return output_string;
}

void Player::RemoveItemFromInventory(std::string item)
{
	inventory.erase(item);
}
