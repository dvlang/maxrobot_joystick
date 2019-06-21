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

bool LeftButton = false;
bool RightButton = false;

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
  
  // Read Left Pushbutton
  if(digitalRead(L_BUTT) == 0)
  {
    LeftButton = true;    
  }
  else
  {
    LeftButton = false;
  }

  //Read Right Pushbutton
  if(digitalRead(R_BUTT) == 0)
  {
   RightButton = true;
  }
  else
  {
    RightButton = false;
  }

  // Read joysticks
    leftStick = ((analogRead(L_JOYSTICK)));
    hStick = analogRead(H_JOYSTICK);

  //**********************************************
  // Build forward/back buffer
  // called "left stick but refers to forward and back
  // or arm up/down control if the right button is pressed
  //**********************************************
  if(leftStick < 300) //user is actuated
  {    
    if(LeftButton) //button is pressed for arm up/down control
    {
      sprintf(buf4,"u"); //chassis stop
      //sprintf(buf5,"U,"); //arm up
    }
    else
    {
      if(!RightButton){
        sprintf(buf4,"b"); //chassis reverse
      }
     // sprintf(buf4,"H,"); //arm hold
    }  
  }
  else if(leftStick > 700)  //user is pushing forward on stick
  {
    if(LeftButton) //left button pressed for arm up/down dontrol
    {
      sprintf(buf4,"d"); //chassis stop
     // sprintf(buf5,"D,"); //arm down
    }
    else
    {
      if(!RightButton){
        sprintf(buf4,"f");  //chassis forward
      }
     // sprintf(buf5,"H,"); //arm hold
    }
  }
  else
  {
    //sprintf(buf4,"H,"); //chassis stopped
   // sprintf(buf5,"H,"); //arm hold
  }

  //**********************************************
  // Build horizontal stick buffer
  // will open/close grip if left button is pressed
    //**********************************************
  if(hStick < 300) //horizontal stick is actuated
  {
    if(RightButton) //left button pressed for gripper dontrol
    {
      sprintf(buf4,"o"); //chassis stop
     // sprintf(buf6,"O,");  //gripper open
    }
    else
    {
      if(!LeftButton){
        sprintf(buf4,"r");  //chassis right
      }
      //sprintf(buf6,"H,"); //gripper hold
    }
    
  }

  else if(hStick > 700)
  {
    if(RightButton) //left button pressed for gripper dontrol
    {
      sprintf(buf4,"c"); //chassis stop
    //sprintf(buf6,"C,");  //gripper CLOSE
    }
    else
    {
      if(!LeftButton){
        sprintf(buf4,"l");  //chassis left
      }
      //sprintf(buf6,"H,"); //gripper HOLD
    }
  }
  else
  {

   //   sprintf(buf4,"H,");  //chassis HOLD
      //sprintf(buf6,"H,"); //gripper HOLD

  }
  

  // Send control commands
  delay(100);
  //sprintf(buf3,"%d\r\n",loopcnt);

  
  //Serial1.print("\r\n");    //comment out for actual robot control
  //Serial1.print(buf0);      //comment out for actual robot control
  //Serial1.print(buf1);      //comment out for actual robot control
  //Serial1.print(buf2);      //comment out for actual robot control
  //Serial1.print(buf3);    //comment out for actual robot control

  //build up the control word and send
  controlword.concat(buf4); //chassis forward/back motion (F,B,L,R,H) (Fowrward,Back,Left,Right,Hold)
 // controlword.concat(buf5); //arm up down (U,D,H)
 // controlword.concat(buf6); //gripper open/close (O,C,H)
 // controlword.concat(buf3);
  Serial1.print(controlword);

  //clear controlword and increment loop counter
  controlword = ("");
  sprintf(buf4,"s"); 
  loopcnt++;
}
