/*

  To use the PCA9685 module connect it to your Arduino as follows:

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

#define triggerPin A0   // Pin that receives TTL trigger signal.
#define ON_POSITION 117  // Angle of servos in ON state. (0-180)
#define OFF_POSITION 0   // Angle of servos in OFF state. (0-180)

unsigned long readBuffer; // Stores values of trigger pin.
bool motorState = false; // Stores current state of the motor so its only changed once per value change.

/* Create an instance of the library */
HCPCA9685 HCPCA9685(I2CAdd);

// Setup and Initialization Loop.
void setup()
{
  /* Initialise the library and set it to 'servo mode' */
  HCPCA9685.Init(SERVO_MODE);

  /* Wake the device up */
  HCPCA9685.Sleep(false);

  // Set mode of trigger pin.
  pinMode(triggerPin, INPUT);    // sets the trigger pin as input
  // Begin serial output.
  Serial.begin(9600);
  // Set Lock off.
  LockOff();
}

// LockOn. Turn on ball lock.
void LockOn()
{
  motorState = !motorState;
  MoveMotors(ON_POSITION);
  Serial.println("Lock On");
}

// LockOff. Turn off ball lock.
void LockOff()
{
  motorState = !motorState;
  MoveMotors(OFF_POSITION);
  Serial.println("Lock Off");
}

// MoveMotors. Moves all motors to requested angle. Called by LockOn/LockOff.
void MoveMotors(int angle) {
  for (int i = 0; i <= 15; i++) {
    HCPCA9685.Servo(i, map(angle,0,180,0,375) ); // maps angle to position (0-180 degrees to 0-375 (emprically determined)).
  }
}


// Main Loop.
void loop()
{
  // Read N samples of the trigger pin value. (avoids noise issues).
  for (int i = 0 ; i < 500 ; i++)
  {
    readBuffer = readBuffer+analogRead(triggerPin);
  }
  readBuffer = readBuffer/500; // average.
  Serial.println(readBuffer);

  // Check if state of trigger pin has changed.
  if (readBuffer>900 & !motorState) { LockOn(); }
  if (readBuffer<900 & motorState) { LockOff(); }
}
