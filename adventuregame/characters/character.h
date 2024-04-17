#pragma once
#include <vector>
#include <string>

class Character {
public:
	Character(std::string name, std::string description) : name(name), description(description) {};
	const std::string getName() { return name; }
private:
	std::string name;
	std::string description;
	int health{ 10 };
	std::vector<std::string> inventory;
};