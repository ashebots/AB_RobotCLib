//This is to make sure we don't include the same module twice
#pragma once

typedef struct
{
	float slope;
  int originalPower;
} AB_CalibratedMotorsData;

static AB_CalibratedMotorsData AB_cMotors[35];

int AB_cMotorRPM[35];

int AB_lowestMotorRPM = 32767; //Set it to the highest number possible, just to be sure we don't miss anything as we count down. :3

float AB_defaultDriveCalibrationSlope = 1;


//Must be called at the beginning of the user's program. Initializes ere'thang.
void AB_InitCMotors()
{
	//Find the lowest RPM of our motors
	for (tMotor i = motorA;/*first motor, 0*/ i <= mtr_S4_C4_2;/*last motor, 34*/ i++)
	{
		if (AB_cMotorRPM[i] < AB_lowestMotorRPM && AB_cMotorRPM[i] > 0) //Find the lowest RPM, excluding 0
		{
			AB_lowestMotorRPM = AB_cMotorRPM[i];
		}
	}

	//Set default values, as well as calculate our slopes
	for (tMotor i = motorA;/*first motor, 0*/ i <= mtr_S4_C4_2;/*last motor, 34*/ i++)
  {
    AB_cMotors[i].originalPower = 0;

    //This is assuming that the values of the AB_cMotorRPM array are initialized at 0
    if (AB_cMotorRPM[i] > 0)
    {
    	AB_cMotors[i].slope = AB_lowestMotorRPM / AB_cMotorRPM[i]; //If the lowest RPM is 144, and some motor has 155, that motor's slope would be ~0.929
    }
    else
    {
    	AB_cMotors[i].slope = AB_defaultDriveCalibrationSlope; //If they didn't provide an RPM for a motor, set the slope to 1, so it isn't affected
  	}
  }
}

//Set the power output of a motor, but calibrate it.
//False if invalid motor sent, true if succeeded. Invalid power values will be clamped.
bool AB_CMotor(tMotor motorToSet, int power)
{
  if (motorToSet < motorA || motorToSet > mtr_S4_C4_2)
  {
    return false; //Invalid motor sent..
  }
  power = AB_Clamp(power, -100, 100);

	int powerForMotor;

  AB_cMotors[motorToSet].originalPower = power; //Save the number that we originally set, 'cuz YOLO #SWAG
  powerForMotor = (power * AB_cMotors[motorToSet].slope); //Initial math
  motor[motorToSet] = AB_Clamp(powerForMotor, -100, 100); //Make sure it's a valid number
  return true;
}

//Get the value that was originally set to be calibrated.
int AB_CMotor(tMotor motorToGet)
{
  return AB_cMotors[motorToGet].originalPower;
}
