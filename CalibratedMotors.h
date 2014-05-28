
typedef struct
{
	float slope;
  int originalPower;
} ABCalibratedMotorsData;

static ABCalibratedMotorsData ABcMotors[35];

int ABmotorRPM[35];

int ABlowestMotorRPM = 32767; //Set it to the highest number possible, just to be sure we don't miss anything as we count down. :3

float ABdefaultDriveCalibrationSlope = 1;


//Must be called at the beginning of the user's program. Initializes ere'thang.
void InitCMotors()
{
	//Find the lowest RPM of our motors
	for (tMotor i = motorA;/*first motor, 0*/ i <= mtr_S4_C4_2;/*last motor, 34*/ i++)
	{
		if (ABmotorRPM[i] < ABlowestMotorRPM && ABmotorRPM[i] > 0) //Find the lowest RPM, excluding 0
		{
			ABlowestMotorRPM = ABmotorRPM[i];
		}
	}

	//Set default values, as well as calculate our slopes
	for (tMotor i = motorA;/*first motor, 0*/ i <= mtr_S4_C4_2;/*last motor, 34*/ i++)
  {
    ABcMotors[i].originalPower = 0;

    //This is assuming that the values of the ABmotorRPM array are initialized at 0
    if (ABmotorRPM[i] > 0)
    {
    	ABcMotors[i].slope = ABlowestMotorRPM / ABmotorRPM[i]; //If the lowest RPM is 144, and some motor has 155, that motor's slope would be ~0.929
    }
    else
    {
    	ABcMotors[i].slope = ABdefaultDriveCalibrationSlope; //If they didn't provide an RPM for a motor, set the slope to 1, so it isn't affected
  	}
  }
}

//Set the power output of a motor, but calibrate it.
//False if invalid motor sent, true if succeeded. Invalid power values will be clamped.
bool cMotor(tMotor motorToSet, int power)
{
  if (motorToSet < motorA || motorToSet > mtr_S4_C4_2)
  {
    return false; //Invalid motor sent..
  }
  power = clamp(power, -100, 100);

	int powerForMotor;

  ABcMotors[motorToSet].originalPower = power; //Save the number that we originally set, 'cuz YOLO #SWAG
  powerForMotor = (power * ABcMotors[motorToSet].slope); //Initial math
  motor[motorToSet] = clamp(powerForMotor, -100, 100); //Make sure it's a valid number
  return true;
}

//Get the value that was originally set to be calibrated.
int cMotor(tMotor motorToGet)
{
  return ABcMotors[motorToGet].originalPower;
}
