#pragma once
#include <vector>
#include <map>
#include <string>

class Character {
public:
	Character(std::string name, std::string description) : name(name), description(description) {};
	const std::string getName() { return name; }
private:
	std::string name;
	std::string description;
	int health{ 10 };
	std::map<std::string,std::string> inventory;
};