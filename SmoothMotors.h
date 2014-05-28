#ifndef MISC_DEFINED
  #include "Misc.h"
#endif

//NOTE: We sorta use motor "power" and motor "speed" interchangeably here. Deal with it.

typedef struct
{
  int initialSpeed;
  int targetSpeed;
  int increments;
  int noChangeWindow;
  int step;
} SmoothMotorData;

int defaultSmoothMotorIncrements = 50;

//This is NOT a percentage. It goes on either side of the target. Make many sense.
//If our SmoothMotor() input target is +/- this of the old one, we don't change it. Not stuff.
int defaultSmoothMotorNoChangeWindow = 5;

SmoothMotorData smoothMotors[35];


//Must be called at the beginning of the user's program. Initializes ere'thang.
void InitSmoothMotors()
{
  for (tMotor i = motorA;/*first motor, 0*/ i <= mtr_S4_C4_2;/*last motor, 34*/ i++)
  {
    smoothMotors[i].increments = defaultSmoothMotorIncrements;
    smoothMotors[i].noChangeWindow = defaultSmoothMotorNoChangeWindow;
    smoothMotors[i].initialSpeed = 0;
    smoothMotors[i].targetSpeed = 0;
    smoothMotors[i].step = 0;
  }
}

//For updating the speed of a single motor.
//Returns true if there is more updating to be done, false if it's finished.
bool UpdateSmoothMotor(tMotor motorToUpdate)
{
	if (motor[motorToUpdate] == smoothMotors[motorToUpdate].targetSpeed)
	{
		return false; //We're at the target speed, so nothing was updated, meaning false.
	}
	//If the absolute difference of the current motor speed and the target motor speed
	//are within one step, jump directly to the target speed.
	else if (AbsDiff(motor[motorToUpdate], smoothMotors[motorToUpdate].targetSpeed) < smoothMotors[motorToUpdate].step)
	{
	  motor[motorToUpdate] = smoothMotors[motorToUpdate].targetSpeed;
	  return false;
	}
	else
	{

		if (smoothMotors[motorToUpdate].targetSpeed > motor[motorToUpdate])
		{
			motor[motorToUpdate] += smoothMotors[motorToUpdate].step;
		}
		else
		{
			motor[motorToUpdate] += -smoothMotors[motorToUpdate].step;
		}
		return true;
	}
}

//For setting a new target for a motor. Will only work if we're at our previously set speed.
bool SmoothMotor(tMotor motorToSet, int target)
{
  if (  (target < (smoothMotors[motorToSet].targetSpeed - smoothMotors[motorToSet].noChangeWindow))
     || (target > (smoothMotors[motorToSet].targetSpeed + smoothMotors[motorToSet].noChangeWindow))
     )
	{ //We've reached the previously set target, and can now set a new one.
		smoothMotors[motorToSet].initialSpeed = motor[motorToSet];
		smoothMotors[motorToSet].targetSpeed = clamp(target, -100, 100);
		smoothMotors[motorToSet].step = AbsDiff(smoothMotors[motorToSet].initialSpeed, smoothMotors[motorToSet].targetSpeed) / smoothMotors[motorToSet].increments;
	}
	return UpdateSmoothMotor(motorToSet);
}

//For updating the speed of all motors.
//Returns true if any motor still has not reached it's target. False if they all have.
bool UpdateSmoothMotors()
{
  bool needsMoreUpdates = false;
	for (tMotor i = motorA;/*first motor, 0*/ i <= mtr_S4_C4_2;/*last motor, 34*/ i++)
  {
    needsMoreUpdates |= UpdateSmoothMotor(i);
  }
  return needsMoreUpdates;
}
