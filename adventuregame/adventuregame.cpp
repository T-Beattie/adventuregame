// adventuregame.cpp : Defines the entry point for the application.

#include "adventuregame.h"

using namespace std;

int main()
{
	Map new_map;
	new_map.buildMap();	

	/*player.getLocation();
	player.move(east);
	player.getLocation();
	player.move(north);
	player.getLocation();
	player.move(west);
	player.getLocation();

	player.move(north);
	player.getLocation();*/
	//std::cout << "Welcome, What is your name?" << std::endl;
	std::string name;

	//std::cin >> name;
	bool game_running{ true };
	Player player("name", "Fun", new_map);
	CommandEngine commandEngine(&player, &game_running);
	
	int location {0};
	

	//system("CLS");

	while (game_running) {
		std::string input;
		std::cout << "What would you like to do?\n";
		std::getline(std::cin, input);
		commandEngine.processCommand(input);			
	}
	
	return 0;
}
