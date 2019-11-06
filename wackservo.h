#ifndef WACKSERVO_H_INCLUDED
#define WACKSERVO_H_INCLUDED

#include <Servo.h>

class wackServo
{
  public:
    //Constructor initializes the pin to be used for the Servo. This
    //pin must be a PWM digital pin for the Servo to work correctly
    wackServo(const short int _pin)
    : pin(_pin)
    {}

    //To be called every loop(), ensures the servo isn't attached if it
    //isn't currently working and writes a new position if it has changed
    //Note: We found that if the servo stays attached while it is idling
    //it can draw current, make a buzzing noise or vibrate and heat up fast
    void update()
    {
      //Perform these actions if the position has changed
      if(lastposition != position)
      {
        //If the servo is currently inactive / detached, activate / attach it
        if(!active)
        {
          active = true;
          servo.attach(pin);
        }

        //Update the idle time as the servo is currently being used
        idleTime = millis() + 1000;

        //Write the new position to the servo
        servo.write(position);

        //Update the last position with the current position
        lastposition = position;
      }

      //Perform these actions if the servo is idle for > 1s and is currently active
      if(millis() >= idleTime && active)
      {
        //Deactivate / Detach the servo
        active = false;
        servo.detach();
      }
    }

    //To be called whenever any other class wants to write a position to the servo
    //Technically potitions range from 0 to 180 but we found our servo wont move past 160
    void write(short int _position)
    {
      position = _position;
    }

    //Turn off / detach the servo in the destructor
    virtual ~wackServo()
    {
      //If the servo is active at the time derstructor is run, deactivate / detach it
      if(active)
      {
        servo.detach();
      }
    }

  private:
    //Storing the pin to be used for the servo and the arduino servo class itself
    const short int pin;
    Servo servo;

    //Store the current and last position of the servo, used to detect changes in position
    short int lastposition = 0;
    short int position = 0;

    //Store the time the servo has been idling since and whether it is currently active or not
    unsigned long int idleTime = 0;
    bool active = false;
};

#endif
