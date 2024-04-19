#ifndef MAP_INTERACTABLE_H_
#define MAP_INTERACTABLE_H_
#include<vector>
#include<map>
#include<string>

class Interactable {
public:
	std::string name;
	std::string type;															// Currently can be "normal" or "lever"
	std::map<std::string, std::string> description;								// Description can change dependent on state <state, description>
	std::string state;															// State can be any int or string - used to adjust the interactable
	bool can_take{ false };														// Check if you can put interactable in your inventory
	bool visible{ true };														// Check if interactable is visible or needs to be interacted with in a certain way

	std::map<std::string, std::map<std::string, std::string>> give_actions;		// Any way we can give this interactable to something is stored here
	std::map<std::string, std::map<std::string, std::string>> use_actions;		// Any way we can use this interactable on something is stored here
};
#endif MAP_INTERACTABLE_H_