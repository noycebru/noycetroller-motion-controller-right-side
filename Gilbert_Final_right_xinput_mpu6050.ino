
// Right controller code (I2C secondary)
// I2C section created by https://www.twitch.tv/gilbertsgadgets
// Special shoutout to GrayArtificer for I2C foundation
// https://github.com/shveytank/MPU_6050_Guesture_codes/blob/master/MPU6050_XYZacc3.ino
/*
    Project     Arduino XInput Library
    @author     David Madison
    @link       github.com/dmadison/ArduinoXInput
    @license    MIT - Copyright (c) 2019 David Madison

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

    Example:      GamepadPins
    Description:  Uses all of the available pin inputs to build a 'complete'
                  Xbox gamepad, with both analog joysticks, both triggers,
                  and all of the main buttons.

 *                * Joysticks should be your typical 10k dual potentiometers.
 *                * Triggers can be either analog (pots) or digital (buttons).
                    Set the 'TriggerButtons' variable to change between the two.
 *                * Buttons use the internal pull-ups and should be connected
                    directly to ground.

                  These pins are designed around the Leonardo's layout. You
                  may need to change the pin numbers if you're using a
                  different board type

*/

#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
byte x = 0;

//MPU6050 mpu;
//int16_t ax, ay, az;
//int16_t gx, gy, gz;


// Joystick Pins
const int Pin_RightJoyX = A0;
const int Pin_RightJoyY = A1;

// Trigger Pins
const int Pin_TriggerR = A2;

// Button Pins
const int Pin_ButtonA = 4;
const int Pin_ButtonB = 5;
const int Pin_ButtonX = 6;
const int Pin_ButtonY = 7;

const int Pin_ButtonRB = 8;

const int Pin_ButtonStart = 9;

const int Pin_ButtonR3 = 10;

/********* Data packet format *************/

typedef struct {
  boolean buttonA;
  boolean buttonB;
  boolean buttonX;
  boolean buttonY;
  boolean buttonRB;
  boolean buttonStart;
  boolean buttonR3;
  int triggerRight;
  int joyX;
  int joyY;
} RIGHT_CONTROLLER_BUTTONS;

RIGHT_CONTROLLER_BUTTONS ButtonStates;

struct MyData {
  byte X;
  byte Y;
};

MyData data;
/**************** SETUP FUNCTION *******************/

void setup() {
  //  Serial.begin(9600);
  //  Serial.println("Serial started");
  // Pin Setup
  /*
    // If using buttons for the triggers, use internal pull-up resistors
    if (UseTriggerButtons == true) {
      pinMode(Pin_TriggerR, INPUT_PULLUP);
    }
  */

  // Set buttons as inputs, using internal pull-up resistors
  pinMode(Pin_ButtonA, INPUT_PULLUP);
  pinMode(Pin_ButtonB, INPUT_PULLUP);
  pinMode(Pin_ButtonX, INPUT_PULLUP);
  pinMode(Pin_ButtonY, INPUT_PULLUP);

  pinMode(Pin_ButtonRB, INPUT_PULLUP);

  pinMode(Pin_ButtonStart, INPUT_PULLUP);

  pinMode(Pin_ButtonR3, INPUT_PULLUP);

  //  Serial.println("Starting I2C");
  // Initialize I2C


  Wire.begin(0x08); // join i2c bus with address 0x08
  Wire.onRequest(requestEvent); // register event


  //  Serial.println("I2C initialized");
}

/***************** LOOP FUNCTION *****************/

void loop() {
 // mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  //data.X = map(ax, -17000, 17000, 0, 255 ); // X axis data
  //data.Y = map(ay, -17000, 17000, 0, 255);  // Y axis data
  // Read all button states and store in ButtonStates

ButtonStates.buttonA = !digitalRead(Pin_ButtonA);
  ButtonStates.buttonB = !digitalRead(Pin_ButtonB);
  ButtonStates.buttonX = !digitalRead(Pin_ButtonX);
  ButtonStates.buttonY = !digitalRead(Pin_ButtonY);
  ButtonStates.buttonRB = !digitalRead(Pin_ButtonRB);
  ButtonStates.buttonStart = !digitalRead(Pin_ButtonStart);
  ButtonStates.buttonR3 = !digitalRead(Pin_ButtonR3);

  // Read trigger potentiometer values
  ButtonStates.triggerRight = analogRead(Pin_TriggerR);

  // Read joystick values
  ButtonStates.joyX = analogRead(Pin_RightJoyX);
  ButtonStates.joyY = analogRead(Pin_RightJoyY);

  // A little delay, for kicks. Might not be necessary...
  delay(10);
}

/***************** I2C REQUEST EVENT FUNCTION ****************/

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  //  Serial.println("Request for data received by secondary.");
  // Send the states of all the buttons

  Wire.write((char*) &ButtonStates, sizeof(RIGHT_CONTROLLER_BUTTONS));
  

  // Don't need the following, but might help for debug/bring-up
  //  Wire.write("hello "); // respond with message of 6 bytes as expected by master
  //  Serial.println("Data sent");
}
