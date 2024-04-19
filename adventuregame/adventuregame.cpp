// adventuregame.cpp : Defines the entry point for the application.

#include "adventuregame.h"

using namespace std;

void wrap(std::string const& input, size_t width, std::ostream& os, size_t indent = 0) {
	std::istringstream in(input);

	os << std::string(indent, ' ');
	size_t current = indent;
	std::string word;

	while (in >> word) {
		if (current + word.size() > width) {
			os << "\n" << std::string(indent, ' ');
			current = indent;
		}
		os << word << ' ';
		current += word.size() + 1;
	}
}

void CompleteGame(Player* player, CommandEngine* ce) {
	// Testing
	player->move(north);
	player->move(east);
	player->move(east);
	player->move(north);
	player->move(north);
	ce->processCommand("take knife");
	player->move(north);
	ce->processCommand("use left switch");
	ce->processCommand("use left switch");
	ce->processCommand("use left switch");
	ce->processCommand("use right switch");
	player->move(north);
	ce->processCommand("take relic");
	player->move(south);
	player->move(west);
	player->move(west);
	player->move(west);
	player->move(south);
	player->move(west);
	ce->processCommand("examine pillar");
	ce->processCommand("use knife");
	player->move(east);
	player->move(north);
	player->move(north);
}

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
	

	CompleteGame(&player, &commandEngine);

	//system("CLS");

	int console_width = 100; 
	std::string answer{ "" };
	std::cout << std::setfill('*') << std::setw(console_width) << '*' << std::endl;
	wrap(new_map.map_description, console_width, std::cout, 0);
	std::cout << std::endl;
	std::cout << std::setfill('*') << std::setw(console_width) << '*' << std::endl;
	bool main_title_shown{ false };

	while (game_running) {
		if(main_title_shown)
			system("CLS");
		int half_string_length = player.current_cell->title.length() / 2;
		int half_output_width = console_width / 2;

		int leading_spaces = half_output_width - half_string_length;

		std::cout << std::setfill('*') << std::setw(console_width) << '*' << std::endl;
		std::cout << std::setfill(' ') << std::setw(leading_spaces) << player.current_cell->title << std::endl;
		std::cout << std::setfill('*') << std::setw(console_width) << '*' << std::endl;
		
		//std::cout << std::left << std::setw(80 ) << player.current_cell->description << std::endl;
		wrap(player.current_cell->description, console_width, std::cout, 0);
		std::cout << "\n\n" << std::setfill('*') << std::setw(console_width) << '*' << "\n\n" << std::endl;

		if (!answer.empty() && answer[0] != '~') {
			wrap(answer, console_width, std::cout, 0);
			std::cout << "\n\n" << std::setfill('-') << std::setw(console_width) << '-' << "\n\n" << std::endl;
		}
		else if (!answer.empty() && answer[0] == '~') {
			
			std::cout << answer << std::endl;
			std::cout << std::setfill('-') << std::setw(console_width) << '-' << std::endl;
		}

		std::string input;
		std::cout << "What would you like to do?\n";
		std::getline(std::cin, input);
		answer = commandEngine.processCommand(input);	
		main_title_shown = true;
	}
	
	return 0;
}
