#pragma systemFile //This is so the compiler doesn't complain about unused functions
#pragma once //This is to make sure we don't include the same module twice

#define AB_MISC_MODULE

#ifndef AB_CORE_MODULE
#include "Core.h"
#warning "AB_RobotCLib: Please include the core module before including any other module(s)."
#endif

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


//!Returns 0 if input is between min and max, otherwise just returns input.
int AB_Deadzone(int input, int min, int max)
{
	int output = input;
	if ((input > min) && (input < max))
	{
		output = 0;
	}
	return output;
}


//!Clamps a number between a low number and a high number.
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

//!Swaps the values of two variables
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

//! Get the absolute difference between two numbers
int AB_Diff(int num1, int num2){
	return abs(num1 - num2);
}


int AB_FindHighest8( //Should this be called "FindLargest"?
	int num0,
	int num1, //The first two don't need a default value, since we need atleast 2 to find the highest.
	int num2 = 0,
	int num3 = 0,
	int num4 = 0,
	int num5 = 0,
	int num6 = 0,
	int num7 = 0
) {
	const int MAX_NUMBERS = 8;
	int numbers[] = { num0, num1, num2, num3, num4, num5, num6, num7 };

	int highest = 0;

	for (int i = 0; i < MAX_NUMBERS; i++) {
		if (numbers[i] > highest) {
			highest = numbers[i];
		}
	}
	return highest;
}


void AB_ScaleDownTo8( //This could prolly do with a new name
	int scaleTarget,
	int &num0,
	int &num1, //The first two don't need a default value, since we need atleast 2
	int &num2 = AB_NULL_INT,
	int &num3 = AB_NULL_INT,
	int &num4 = AB_NULL_INT,
	int &num5 = AB_NULL_INT,
	int &num6 = AB_NULL_INT,
	int &num7 = AB_NULL_INT
) {
	const int MAX_NUMBERS = 8;
	int &numbers[] = { num0, num1, num2, num3, num4, num5, num6, num7 };

	int scaleFrom = AB_FindHighest8(num0, num1, num2, num3, num4, num5, num6, num7);
	if (scaleFrom > scaleTarget) {
		for (int i = 0; i < MAX_NUMBERS; i++) {
			numbers[i] = AB_Scale(numbers[i], scaleFrom, scaleTarget);
		}
	}
}
