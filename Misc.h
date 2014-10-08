#pragma systemFile
#pragma once

#define MISC_DEFINED

//This file is for semi-generic functions which we
//don't need to look at very often.


//One shotting
typedef struct
{
  bool defaultState;
  bool toggledLast;
} AB_OneShotData;

void AB_InitOneShot(AB_OneShotData &oneShotData, bool defaultState = false)
{
  oneShotData.defaultState = defaultState;
  oneShotData.toggledLast = false;
}

bool AB_OneShot(AB_OneShotData &oneShotData, bool input)
{
  if (input == true)
  {
    if (oneShotData.toggledLast == false)
    {
      oneShotData.toggledLast = true;
      return !oneShotData.defaultState;
    }
    else
    {
      return oneShotData.defaultState;
    }
  }
  else
  {
    oneShotData.toggledLast = false;
    return oneShotData.defaultState;
  }
}

//Toggling
typedef struct
{
  bool toggleState;
  bool toggledLast;
} AB_ToggleData;

void AB_InitToggle(AB_ToggleData &toggleData, bool state = false)
{
  toggleData.toggleState = state;
  toggleData.toggledLast = false;
}

bool AB_Toggle(AB_ToggleData &toggleData, bool input)
{
  if (input == true)
  {
    if (toggleData.toggledLast == false)
    {
      toggleData.toggleState = !toggleData.toggleState;

      toggleData.toggledLast = true;
    }
  }
  else
  {
    toggleData.toggledLast = false;
  }

  return toggleData.toggleState;
}
/*
task main()
{
  AB_ToggleData myButtonToggle;
  AB_InitToggle(myButtonToggle);

  while (true)
  {
    if (AB_Toggle(myButtonToggle, joy1Btn(3))
    {
      motor[myMotor] = 100;
    }
    else
    {
      motor[myMotor] = -100;
    }
}
*/


//If you want a value with a range of -127 to 127 to be scaled
//down to a value with a range of -100 to 100, do
//AB_Scale(input, 127, 100);
int AB_Scale(int input, int inMax, int outMax)
{
  return (int)input * outMax / inMax;
}
float AB_Scale(float input, float inMax, float outMax)
{
  return (float)input * outMax / inMax;
}


//Returns 0 if input is between min and max, otherwise just returns input.
int AB_Deadzone(int input, int min, int max)
{
  int output = input;
  if ((input > min) && (input < max))
  {
    output = 0;
  }
  return output;
}

//Clamps a number between a low number and a high number.
int AB_Clamp(int input, int min, int max)
{
  if (input > max)
  {
    input = max;
  }
  else if (input < min)
  {
    input = min;
  }
  return(input);
}
float AB_Clamp(float input, float min, float max)
{
  if (input > max)
  {
    input = max;
  }
  else if (input < min)
  {
    input = min;
  }
  return(input);
}

//Swaps the values of two variables
void AB_Swap(int &a, int &b)
{
  int c = a;
  a = b;
  b = c;
}
void AB_Swap(float &a, float &b)
{
  float c = a;
  a = b;
  b = c;
}
void AB_Swap(string &a, string &b)
{
  string c = a;
  a = b;
  b = c;
}
void AB_Swap(bool &a, bool &b)
{
  bool c = a;
  a = b;
  b = c;
}

int AB_Diff(int num1, int num2){
	return abs(num1 - num2);
}
