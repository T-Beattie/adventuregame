// adventuregame.cpp : Defines the entry point for the application.
//

#include "adventuregame.h"
#include <nlohmann/json.hpp>
#include <fstream>

// for convenience
using json = nlohmann::json;

using namespace std;

int main()
{
	cout << "Hello World" << endl;
	std::ifstream f("example.json");
	json data = json::parse(f);
	std::cout << data.dump(4) << std::endl;
	return 0;
}
