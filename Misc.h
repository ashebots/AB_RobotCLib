#pragma systemFile //This is so the compiler doesn't complain about unused functions
#pragma once //This is to make sure we don't include the same module twice

#define AB_MISC_MODULE

#ifndef AB_CORE_MODULE
#include "Core.h"
#warning "AB_RobotCLib: Please include the Core.h module before including any other module(s)."
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


/*!
Searches through a list of numbers and returns the one farthest from zero.

If a list contains [10, -50, -150, 110], it will return -150.
*/
int AB_FindLargest_8(
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

	int largest = 0;

	for (int i = 0; i < MAX_NUMBERS; i++) {
		if (abs(numbers[i]) > abs(largest)) {
			largest = numbers[i];
		}
	}
	return largest;
}


/*!
Scales a list of numbers down to fit within rangeMax (while still maintaining their relative position to eachother)
if the ABSOLUTE value of one of numbers is greater than rangeMax.

Ex1: Given a rangeMax of 100, [-110, 100, -90, 73, 13] would be scaled down to [-100, 90, -81, 66, 11]
Ex2: Given a rangeMax of 100, [-100, 90, -81, 66, 11] would remain untouched, since none of the ABSOLUTE values are greater than rangeMax.
*/
void AB_ScaleIntoRange_8( //This could prolly do with a new name
	int rangeMax,
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

	rangeMax = abs(rangeMax);

	int scaleFrom = abs(AB_FindLargest_8(num0, num1, num2, num3, num4, num5, num6, num7));
	if (scaleFrom > rangeMax) {
		for (int i = 0; i < MAX_NUMBERS; i++) {
			numbers[i] = AB_Scale(numbers[i], scaleFrom, rangeMax);
		}
	}
}

//ToDo: Document this!
float AB_MatchArrays(ubyte *profile, ubyte *dataset, int length, int range = 0, int leniency = 0)
{
	writeDebugStreamLine("In function <MatchArrays>");
	if (range < 0)
		range = 0; //replace this with clamp?

	range++; //Range has to be a minimum of 1, but that makes it seem like it looks for 1 on either side of the number, which it does not.

	float numMatches = 0.0;
	for (int index=0; index<length; index++)
	{
		//Check the number at the current index of the new dataset against (leniency*2)+1 number of numbers surrounding the corresponding index in the data profile.
		//e.g. if our leniency=4, and our range=5, if we're looking at number dataset[13] in our new dataset, we check if it falls within <range>(5) of ANY of the numbers profile[13 - leniency] and profile[13 + leniency] (9 to 17 in this case)

		float bestRangeMatch = 0.0;
		int bestRangeMatchDistanceFromIndex = 0;

		int leniencyIndex = 0; //Remove this declaration
		int indexAtMatch; //for debug
		for (leniencyIndex=(index-leniency); leniencyIndex<=(index+leniency); leniencyIndex++)
		{
			float rangeMatch = 1.0 / range * -AB_Diff(dataset[index], profile[leniencyIndex]) + 1.0; //Will decrease from 1 to 0 the farther away the match is, 1 being perfect number match, 0 being outside of range.

			if (rangeMatch > bestRangeMatch)
			{
				bestRangeMatch = rangeMatch;
				bestRangeMatchDistanceFromIndex = AB_Diff(leniencyIndex, index);
				indexAtMatch = leniencyIndex; //for debug
			}
		}
		float leniencyMatch = 1.0 / range * -(bestRangeMatchDistanceFromIndex/4.0) + 1.0;

		float amountMatched = (bestRangeMatch * leniencyMatch);
		if (amountMatched != 0)
		{
			writeDebugStreamLine("Match found for dataset[%d] (%d) at profile[%d] (%d) where bestRangeMatch=%f and leniencyMatch=%f", index, dataset[index], indexAtMatch, profile[indexAtMatch], bestRangeMatch, leniencyMatch);
		}
		numMatches += amountMatched;
	}

	float percentMatched = numMatches * 100.0 / length;
	return percentMatched;
}

//ToDo: Document this!
bool AB_LoadArray(ubyte *datasetArray, short datasetLength, string fileName)
{
	writeDebugStreamLine("In function <LoadArray>");

	fileName = fileName + ".txt";

	TFileHandle fileHandle;
	TFileIOResult ioResult;
  short fileSize = 0;
	OpenRead(fileHandle, ioResult, fileName, fileSize);

	if (ioResult == 0)
	{
		writeDebugStreamLine("Opened file");
		short arrayIndex = 0;
		for (int i=0; i<(fileSize/sizeof(ubyte)); i++)
		{
			if (arrayIndex < datasetLength) {
				ubyte readResult = 0;
				ReadByte(fileHandle, ioResult, readResult);
				if (ioResult == 0)
				{
					writeDebugStreamLine("ReadByte #%d", i);
					datasetArray[i] = readResult;
				}
				else
				{
					//Read error, abort
					Close(fileHandle, ioResult);
					return false;
				}
			}
			else
			{
				//Array is full
				Close(fileHandle, ioResult);
				return true;
			}

			arrayIndex++;
		}
		//Read whole file successfully..hopefully
		Close(fileHandle, ioResult);
		return true;
	}
	else
	{
		//Couldn't open file
		Close(fileHandle, ioResult);
		return false;
	}
}

//ToDo: Document this!
bool AB_SaveArray(ubyte *datasetArray, short datasetLength, string fileName, bool force = true)
{
	writeDebugStreamLine("In function <SaveArray>");

	fileName = fileName + ".txt";

	TFileIOResult ioResult;

	//Will delete the file first if it exists
	if (force == true)
	{
		Delete(fileName, ioResult);
	}

	TFileHandle fileHandle;
  short fileSize = datasetLength * sizeof(ubyte);

	OpenWrite(fileHandle, ioResult, fileName, fileSize);
	if (ioResult == 0)
	{
		for (short i=0; i<datasetLength; i++)
		{
			WriteByte(fileHandle, ioResult, datasetArray[i]);
			if (ioResult != 0)
			{
				//Error saving to file
				Close(fileHandle, ioResult);
				return false;
			}
		}
		//Hopefully wrote everything successfully
		Close(fileHandle, ioResult);
		return true;
	}
	else
	{
		//Couldn't open file for writing
		Close(fileHandle, ioResult);
		return false;
	}
}


//By Xander Soldaat @BotBench // http://botbench.com/blog/2012/07/21/tutorial-sorting-your-data/
//Shell sort, sorts low to high
void AB_Sort(int *array, int length)
{
	int mid;
	for(int m = length/2 ; m > 0; m /= 2)
	{
	  for(int j = m; j < length; j++)
	  {
	    for(int i = j - m; i >= 0; i -= m)
	    {
	      if(array[i + m] >= array[i])
	        break;
	      else
	      {
	        mid = array[i];
	        array[i] = array[i + m];
	        array[i + m] = mid;
	      }
	    }
	  }
	}
}
