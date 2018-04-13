#include <Zumo32U4.h>
#include "SimpleTimer.h"

#define FORW 0
#define TURN 1
#define REVR 2
#define TURNA 3

Zumo32U4ButtonA buttonA;
Zumo32U4Motors motors;
Zumo32U4LineSensors lineSensors;
Zumo32U4Buzzer buzzer;
SimpleTimer timer;

unsigned int lineSensorValues[5];
int state;

const char TheFinalCountdown[] PROGMEM = "!L2O5 c#16b16c#4<f#d16c#16d8c#8<bc#16"
"c#16d4<f#<b16a16<b8<a8<g#8<b8<a4R4"
"c#16b16c#4<f#d16c#16d8c#8<bc#16"
"c#16d4<f#<b16a16<b8<a8<g#8<b8<a4";

void setup() {

  lineSensors.initFiveSensors();
  buttonA.waitForButton();
  buzzer.playFromProgramSpace(TheFinalCountdown);
  delay(1000);
  timer.setInterval(3500, stateSwitch);
  state = FORW;

}

void loop() {

  timer.run();
  switch(state){
    case FORW:
      lineSensors.read(lineSensorValues);
      motors.setSpeeds(100,100);
      if(!(lineSensorValues[0] <= 1000)){
        state = TURN;
      }
      else if(!(lineSensorValues[4] <= 1000)){
        state = TURNA;
      }
    break;

    case TURN:
      lineSensors.read(lineSensorValues);
      motors.setSpeeds(50,200);
      if(lineSensorValues[0] <= 1000){
        state = FORW;
      }
      else if(!(lineSensorValues[4] <= 1000)){
        state = TURNA;
      }
    break;

    case REVR:
      lineSensors.read(lineSensorValues);
      motors.setSpeeds(-100,-100);
      //if(!(lineSensorValues[2] <= 600)){
      //  state = TURNA;
      //}
    break;

    case TURNA:
      lineSensors.read(lineSensorValues);
      motors.setSpeeds(200,50);
      if(lineSensorValues[4] <= 1000){
        state = FORW;
      }
      else if(!(lineSensorValues[0] <= 1000)){
        state = TURN;
      }
    break;
  }

}

void stateSwitch() {

  switch(state){
    case FORW:
      state = REVR;
    break;

    case REVR:
      state = FORW;
    break;

    //case TURN:
    //  state = TURNA;
    //break;

    //case TURNA:
    //  state = TURN;
    //break;
  }
  
}

