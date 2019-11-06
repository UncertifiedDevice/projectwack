#ifndef WACKLEDARRAY_H_INCLUDED
#define WACKLEDARRAY_H_INCLUDED

class wackLedArray
{
  public:
    //Constructor initializes the attributes for the LED array and sets pin modes to OUTPUT
    //Note: As the "Array" implies the pins used for the LEDs have to be consecutive
    wackLedArray(short int _pos, short int _len)
    : position(_pos)
    , length(_len)
    {
      //Looping through all pins and setting mode
      for(short int i = position; i < position + length; i++)
      {
        pinMode(i, OUTPUT);
      }
    }

    //Turns on all LEDs in the array
    void on()
    {
      for(int i = 0; i < length; i++)
      {
        //Set the pin at position + i to 5V
        digitalWrite(position + i, HIGH);
      }
    }

    //Turns off all LEDs in the array
    void off()
    {
      for(int i = 0; i < length; i++)
      {
        //Set the pin at position + i to ground
        digitalWrite(position + i, LOW);
      }
    }

    //Inverts all LEDs in the array
    void toggle()
    {
      for(int i = 0; i < length; i++)
      {
        //Conditional operator checks if the pin is currently set to 5V, if it is already 5V
        //then the pin is set to ground, if it is already ground it is set to 5V
        digitalWrite(position + i, digitalRead(position + i) == HIGH ? LOW : HIGH);
      }
    }

    //Turns on the LED at pin position + index
    void on(short int index) { digitalWrite(position + index, HIGH); }
    
    //Turns off the LED at pin position + index
    void off(short int index) { digitalWrite(position + index, LOW); }
    
    //Inverts the LED at pin position + index, uses the same conditional statement as before to decide if ground or 5V
    void toggle(short int index) { digitalWrite(position + index, digitalRead(position + index) == HIGH ? LOW : HIGH); }

    //Blank destructor, nothing to do here...
    virtual ~wackLedArray() {}

  private:
    //First pin of the LED array, think the address of an array
    short int position;
    //Number of LEDs in the LED array
    short int length;

    //Note: Not storing the state of LEDs as it can easily be retrieved with digitalRead(x)
};

#endif
