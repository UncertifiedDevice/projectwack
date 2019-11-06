#ifndef WACKINSTANCE_H_INCLUDED
#define WACKINSTANCE_H_INCLUDED

#include "wackledarray.h"
#include "wackbutton.h"

//Three difficulty settings, the first number is the minimum delay between LED
//toggles, the second number is the upper bounds of an rng starting with 0
#define DIFFICULTY0 400 + rand() % 150
#define DIFFICULTY1 300 + rand() % 250
#define DIFFICULTY2 150 + rand() % 300

class wackInstance
{
  //Constructor initializes the wackButton and wackLedArray classes which are
  //members of this class, also defines the buzzer pin and seeds the rng
  public:
    wackInstance(short int _ledPin0, short int _buttonPin, short int _buzzerPin)
    : leds(_ledPin0, 4)
    , button(_buttonPin)
    , buzzer(_buzzerPin)
    {
      srand(micros());
    }

    //Used to reset this instance, to be used when the game is restarting
    //from a game over or to set the difficulty initially
    void reset(short int _difficulty)
    {
      //Update difficulty and reset score
      difficulty = _difficulty;
      score = 0;

      //Clear all LEDs under this instance
      leds.off();

      //Prime the timer and select a random LED to start the instance
      randTime = millis() + difficultyTimeGen();
      randLed = rand() % 3;
    }

    //Used every game tick to process user input and update the score and LEDs
    void update()
    {
      //Poll the button...
      button.update();

      //Perform some actions when button is pressed
      if(button.down())
      {
        //Perform these actions if the player lands a hit
        if(leds.get(randLed))
        {
          //Turn off the current LED
          leds.off(randLed);

          //Turn on the 4th LED to indicate a hit, set a timer for it to stay on
          leds.on(3);
          scoreFlashTime = millis() + 300;

          //Prime the timer and select a random LED to turn on next
          randTime = millis() + difficultyTimeGen();
          randLed = rand() % 3;

          //Sound the buzzer to indicate a hit and finally increment the score
          tone(buzzer, 1800, 50);
          score++;
        }
        //Perform these actions if the player misses
        else
        {
          //Sound the buzzer at a lower frequency to indicate a miss, increase
          //time until currently selected LED turns on as a penalty, also to discourage
          //spamming the button as the LED will remain off indefinitely
          tone(buzzer, 900, 50);
          randTime += 350;
        }
      }

      //Reset the 4th LED (The blue one) after it lights up to indicate a hit
      if(millis() > scoreFlashTime && leds.get(3))
      {
        leds.off(3);
      }

      //Toggle an LED if nothing happened while it was on / off
      if(millis() > randTime)
      {
        //Toggle the currently selected LED
        leds.toggle(randLed);

        //Prime the timer for the next toggle
        randTime = millis() + difficultyTimeGen();

        //If the currently selected LED was just turned off, select a new one
        //otherwise stay on the current LED so multiple LEDs dont end up getting lit
        if(!leds.get(randLed)) { randLed = rand() % 3; }
      }
    }

    //Simple getter for the score, used in overarching game logic for comparisons
    int getScore() { return score; }

    //Blank destructor, nothing to do here...
    virtual ~wackInstance() {}

  protected:
    //Generate a random time for LEDs to stay on/off based on current difficulty
    unsigned short int difficultyTimeGen()
    {
      //Based on difficulty...
      switch(difficulty)
      {
        //Easy / 0 difficulty
        case 0:
          return DIFFICULTY0;
          break;

        //Medium / 1 difficulty
        case 1:
          return DIFFICULTY1;
          break;

        //Hard / 2 difficulty
        case 2:
          return DIFFICULTY2;
          break;

        //Undefined difficulty, defaults to Easy / 0
        //This should never happen
        default:
          return DIFFICULTY0;
          break;
      }
    }

  private:
    //Storing an LED Array and a Button for this instance, also storing the pin used for the buzzer
    //Buzzer pin can be set to -1 to effectively disable it
    wackLedArray leds;
    wackButton button;
    short int buzzer;

    //Storing the current difficulty, defaults to Easy / 0
    short int difficulty = 0;

    //Store the time to flash the 4th / Hit LED until
    unsigned long int scoreFlashTime = 0;
    
    //Store the time until the current LED has to be toggled
    unsigned long int randTime = 0;
    
    //Store the current LED that is about to be turned on / off
    short int randLed = 0;
    
    //Store the player's current score on this instance
    int score = 0;

    //Define wackGame as a friend class of wackInstance so that wackGame can access private members of
    //wackInstance such as leds and button that it can use during setup or gameover
    friend class wackGame;
};

#endif
