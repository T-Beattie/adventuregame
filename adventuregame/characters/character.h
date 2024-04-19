#ifndef CHARACTER_CHARACTER_H_
#define CHARACTER_CHARACTER_H_

#include <map>
#include <string>

class Character {
public:
	Character(std::string name, std::string description) : name(name), description(description) {};
	const std::string name;
	const std::string description;					// Character description
private:

	int health{ 10 };
	std::map<std::string,std::string> inventory;	// Inventory of Item - Description
};
#endif CHARACTER_CHARACTER_H_