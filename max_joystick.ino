/* MAX'S ROBOT JOYSTICK CONTROLLER
 *  by: Dan Lang
 *  DVLANG CONSULTING
 *  DATE: 03/05/2018
 *     
 * BASED ON "Wireless Joystick Tank Steering Robot Example; by: Alex Wende; SparkFun Electronics; date: 9/28/16"
 * 
 * license: Creative Commons Attribution-ShareAlike 4.0 (CC BY-SA 4.0)
 * Do whatever you'd like with this code, use it for any purpose.
 * Please attribute and keep this license.
 * 
 * PURPOSE:
 * This is code for the Wireless Joystick to control a robot
 * using XBee. Plug the first Xbee into the Wireless Joystick board,
 * and connect the second to the SparkFun Serial Motor Driver.
 * 
 * Moving the left joystick up and down will drive the
 * FORWARD and REVERSE direction. The left PB will send the left
 * button press.  Right will send right.  It's up to the controller
 * to deal with it
 * 
 * Connections to the motor driver is as follows:
 * XBee - Motor Driver
 *   5V - VCC
 *  GND - GND
 * DOUT - RX
 * 
 */

#define L_BUTT 4        // pin used for left button
#define R_BUTT 9        // pin used for right button
#define H_JOYSTICK    A2
#define L_JOYSTICK A3   // Pin used for left joystick
#define R_JOYSTICK A0   // Pin used for right joystick


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
  char buf0[10],buf1[10],buf2[10],buf3[10],buf4[2],buf5[2],buf6[2]; // character buffers used to set motor speeds
  String controlword;
  
  // Read Pushbutton
  if(digitalRead(L_BUTT) == 0)
  {
    sprintf(buf2,"LEFT-PRESSED\r\n");
    sprintf(buf6,"L,");
  }
  // Increase top speed
  else if(digitalRead(R_BUTT) == 0)
  {
    sprintf(buf2,"RIGHT-PRESSED\r\n");
    sprintf(buf6,"R,");
  }
  else
  {
    sprintf(buf2,"NO BUTT\r\n");
    sprintf(buf6,"N,");
  }

  // Read joysticks
    leftStick = ((analogRead(L_JOYSTICK)));
    hStick = analogRead(H_JOYSTICK);


  // Build forward/back buffer
  if(leftStick < 300)
  {
    //sprintf(buf0,"DIR-R%d\r\n",leftStick);
    sprintf(buf0,"REVERSE\r\n");
    sprintf(buf4,"R,");
  }
  else if(leftStick > 700)
  {
   // sprintf(buf0,"DIR-F%d\r\n",abs(leftStick));
    sprintf(buf0,"FORWARD\r\n");
    sprintf(buf4,"F,");
  }
  else
  {
    //sprintf(buf0,"M0F%d\r\n",abs(leftStick));
    sprintf(buf0,"STOPPED\r\n");
    sprintf(buf4,"S,");
  }

  // Build horizontal stick buffer
  if(hStick < 300)
  {
    //sprintf(buf1,"OPEN-%d\r\n",hStick);
    sprintf(buf1,"OPEN\r\n");
    sprintf(buf5,"O,");
  }

  else if(hStick > 700)
  {
   // sprintf(buf0,"DIR-F%d\r\n",abs(leftStick));
    sprintf(buf1,"CLOSE\r\n");
    sprintf(buf5,"C,");
  }
  else
  {
   // sprintf(buf1,"CLOSE-%d\r\n",abs(hStick));
   sprintf(buf1,"HOLD\r\n");
   sprintf(buf5,"H,");

  }
  

  // Send control commands
  delay(50);
  sprintf(buf3,"%d\r\n",loopcnt);

  
  //Serial1.print("\r\n");    //comment out for actual robot control
  //Serial1.print(buf0);      //comment out for actual robot control
  //Serial1.print(buf1);      //comment out for actual robot control
  //Serial1.print(buf2);      //comment out for actual robot control
  //Serial1.print(buf3);    //comment out for actual robot control

  //build up the control word and send
  controlword.concat(buf4);
  controlword.concat(buf5);
  controlword.concat(buf6);
  controlword.concat(buf3);
  Serial1.print(controlword);

  //clear controlword and increment loop counter
  controlword = ("");
  loopcnt++;
}
