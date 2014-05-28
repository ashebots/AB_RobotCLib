#define MISC_DEFINED

//This file is for semi-generic functions which we
//don't need to look at very often.


//One shotting
typedef struct
{
  bool defaultState;
  bool toggledLast;
} OneShotData;

void InitOneShot(OneShotData &oneShotData, bool defaultState = false)
{
  oneShotData.defaultState = defaultState;
  oneShotData.toggledLast = false;
}

bool OneShot(OneShotData &oneShotData, bool input)
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
} ToggleData;

void InitToggle(ToggleData &toggleData, bool state = false)
{
  toggleData.toggleState = state;
  toggleData.toggledLast = false;
}

bool Toggle(ToggleData &toggleData, bool input)
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
  ToggleData myButtonToggle;
  InitToggle(myButtonToggle);

  while (true)
  {
    if (Toggle(myButtonToggle, joy1Btn(3))
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
//Scale(input, 127, 100);
int Scale(int input, int inMax, int outMax)
{
  return (int)input * outMax / inMax;
}
float Scale(float input, float inMax, float outMax)
{
  return (float)input * outMax / inMax;
}


//Returns 0 if input is between min and max, otherwise just returns input.
int deadzone(int input, int min, int max)
{
  int output = input;
  if ((input > min) && (input < max))
  {
    output = 0;
  }
  return output;
}

//Clamps a number between a low number and a high number.
int clamp(int input, int min, int max)
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
float clamp(float input, float min, float max)
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
long clamp(long input, long min, long max)
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
void swap(int &a, int &b)
{
  int c = a;
  a = b;
  b = c;
}
void swap(float &a, float &b)
{
  float c = a;
  a = b;
  b = c;
}
void swap(string &a, string &b)
{
  string c = a;
  a = b;
  b = c;
}
void swap(bool &a, bool &b)
{
  bool c = a;
  a = b;
  b = c;
}
void swap(long &a, long &b)
{
  long c = a;
  a = b;
  b = c;
}

int AbsDiff(int num1, int num2){
	return abs(num1 - num2);
}
