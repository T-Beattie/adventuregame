# Adventure Game

A text based adventure game, where the player traverses through a labyrinth solving puzzles in order to find glory!

## Build
You can build this project in Visual Studio or in the command line with CMake.

**Windows**

Open the project in Visual Studio. It is configured to build using Windows and WSL(if you have this set up) for a Linux build. Just change the target machine, target configuration and hit build.

**CMake**

If you'd prefer to use CMake. The install instructions are below:
```cmake
# Windows Build Commands
cmake -S <path/to/src> -B <path/to/build/directory>
cmake --build <path/to/build/directory> --config Release

# Linux Build Commands
cmake -DCMAKE_BUILD_TYPE=Release -S <path/to/src> -B <path/to/build/directory>
cmake --build <path/to/build/directory>
```

## Installation

**Windows**

Download the latest release from the releases tab. Unzip it and click on the "adventuregame.exe"

**Linux**

Download the latest release from the releases tab. Unzip it and click on the "adventuregame" or run 
``` console
./<path/to/adventuregame>/adventuregame
```

## Usage
The game works off a series of text commands. You will be prompted to enter a command to progress in the game. 
Here are a list of example commands

| Keyword | alternative Keywords | Description | Command Example |
| ------------- | ------------- | ------------- | ------------- |
| move | move, go, travel, walk, run | Lets you move in a N,E,S,W compass direction   | move north, go down  |
| use  | use, apply, pull | Lets you use an object on your focus  | use stone disk, apply rope  |
| examine | examine, inspect, check | Sets that object as your focus  | examine pedestal, check statue  |
| give | give, place | Attempts to give your item to the focus  | give rock, place jewel |
| take | take, grab | Take an item you find and put it in your inventory  | take helm, grab wand  |
| inventory | invention, inv, bag | Displays your inventory   | inventory, inv  |
| quit | quit, exit | Exits the game  | quit  |
| help | help | Displays this information   | help  |

## Ideal Updates 
A list of features/updates that would be implemented if I could dedicate more time to the project

1. Rework the basic_map.json -  It is quite bulky and would get more bulky as time went on. I'm sure there are more elegant ways to store the data and nicer data structure to use
2. Rework how the messages display on screen - Currently I dont believe its the most effective way, passing the results of the command engine back to the main loop. There is a lot of potential for missed messages. Maybe an answer builder singleton
3. More Intelligent commands - During playtesting it was found that users will try and type anything and everything. Currently the commands are very ridgid especially when using or giving items, if you dont type the name as it is coded it wont work. I'd like to extend the command engine to be able to interpret what the user is inputting and either selecting what it thinks the user is refering too or offering a choice of what it thinks. This includes adjusting the map to have alternate naming for items.
4. A more extensive command structure -  Currently it only takes two arguments as a command, I'd like to extend it to include arguments for extended functionality, such as use <target> <object>.
5. Adjust the descriptions of each Cell to be dynamic, where if the user alters the state of the Cell its reflected in the description.
6. Implement Characters - add characters that you could have conversations with and get clues on where to go and what to do.
7. An RPG system - Add in health and experience, with stats. Complete with a fighting system with monsters.

   
## License

[MIT](https://choosealicense.com/licenses/mit/)
