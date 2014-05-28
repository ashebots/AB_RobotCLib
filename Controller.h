#ifndef getJoystickSettings
  #include "JoystickDriver.c"
#endif

//THE ORDER OF THIS ENUM IS CRITICAL.
//NO NO TOUCHY
typedef enum
{
  X1 = 1,
  A1,
  B1,
  Y1,
  LeftBumper1,
  RightBumper1,
  LeftTrigger1,
  RightTrigger1,
  LeftArrow1,
  RightArrow1,
  LeftJoy1,
  RightJoy1,
  HatUp1,
  HatUpRight1,
  HatRight1,
  HatDownRight1,
  HatDown1,
  HatDownLeft1,
  HatLeft1,
  HatUpLeft1, //20

  X2, //21
  A2,
  B2,
  Y2,
  LeftBumper2,
  RightBumper2,
  LeftTrigger2,
  RightTrigger2,
  LeftArrow2,
  RightArrow2,
  LeftJoy2,
  RightJoy2,
  HatUp2,
  HatUpRight2,
  HatRight2,
  HatDownRight2,
  HatDown2,
  HatDownLeft2,
  HatLeft2,
  HatUpLeft2, //40
} Gamepad;


bool ButtonState(Gamepad button)
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
