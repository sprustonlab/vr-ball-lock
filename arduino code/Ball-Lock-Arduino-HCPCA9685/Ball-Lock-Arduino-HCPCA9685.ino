/*

To use the module connect it to your Arduino as follows:

PCA9685...........Uno/Nano
GND...............GND
OE................N/A
SCL...............A5
SDA...............A4
VCC...............5V
*/


/* Include the HCPCA9685 library */
#include "HCPCA9685.h"

/* I2C slave address for the device/module. For the HCMODU0097 the default I2C address
   is 0x40 */
#define  I2CAdd 0x40

#define triggerPin 12
#define testPin 11
#define ON_POSITION 45
#define OFF_POSITION 0

unsigned long startTime = millis();
unsigned long elapsedTime;
bool teststate=true;
bool motorState=false;
  
/* Create an instance of the library */
HCPCA9685 HCPCA9685(I2CAdd);


void setup() 
{
  /* Initialise the library and set it to 'servo mode' */ 
  HCPCA9685.Init(SERVO_MODE);

  /* Wake the device up */
  HCPCA9685.Sleep(false);

  pinMode(triggerPin, INPUT);    // sets the digital pin as input
  pinMode(testPin, OUTPUT);    
  digitalWrite(testPin, HIGH);
  Serial.begin(9600);

}

void LockOn()
{
  motorState = !motorState;
  MoveMotors(ON_POSITION);
  Serial.println("Lock On");
}


void LockOff()
{
  motorState = !motorState;
  MoveMotors(OFF_POSITION);
  Serial.println("Lock Off");
}

void MoveMotors(int pos){
  for (int i = 0; i <= 15; i++) {
    HCPCA9685.Servo(i, pos);
  }
}

void loop() 
{
  elapsedTime = millis() - startTime;
  if (elapsedTime>3000){
    if (teststate){
      digitalWrite(testPin, LOW);
    }
    else {
      digitalWrite(testPin, HIGH);
    }
    teststate = !teststate;
    startTime = millis();
  }
  if (digitalRead(triggerPin) & !motorState) { LockOn();}
  if (!digitalRead(triggerPin) & motorState) { LockOff();}
}
