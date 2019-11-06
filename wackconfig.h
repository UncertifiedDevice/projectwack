//NOTE: If you want to change the number of players i.e. you're setting this up on a mega you will
//have to adapt the constructor in wackgame.h to initialize the different instances for example
//    wackGame()
//    : instanceArray({wackInstance(LED0, BTN0, BUZZ),
//                     wackInstance(LED1, BTN1, BUZZ),
//                     wackInstance(LED2, BTN2, BUZZ),
//                     wackInstance(LED3, BTN3, BUZZ)})  <-----------
//    , servo(SERV)

//Hardware Players, the maximum number of players / instances available on the current hardware
#define HWPLAYERS 3

//The starting pin of LED arrays used by different instances
#define LED0 2
#define LED1 6
#define LED2 12

//The button pin used by different instances
#define BTN0 16
#define BTN1 17
#define BTN2 18

//The pins used for the Buzzer and Servo respectively, strongly recommended to use PWM pins here
#define BUZZ 10
#define SERV 11

//The length of each game tick, lowering this value will increase responsiveness but may casuse instability
#define TICKTIME 10
