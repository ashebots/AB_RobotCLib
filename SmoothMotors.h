#pragma systemFile //This is so the compiler doesn't complain about unused functions
#pragma once //This is to make sure we don't include the same module twice

//Make sure we load the Misc module
#include "Misc.h"

//NOTE: We sorta use motor "power" and motor "speed" interchangeably here. Deal with it.

typedef struct
{
  int initialSpeed;
  int targetSpeed;
  int increments;
  int noChangeWindow;
  int step;
} AB_SmoothMotorData;

int AB_defaultSmoothMotorIncrements = 50;

//This is NOT a percentage. It goes on either side of the target. Make many sense.
//If our AB_SmoothMotor() input target is +/- this of the old one, we don't change it. Not stuff.
int AB_defaultSmoothMotorNoChangeWindow = 5;

AB_SmoothMotorData AB_smoothMotors[35];


//Must be called at the beginning of the user's program. Initializes ere'thang.
void AB_InitSmoothMotors()
{
  for (tMotor i = motorA;/*first motor, 0*/ i <= mtr_S4_C4_2;/*last motor, 34*/ i++)
  {
    AB_smoothMotors[i].increments = AB_defaultSmoothMotorIncrements;
    AB_smoothMotors[i].noChangeWindow = AB_defaultSmoothMotorNoChangeWindow;
    AB_smoothMotors[i].initialSpeed = 0;
    AB_smoothMotors[i].targetSpeed = 0;
    AB_smoothMotors[i].step = 0;
  }
}

//For updating the speed of a single motor.
//Returns true if there is more updating to be done, false if it's finished.
bool AB_UpdateSmoothMotor(tMotor motorToUpdate)
{
	if (motor[motorToUpdate] == AB_smoothMotors[motorToUpdate].targetSpeed)
	{
		return false; //We're at the target speed, so nothing was updated, meaning false.
	}
	//If the absolute difference of the current motor speed and the target motor speed
	//are within one step, jump directly to the target speed.
	else if (AB_Diff(motor[motorToUpdate], AB_smoothMotors[motorToUpdate].targetSpeed) < AB_smoothMotors[motorToUpdate].step)
	{
	  motor[motorToUpdate] = AB_smoothMotors[motorToUpdate].targetSpeed;
	  return false;
	}
	else
	{

		if (AB_smoothMotors[motorToUpdate].targetSpeed > motor[motorToUpdate])
		{
			motor[motorToUpdate] += AB_smoothMotors[motorToUpdate].step;
		}
		else
		{
			motor[motorToUpdate] += -AB_smoothMotors[motorToUpdate].step;
		}
		return true;
	}
}

//For setting a new target for a motor. Will only work if we're at our previously set speed.
bool AB_SmoothMotor(tMotor motorToSet, int target)
{
  if (  (target < (AB_smoothMotors[motorToSet].targetSpeed - AB_smoothMotors[motorToSet].noChangeWindow))
     || (target > (AB_smoothMotors[motorToSet].targetSpeed + AB_smoothMotors[motorToSet].noChangeWindow))
     )
	{ //We've reached the previously set target, and can now set a new one.
		AB_smoothMotors[motorToSet].initialSpeed = motor[motorToSet];
		AB_smoothMotors[motorToSet].targetSpeed = AB_Clamp(target, -100, 100);
		AB_smoothMotors[motorToSet].step = AB_Diff(AB_smoothMotors[motorToSet].initialSpeed, AB_smoothMotors[motorToSet].targetSpeed) / AB_smoothMotors[motorToSet].increments;
	}
	return AB_UpdateSmoothMotor(motorToSet);
}

//For updating the speed of all motors.
//Returns true if any motor still has not reached it's target. False if they all have.
bool AB_UpdateSmoothMotors()
{
  bool needsMoreUpdates = false;
	for (tMotor i = motorA;/*first motor, 0*/ i <= mtr_S4_C4_2;/*last motor, 34*/ i++)
  {
    needsMoreUpdates |= AB_UpdateSmoothMotor(i);
  }
  return needsMoreUpdates;
}
