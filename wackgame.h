#ifndef WACKGAME_H_INCLUDED
#define WACKGAME_H_INCLUDED

#include "wackinstance.h"
#include "wackservo.h"

//Hardware Players, the maximum number of players / instances available on the current hardware
#define HWPLAYERS 3

//Defining the starting pin of LED arrays used by different instances
#define LED0 2
#define LED1 6
#define LED2 12

//Defining the button pin used by different instances
#define BTN0 16
#define BTN1 17
#define BTN2 18

//Defining the pins used for the Buzzer and Servo, strongly recommended to use PWM pins here
#define BUZZ 10
#define SERV 11

//Defining game states
#define SSETUP 0
#define SINGAME 1
#define SGAMEOVER 2

class wackGame
{
  public:
    wackGame()
    : instanceArray({wackInstance(LED0, BTN0, BUZZ),
                     wackInstance(LED1, BTN1, BUZZ),
                     wackInstance(LED2, BTN2, BUZZ)})
    , servo(SERV)
    { }

    void update()
    {
      if(millis() >= tickTime)
      {
        tickTime = millis() + 10;

        switch(gameState)
        {
          case SSETUP:
          {
            reset(2);

            gameState = SINGAME;
            break;
          }

          case SINGAME:
          {
            for(int i = 0; i < HWPLAYERS; i++)
            {
              instanceArray[i].update();
            }

            break;
          }

          case SGAMEOVER:
          {
            break;
          }

          default:
          {
            gameState = SSETUP;
            break;
          }
        }

        delay(tickTime - millis());
      }
    }

    virtual ~wackGame() {}

  protected:
    void reset(const short int difficulty)
    {
      for(int i = 0; i < HWPLAYERS; i++)
      {
        instanceArray[i].reset(difficulty);
      }
    }

  private:
    wackInstance instanceArray[HWPLAYERS];
    wackServo servo;

    unsigned long int tickTime = 0;
    short int gameState = 0;
};

#endif
