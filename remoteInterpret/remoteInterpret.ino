#include <Wire.h>
#include "TurnSensor.h"
#include <SoftwareSerial.h>
#include <ZumoShield.h>

ZumoMotors motors;
L3G gyro;

int speed = 200;

SoftwareSerial XBee(2, 3); // RX, TX

void setup()
{
	XBee.begin(9600);
	Serial.begin(9600);
	turnSensorSetup();
}

void loop()
{
	turnSensorUpdate();
	if(XBee.available()){
		char dat = XBee.read();
		switch (dat) {

		// Forwards and Backwards

			case 'w':
				turnSensorUpdate();
				motors.setSpeeds(speed,speed);
				break;
			case 's':
				turnSensorUpdate();
				motors.setSpeeds(-speed,-speed);
				break;
			
		// 90 Degree Turns

			case 'e': // Clockwise (Right)
				turnSensorUpdate();
				motors.setSpeeds(100,-100);
				while((int32_t)turnAngle > -turnAngle45 * 2){
					turnSensorUpdate();
				}
				motors.setSpeeds(0,0);
				turnSensorReset();
				dat = 'h';
				XBee.write(dat);
				break;
			case 'q': // Anti-clockwise (Left)
				turnSensorUpdate();
				motors.setSpeeds(-100,100);
				while((int32_t)turnAngle < turnAngle45 * 2){
					turnSensorUpdate();
				}
				motors.setSpeeds(0,0);
				turnSensorReset();
				dat = 'h';
				XBee.write(dat);
				break;

		// Stop

			case 'h':
				motors.setSpeeds(0,0);
				break;

		// Continuous Lef and Right Turns

			case 'a':
				motors.setSpeeds(-100,100);
				turnSensorUpdate();
				break;
			case 'd':
				motors.setSpeeds(100,-100);
				turnSensorUpdate();
				break;

		// Variable Speed Controls

			case '1':
				speed = 100;
				break;
			case '2':
				speed = 200;
				break;
			case '3':
				speed = 300;
				break;
			case '4':
				speed = 400;
				break;

		// "Snap" to Right Angle

			case 'z': // "Snap" Left
				motors.setSpeeds(-100,100);
				while(((int32_t)turnAngle % (turnAngle45 * 2)) != 0){
					turnSensorUpdate();
				}
				motors.setSpeeds(0,0);
				turnAngle = 0;
				dat = 'h';
				XBee.write(dat);
				break;
			case 'x': // "Snap" Right
				motors.setSpeeds(100,-100);
				while(((int32_t)turnAngle % (turnAngle45 * 2)) != 0){
					turnSensorUpdate();
				}
				motors.setSpeeds(0,0);
				turnAngle = 0;
				dat = 'h';
				XBee.write(dat);
				break;

		// 180 Degree Turn

			case 'p':
				turnSensorUpdate();
				motors.setSpeeds(-100,100);
				while((int32_t)turnAngle < turnAngle45 * 4){
					turnSensorUpdate();
				}
				motors.setSpeeds(0,0);
				turnAngle = 0;
				dat = 'h';
				XBee.write(dat);
				break;

			case 'u':
				turnSensorUpdate();
				break;
		}
	}
}