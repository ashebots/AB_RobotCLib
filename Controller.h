#pragma systemFile //This is so the compiler doesn't complain about unused functions
#pragma once //This is to make sure we don't include the same module twice

//Make sure we load the joystick driver
#ifndef getJoystickSettings
  #include "JoystickDriver.c"
#endif

//Make sure we load the Misc module
#ifndef AB_MISC_MODULE
  #include "Misc.h"
#endif

//Set a variable for a control to one of these.
typedef enum AB_Gamepad
{
  X1            = 1, //First controller
  A1            = 2,
  B1            = 3,
  Y1            = 4,
  LeftBumper1   = 5,
  RightBumper1  = 6,
  LeftTrigger1  = 7,
  RightTrigger1 = 8,
  LeftArrow1    = 9,
  RightArrow1   = 10,
  LeftJoy1      = 11,
  RightJoy1     = 12,
  HatUp1        = 13,
  HatUpRight1   = 14,
  HatRight1     = 15,
  HatDownRight1 = 16,
  HatDown1      = 17,
  HatDownLeft1  = 18,
  HatLeft1      = 19,
  HatUpLeft1    = 20,

  X2            = 21, //Second controller
  A2            = 22,
  B2            = 23,
  Y2            = 24,
  LeftBumper2   = 25,
  RightBumper2  = 26,
  LeftTrigger2  = 27,
  RightTrigger2 = 28,
  LeftArrow2    = 29,
  RightArrow2   = 30,
  LeftJoy2      = 31,
  RightJoy2     = 32,
  HatUp2        = 33,
  HatUpRight2   = 34,
  HatRight2     = 35,
  HatDownRight2 = 36,
  HatDown2      = 37,
  HatDownLeft2  = 38,
  HatLeft2      = 39,
  HatUpLeft2    = 40,
};


bool AB_ButtonState(AB_Gamepad button)
{
  const int lastGamepad1Control = HatUpLeft1; //A "Control" encompasses all the buttons + the 8 different TopHats.

  const int lastGamepad1Button = RightJoy1; //A "Button" is everything other than a TopHat.
  const int lastGamepad2Button = RightJoy2;

  if (button <= lastGamepad1Control)
  { //Gamepad ONE
    if (button <= lastGamepad1Button)
    { //Button
      return (bool)joy1Btn(button);
    }
    else
    { //Tophat
      int askedForTopHat = button - HatUp1;
      return (askedForTopHat == joystick.joy1_TopHat);
    }
  }
  else
  { //Gamepad TWO
    if (button <= lastGamepad2Button)
    { //Button
      return (bool)joy2Btn(button - lastGamepad1Control);
    }
    else
    { //Tophat
      int askedForTopHat = button - HatUp2;
      return (askedForTopHat == joystick.joy2_TopHat);
    }
  }
}

/*!
Scales a value with a max of 127 down to 100. Useful for scaling raw joystick axes before using them for driving.
*/
int AB_ScaleJoystick(int joystickAxis)
{
	return AB_Scale(joystickAxis, 127, 100);
}
