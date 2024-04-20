// adventuregame.cpp : Defines the entry point for the application.

#include "adventuregame.h"

using namespace std;

// Define CLEAR for use on windows and linux systems so we can clear the screen
#ifdef _WIN32
#define CLEAR "cls"
#else //In any other OS
#define CLEAR "clear"
#endif

// Wrap will wrap the input round a set width on the console, to make it print nice
static void Wrap(std::string const& input, size_t width, std::ostream& os, size_t indent = 0) {
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

static void CompleteGame(Player* player, CommandEngine* ce) {
	// Testing
	player->Move(north);
	player->Move(east);
	player->Move(east);
	player->Move(south);
	ce->processCommand("take stone disk");
	player->Move(north);
	player->Move(north);
	player->Move(north);
	ce->processCommand("take knife");
	player->Move(north);
	ce->processCommand("use left switch");
	ce->processCommand("use left switch");
	ce->processCommand("use left switch");
	ce->processCommand("use right switch");
	player->Move(north);
	ce->processCommand("take left relic");
	player->Move(south);
	player->Move(west);
	player->Move(west);
	player->Move(west);
	player->Move(south);
	player->Move(west);
	ce->processCommand("examine pillar");
	ce->processCommand("use knife");
	player->Move(east);
	player->Move(north);
	ce->processCommand("examine pedestal");
	ce->processCommand("give stone disk");
	player->Move(north);
	ce->processCommand("examine door");
	ce->processCommand("give left relic");
	ce->processCommand("give right relic");
	player->Move(west);
}

int main()
{
	Map new_map;
	new_map.BuildMap();	

	bool game_running{ true };

	Player player("Bob", "An Adventurer", &new_map);
	CommandEngine commandEngine(&player, &game_running);	

	//CompleteGame(&player, &commandEngine);                                   // Uncomment this to do a complete run through of the game without input

	// Display the starting Message for the map
	int console_width = 100; 
	std::string answer{ "" };
	std::cout << std::setfill('*') << std::setw(console_width) << '*' << std::endl;
	Wrap(new_map.map_description, console_width, std::cout, 0);
	std::cout << std::endl;
	std::cout << std::setfill('*') << std::setw(console_width) << '*' << std::endl;
	bool main_title_shown{ false };

	// Main Game loop
	while (game_running) {
		// Show the main title description only at the start of the game
		if (main_title_shown)
			system(CLEAR);

		// Work out the spacing to have the current cells title in the middle of the console
		int half_string_length = player.current_cell->title.length() / 2;
		int half_output_width = console_width / 2;

		int leading_spaces = half_output_width - half_string_length;

		// Display current cells title
		std::cout << std::setfill('*') << std::setw(console_width) << '*' << std::endl;
		std::cout << std::setfill(' ') << std::setw(leading_spaces) << player.current_cell->title << std::endl;
		std::cout << std::setfill('*') << std::setw(console_width) << '*' << std::endl;

		// Display current cells description                
		Wrap(player.current_cell->description, console_width, std::cout, 0);
		std::cout << "\n\n" << std::setfill('*') << std::setw(console_width) << '*' << "\n\n" << std::endl;

		// Display users answer, check if the first char contains a '~' char, if so do not use the wrap function
		if (!answer.empty() && answer[0] != '~') {
			Wrap(answer, console_width, std::cout, 0);
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
		// Disable the main title after the first processed input
		main_title_shown = true;
	}
	
	return 0;
}
