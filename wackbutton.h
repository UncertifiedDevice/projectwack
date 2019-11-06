#ifndef WACKBUTTON_H_INCLUDED
#define WACKBUTTON_H_INCLUDED

class wackButton
{
  public:
    //Constructor sets the pin used for the button and changes its mode to INPUT
    wackButton(short int _pin)
    : pin(_pin) 
    {
      pinMode(pin, INPUT);
    }

    //The update() function reads the state of the button and updates attributes accordingly
    void update() 
    {
      prevState = currState;
      currState = digitalRead(pin);
    }

    //Note: In our circuit the button gives out a HIGH signal while pressed down

    //Check if the button has been released this update() call
    bool up() { return prevState == HIGH && currState == LOW; }

    //Check if the button has been pressed this update() call
    bool down() { return prevState == LOW && currState == HIGH; }

    //Check if the button is currently pressed
    bool high() { return currState == HIGH; }

    //Blank destructor
    virtual ~wackButton() {}

  private;
    //Pin to read for button input
    short int pin;

    //State of the button since the last update() call
    bool currState = LOW;
    //State of the button since  update() call before
    bool prevState = LOW;
}

#endif
