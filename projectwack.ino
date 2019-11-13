#include "wackgame.h"

//Setting up the game
wackGame game;

void setup() {
  //Setting up the serial port for printing
  Serial.begin(9600);
}

void loop() {
  //Ticking the game
  game.update();
}
