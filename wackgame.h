#ifndef WACKGAME_H_INCLUDED
#define WACKGAME_H_INCLUDED

#include "wackinstance.h"
#include "wackservo.h"

#include "wackconfig.h"

//Defining game states for code readability
#define SSETUP 0
#define SINGAME 1
#define SGAMEOVER 2

class wackGame
{
  public:
    //Constructor initializes all of the wackInstance instances with the
    //LED, Button and Buzzer pins defined in wackconfig.h, also initializes
    //servo class with the servo pin defined in that header
    wackGame()
    : instanceArray({wackInstance(LED0, BTN0, BUZZ),
                     wackInstance(LED1, BTN1, BUZZ),
                     wackInstance(LED2, BTN2, BUZZ)})
    , servo(SERV)
    { }

    //Update / tick function that processes the games logic, to be run every loop()
    void update()
    {
      //Check if it's time to execute the next tick
      if(millis() >= tickTime)
      {
        //Set the timer for the next tick
        tickTime = millis() + TICKTIME;

        //Execute code associated with the current game state
        switch(gameState)
        {
          //Set the player count and difficulty for the
          //game based on input
          case SSETUP:
          {
            //Execute code associated with the current setup stage
            switch(setupStage)
            {
              //Short animation and tone to indicate everything is loaded
              case 0:
              {
                //Iterate through all instances interface with their led classes directly
                for(int i = 0; i < HWPLAYERS; i++)
                {
                  //Sound the buzzer for this instance
                  tone(BUZZ, TONEHIT * (i + 1) / HWPLAYERS, 100);

                  //Flash the 4 LEDs of this instance in ascending order
                  instanceArray[i].leds.toggle(0);
                  for(int j = 0; j < 3; j++)
                  {
                    delay(100);
                    instanceArray[i].leds.toggle(j);
                    instanceArray[i].leds.toggle(j + 1);
                  }
                  delay(100);
                  instanceArray[i].leds.toggle(3);
                }

                //Proceed to the next stage of setup
                setupStage = 1;
                break;
              }

              //Read button input to determine the number of players playing
              case 1:
              {
                //Iterate through all hardware input to detect which button was pressed
                for(int i = 0; i < HWPLAYERS; i++)
                {
                  instanceArray[i].button.update();

                  if(instanceArray[i].button.down())
                  {
                    //Briefly flash LEDs associated with this button
                    instanceArray[i].leds.on();
                    tone(BUZZ, TONEHIT, 50);
                    delay(100);
                    tone(BUZZ, TONEHIT, 50);
                    delay(100);
                    instanceArray[i].leds.off();

                    //Set the player count to include this button and all before it
                    //Proceed to the next stage of setup
                    playerCount = i + 1;
                    setupStage = 2;
                    break;
                  }
                }

                break;
              }

              //Read button input to determine the desired difficulty
              case 2:
              {
                //If there are not enough buttons to select between three options a default it used
                if(HWPLAYERS < 3)
                {
                  instReset(HWDIFFICULTY);
                  break;
                }

                //Iterate through all hardware input to detect which button was pressed
                for(int i = 0; i < 3; i++)
                {
                  instanceArray[i].button.update();

                  if(instanceArray[i].button.down())
                  {
                    //Briefly flash LEDs associated with this button
                    instanceArray[i].leds.on();
                    tone(BUZZ, TONEHIT, 50);
                    delay(100);
                    tone(BUZZ, TONEHIT, 50);
                    delay(100);
                    instanceArray[i].leds.off();

                    //Use this button to decide the difficulty of the game
                    //Proceed to the final stage of setup
                    instReset(i);
                    setupStage = 3;
                    break;
                  }
                }

                break;
              }

              //Switch game state to SINGAME, reset setup stage for next time
              case 3:
              {
                gameState = SINGAME;
                setupStage = 0;
                break;
              }

              //Perform these actions if stage is undefined
              //This should never happen
              default:
              {
                Serial.println("WARN: Possible memory corruption, setupStage was undefined");
                setupStage = 0;
                break;
              }
            }

            break;
          }

          //Update all of the instances and check scores,
          //set the servo to point to the current leader
          case SINGAME:
          {
            //Update instances...
            instUpdate();

            //Variable used to store the current leader,
            //equal to playerCount if there's multiple leaders
            int i = 0;

            //Iterate through all active instances to find current highest score
            for( ; i < playerCount; i++)
            {
              //These variables are used to iterate through other active instances to compare scores
              int j = i + 1;
              int k = i - 1;

              //Iterate through all active instances after this one, stop when one has a score higher
              //than or equal to that of the current instance
              for( ; j < playerCount; j++)
              {
                if(instanceArray[i].getScore() <= instanceArray[j].getScore()) { break; }
              }

              //Iterate through all active instances before this one, stop when one has a score higher
              //than or equal to that of the current instance
              for( ; k >= 0; k--)
              {
                if(instanceArray[i].getScore() <= instanceArray[k].getScore()) { break; }
              }

              //If this instance has the highest score the previous two loops will run until
              //j is equal to player count and k is -1, stop iterating through active instances
              //if this happens, if this doesn't happen i will end up equal to playerCount, this
              //means there is no single highest score
              if(j == playerCount && k == -1)
              {
                break;
              }
            }

            //Multiple players hold the top spot or there's only one player, just sweep the servo
            if(i == playerCount || playerCount == 0)
            {
              //Swap the servo between its minimum and maximum positions every 700ms
              if(millis() > sweepTimer)
              {
                //Conditional statement used to decide where to move the servo next based on where it is now
                servo.write(servo.read() > MAXSERVOPOS / 2 ? 0 : MAXSERVOPOS);
                sweepTimer = millis() + 700;
              }

              //Since there are multiple leaders all of the scores need to be checked to see if any has
              //reached the score limit defined in the config header
              for(int j = 0; j < playerCount; j++)
              {
                if(instanceArray[j].getScore() >= SCORELIMIT)
                {
                  //Winner is undefined / there are multiple winners
                  winner = playerCount;

                  //Switch to game over state, the game has ended
                  gameState = SGAMEOVER;

                  //Turn off all of the LEDs, the game is over
                  for(int k = 0; k < playerCount; k++)
                  {
                    instanceArray[k].leds.off();
                  }
                }
              }
            }
            //One specific player holds the top spot, update servo accordingly
            else
            {
              //Move the servo to point towards the winning player on a dial mounted to the servo
              servo.write(MAXSERVOPOS * i / (HWPLAYERS - 1));

              //Check if the leading player has reached the score limit
              if(instanceArray[i].getScore() >= SCORELIMIT)
              {
                //The leading player has won, update the winner attribute accordingly
                winner = i;

                //Switch to game over state, the game has ended
                gameState = SGAMEOVER;

                //Turn off all of the LEDs, the game is over
                for(int j = 0; j < playerCount; j++)
                {
                  instanceArray[j].leds.off();
                }
              }
            }

            break;
          }

          //Play a small animation on winners LEDs, go back
          //to SSETUP if any input is detected
          case SGAMEOVER:
          {
            gameState = SSETUP;
            break;
          }

          //Perform these actions if state is undefined
          //This should never happen
          default:
          {
            Serial.println("WARN: Possible memory corruption, gameState was undefined");
            gameState = SSETUP;
            break;
          }
        }

        //Update the servo position
        servo.update();

        //Wait until roughly the next tick, this is mostly done to save power, if you want
        //to do other stuff in your loop() you can safely comment this line out
        delay(tickTime - millis() <= TICKTIME ? tickTime - millis() : 0);
      }
    }

    //Blank destructor, nothing to do here...
    virtual ~wackGame() {}

  protected:
    //Reset all of the active game instances and set difficulty
    void instReset(const short int difficulty)
    {
      for(int i = 0; i < playerCount; i++)
      {
        instanceArray[i].reset(difficulty);
      }
    }

    //Update all of the active game instances
    void instUpdate()
    {
      for(int i = 0; i < playerCount; i++)
      {
        instanceArray[i].update();
      }
    }

  private:
    //All of the player instances supported by hardware
    wackInstance instanceArray[HWPLAYERS];
    //Storing the servo class used to indicate leader
    wackServo servo;

    //The time until the next tick, the current game state and player count
    unsigned long int tickTime = 0;
    short int gameState = 0;
    int playerCount = 0;

    //Used to store the sub-state / stage of the setup
    short int setupStage = 0;

    //Timer used to sweep the servo in case multiple players have the same score
    unsigned long int sweepTimer = 0;

    //Used for animations in gameover state
    short int winner;
};

#endif
