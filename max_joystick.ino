/* Wireless Joystick Tank Steering Robot Example
 * by: Alex Wende
 * SparkFun Electronics
 * date: 9/28/16
 * 
 * license: Creative Commons Attribution-ShareAlike 4.0 (CC BY-SA 4.0)
 * Do whatever you'd like with this code, use it for any purpose.
 * Please attribute and keep this license.
 * 
 * This is example code for the Wireless Joystick to control a robot
 * using XBee. Plug the first Xbee into the Wireless Joystick board,
 * and connect the second to the SparkFun Serial Motor Driver.
 * 
 * Moving the left and right joystick up and down will change the
 * speed and direction of motor 0 and motor 1. The left trigger will
 * reduce the maximum speed by 5%, while the right trigger button
 * will increase the maximum speed by 5%.
 * 
 * Connections to the motor driver is as follows:
 * XBee - Motor Driver
 *   5V - VCC
 *  GND - GND
 * DOUT - RX
 * 
 * Power the motor driver with no higher than 11V!
 */

#define L_TRIG 6        // Pin used for left trigger
#define R_TRIG 3        // Pin used for right trigger
#define L_BUTT 4        // pin used for left button
#define R_BUTT 9        // pin used for right button
#define H_JOYSTICK    A2
#define L_JOYSTICK A3   // Pin used for left joystick
#define R_JOYSTICK A0   // Pin used for right joystick

int8_t speedLevel = 20; //Maximum speed (%) = speedLevel * 5 (units are percent)
unsigned long loopcnt = 0;

void setup() {  
  Serial1.begin(9600); // Start serial communication with XBee at 9600 baud
  delay(10);

  Serial1.print("W7001\r\n"); // Set the bit in enable register 0x70 to 0x01

  pinMode(L_BUTT,INPUT_PULLUP); // Enable pullup resistor for left trigger
  pinMode(R_BUTT,INPUT_PULLUP); // Enable pullup resistor for right trigger
}

void loop() {
  int16_t leftStick, rightStick =500;    // We'll store the the analog joystick values here
  int16_t hStick = 500;
  char buf0[10],buf1[10],buf2[10],buf3[10]; // character buffers used to set motor speeds

  // Reduce top speed
  if(digitalRead(L_BUTT) == 0)
  {
    sprintf(buf2,"LEFT-PRESSED\r\n");
  }
  // Increase top speed
  else if(digitalRead(R_BUTT) == 0)
  {
    sprintf(buf2,"RIGHT-PRESSED\r\n");
  }
  else
  {
    sprintf(buf2,"NO BUTT\r\n");
  }

  // Read joysticks
  // Convert analog value range to motor speeds (in %)
  //leftStick = (5-(analogRead(L_JOYSTICK)/93))*speedLevel;
    leftStick = ((analogRead(L_JOYSTICK)));
    hStick = analogRead(H_JOYSTICK);
  //rightStick = (5-(analogRead(R_JOYSTICK)/93))*speedLevel;

  // Build forward/back buffer
  if(leftStick < 300)
  {
    //sprintf(buf0,"DIR-R%d\r\n",leftStick);
    sprintf(buf0,"REVERSE\r\n");
  }
  else if(leftStick > 700)
  {
   // sprintf(buf0,"DIR-F%d\r\n",abs(leftStick));
    sprintf(buf0,"FORWARD\r\n");
  }
  else
  {
    //sprintf(buf0,"M0F%d\r\n",abs(leftStick));
    sprintf(buf0,"STOPPED\r\n");
  }

  // Build horizontal stick buffer
  if(hStick < 300)
  {
    //sprintf(buf1,"OPEN-%d\r\n",hStick);
    sprintf(buf1,"OPEN\r\n");
  }

  else if(hStick > 700)
  {
   // sprintf(buf0,"DIR-F%d\r\n",abs(leftStick));
    sprintf(buf1,"CLOSE\r\n");
  }
  else
  {
   // sprintf(buf1,"CLOSE-%d\r\n",abs(hStick));
   sprintf(buf1,"HOLD\r\n");

  }
  

  // Build motor 1 buffer
 // if(rightStick > 0)
 // {
 //   sprintf(buf1,"M1F%d\r\n",rightStick);
//  }
//  else
//  {
//    sprintf(buf1,"M1R%d\r\n",abs(rightStick));
//  }

  // Send motor speeds
  delay(500);
  Serial1.print("\r\n");
  Serial1.print(buf0);
 // delay(1000);
  Serial1.print(buf1);
  Serial1.print(buf2);
 // Serial1.print(loopcnt);
  
  sprintf(buf3,"%d\r\n",loopcnt);
  Serial1.print(buf3);
  loopcnt++;
}
