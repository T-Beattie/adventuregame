# Adventure Game

A text based adventure game, where the player traverses through a labyrinth solving puzzles in order to find glory!

## Build

Currently the best way to build the windows version is to open up this project in visual studio and build one of the presets

## Installation

Download the latest release from the releases tab. Unzip it and click on the "adventuregame.exe"

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
A list of features/updates that would be implemented if I could dedicate more time

1. Rework the basic_map.json -  It is quite bulky and would get more bulky as time went on. I'm sure there are more elegant ways to store the data and nicer data structure to use
2. Rework how the messages display on screen - Currently I dont believe its the most effective way, passing the results of the command engine back to the main loop. There is a lot of potential for missed messages. Maybe an answer builder singleton
3. A more extensive command structure -  Currently it only takes two arguments as a command, I'd like to extend it to include arguments for extended functionality, such as use <target> <object>
4. Implement Characters - add characters that you could have conversations with and get clues on where to go and what to do
5. An RPG system - Add in health and experience, with stats. Complete with a fighting system with monsters   
   

## License

[MIT](https://choosealicense.com/licenses/mit/)
