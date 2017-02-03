Assignment 1
Morgan Roff, 10044329

NetID: 11mr60

Major Game Modifications:
  -  Addition of start and end game messages: I added the TextGenerator class with a simple static method for displaying text at the specified x,y location in the world. The x,y coordinates are in world coordinates, and specify the location to start outputting the first character. This was based on Professor James Stewart's work in demo7.cpp. The game now opens with a "GOOD LUCK" message, and upon destruction of all buildings, it ends with "GAME OVER" and displays how long the player survived (a basic form of "score" for the user). See figures good-luck.png and game-over.png.
  -  Modifications to silos: Silos now receive 5 additional missiles every 7 seconds, to enable a player to survive for longer, but still limit them from continuously firing. A number is displayed on each silo indicating how many missiles it currently has available. Each silo also starts with 15 health, which is reduced by one for each explosion that touches it. The silo transitions from white to red as it loses health. Similarly, cities start with 5 health. See figure silo-upgrades.png.

Minor Game Modifications (see figure mid-game.png):
  -  Missiles have a maximum tail length, and they fade away towards the end of the tail.
  -  Players can use the "a" "s" "d" keys to fire from individual silos, corresponding to left, middle, and right mouse buttons respectively.
