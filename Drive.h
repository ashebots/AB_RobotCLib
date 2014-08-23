#ifndef MISC_DEFINED
  #include "Misc.h"
#endif

  //---------//
 // GLOBALS //
//---------//

const int AB_defaultEncoderTicksPerRotation = 1440; //Tetrix


  //----------//
 // TYPEDEFS //
//----------//

typedef enum
{
	AB_LINEAR_DRIVE = 1,
	AB_OMNI_DRIVE = 2,
} AB_DriveType;

typedef struct
{
	//Used in conjunction with "left" and "right" for 3 wheel omni drive
	tMotor front;

	//Used on their own for 2 wheel linear drive, or as the middle two for 6 wheel linear drive
	tMotor left;
	tMotor right;

	//Used on their own for 4 wheel linear drive, or 4 wheen omni drive, or with "left" and "right" for 6 wheel linear drive
	tMotor frontLeft;
	tMotor frontRight;
	tMotor backLeft;
	tMotor backRight;
} AB_DriveWheels;

typedef struct
{
  int drivePower;
  int rotationPower;
  int numWheels;
  AB_DriveType driveType;

  float wheelRadius;
  float wheelDiameter;
  float wheelCircumference;

  tMotor encoder; //We currently only support one encoder for things like precision driving.
  int encoderTicksPerRotation;
} AB_DriveData;

typedef struct
{
	AB_DriveWheels wheels;
  AB_DriveData data;
} AB_DriveChassis;


  //-------------------//
 //	DRIVING FUNCTIONS //
//-------------------//

  //        //
 // TELEOP //
//        //

void AB_ArcadeDrive(AB_DriveChassis chassis, int x, int y)
{
	//We only accept a range of -100 to 100. Scale joystick values accordingly. See AB_Scale() and AB_ScaleJoystick().
	if (chassis.data.driveType == AB_LINEAR_DRIVE)
	{
	  y = AB_Clamp(y, -100, 100);
	  x = AB_Clamp(x, -100, 100);
	  int leftSidePower = (y + x) * chassis.data.drivePower / 100;
	  int rightSidePower = (y + -x) * chassis.data.drivePower / 100;
	  leftSidePower = AB_Clamp(leftSidePower, -100, 100);
	  rightSidePower = AB_Clamp(rightSidePower, -100, 100);

	  if ((chassis.data.numWheels == 2) || (chassis.data.numWheels == 6))
	  {
		  motor[chassis.wheels.left] = leftSidePower;
		  motor[chassis.wheels.right] = rightSidePower;
		}
		if ((chassis.data.numWheels == 4) || (chassis.data.numWheels == 6))
		{
			motor[chassis.wheels.frontLeft] = leftSidePower;
		  motor[chassis.wheels.frontRight] = rightSidePower;
		  motor[chassis.wheels.backLeft] = leftSidePower;
		  motor[chassis.wheels.backRight] = rightSidePower;
		}
	}
	else
	{
		PlaySound(soundException);
	}
}

//This is apparently "fancier"/"prettier" than normal ArcadeDrive().
//We couldn't tell much of a difference.
//This function was ported from the OpenVex library.
//http://personalpages.tds.net/~jwbacon/OpenVex/
void AB_ArcadeDriveFancy(AB_DriveChassis chassis, int x, int y)
{
	y = AB_Clamp(y, -100, 100);
  x = AB_Clamp(x, -100, 100);
  int left_power;
	int right_power;
	int abs_x = abs(x);
	int abs_y = abs(y);

  /*
   * Compute left and right motor speeds using simple formula for
   * quadrant 1.
   */
	left_power = abs_y + abs_x * ( 100 - abs_y ) / 100;
	left_power = left_power * chassis.data.drivePower / 100;
	right_power = (abs_y - abs_x);
	right_power = right_power * chassis.data.drivePower / 100;

	if ( y >= 0 )   /* Quadrants 1 and 2 */
    {
	/*
	 *  Left speed and right speed are simply swapped when going from
	 *  x,y to -x,y.  ( Quadrant 1 to 2 )
	 */
	if ( x < 0 )    /* Swap for quadrant 2 */
	  AB_Swap(left_power,right_power);
  }
  else    /* Quadrants 3 and 4 */
  {
		/*
		 *  Left speed and right speed are swapped AND negated when going
		 *  from x,y to x,-y.  ( Quadrant 1 to 4 )
		 *
		 *  Going from x,y to -x,-y (quadrant 1 to 3), motor speeds are
		 *  only negated.
		 */
	  if ( x >= 0 )   /* Swap for quadrant 4 only */
	    AB_Swap(left_power,right_power);

	  /* Negate for quadrants 3 and 4 */
	  left_power = -left_power;
	  right_power = -right_power;
  }
  motor[chassis.wheels.left] = left_power;
  motor[chassis.wheels.right] = right_power;
}

void AB_TankDrive(AB_DriveChassis chassis, int leftInput, int rightInput)
{
	//We only accept a range of -100 to 100. Scale joystick values accordingly. See AB_Scale() and AB_ScaleJoystick().
	if (chassis.data.driveType == AB_LINEAR_DRIVE)
	{
	  leftInput = AB_Clamp(leftInput, -100, 100);
	  rightInput = AB_Clamp(rightInput, -100, 100);
	  int leftSidePower = leftInput * chassis.data.drivePower / 100;
	  int rightSidePower = rightInput * chassis.data.drivePower / 100;
	  leftSidePower = AB_Clamp(leftSidePower, -100, 100);
	  rightSidePower = AB_Clamp(rightSidePower, -100, 100);

	  if ((chassis.data.numWheels == 2) || (chassis.data.numWheels == 6))
	  {
		  motor[chassis.wheels.left] = leftSidePower;
		  motor[chassis.wheels.right] = rightSidePower;
		}
		if ((chassis.data.numWheels == 4) || (chassis.data.numWheels == 6))
		{
			motor[chassis.wheels.frontLeft] = leftSidePower;
		  motor[chassis.wheels.frontRight] = rightSidePower;
		  motor[chassis.wheels.backLeft] = leftSidePower;
		  motor[chassis.wheels.backRight] = rightSidePower;
		}
	}
	else
	{
		PlaySound(soundException);
	}
}

void AB_OmniDrive(AB_DriveChassis chassis, int x, int y, int rotation = 0)
{
	//We only accept a range of -100 to 100. Scale joystick values accordingly. See AB_Scale() and AB_ScaleJoystick().
	if (chassis.data.driveType == AB_OMNI_DRIVE)
	{
		//Code for 4 wheel omni
		if (chassis.data.numWheels == 4)
		{
			x = AB_Clamp(x, -100, 100);
			y = AB_Clamp(y, -100, 100);

			int m1 = (-x) + (-y);
			int m2 = (-x) + (y);
			int m3 = (x) + (-y);
			int m4 = (x) + (y);

			m1 = m1 * chassis.data.drivePower / 100;
			m2 = m2 * chassis.data.drivePower / 100;
			m3 = m3 * chassis.data.drivePower / 100;
			m4 = m4 * chassis.data.drivePower / 100;

			if (rotation != 0) //Only bother to apply rotation if one was supplied
			{
				rotation = rotation * chassis.data.rotationPower / 100; //rotationPower can be a value 0 to 100, set by AB_SetRotationPower(int)

				m1 = m1 - rotation;
				m2 = m2 - rotation;
				m3 = m3 - rotation;
				m4 = m4 - rotation;
			}

		  m1 = AB_Clamp(m1, -100, 100);
			m2 = AB_Clamp(m2, -100, 100);
			m3 = AB_Clamp(m3, -100, 100);
			m4 = AB_Clamp(m4, -100, 100);

			motor[chassis.wheels.frontLeft] = m1;
			motor[chassis.wheels.frontRight] = m2;
			motor[chassis.wheels.backLeft] = m3;
			motor[chassis.wheels.backRight] = m4;
		}
		//Code for 3 wheel omni
		else if (chassis.data.numWheels == 3)
		{
			//3 wheel omnidrive code
		}
	}
	//We weren't given an omni chassis
	else
	{
		PlaySound(soundException);
	}
}

  //            //
 // AUTONOMOUS //
//            //

void AB_StopDriving(AB_DriveChassis &chassis)
{
	motor[chassis.wheels.front] = 0;
	motor[chassis.wheels.left] = 0;
	motor[chassis.wheels.right] = 0;
	motor[chassis.wheels.frontLeft] = 0;
	motor[chassis.wheels.frontRight] = 0;
	motor[chassis.wheels.backLeft] = 0;
	motor[chassis.wheels.backRight] = 0;
}

//Use encoder(s) to drive a precise distance forward
void AB_DriveForDistance(AB_DriveChassis &chassis, float distance, int drivePower)
{
	//Figure out how long we should drive
	float rotationsToTarget = distance / chassis.data.wheelCircumference;
	int encoderTicksToTarget = rotationsToTarget * chassis.data.encoderTicksPerRotation;
	drivePower = AB_Clamp(drivePower, -100, 100);

	//Zero the motor encoder, so we don't accidentally have it wrap around
	nMotorEncoder[chassis.data.encoder] = 0;

	//Start us driving
	if (chassis.data.driveType == AB_LINEAR_DRIVE)
	{
		AB_ArcadeDrive(chassis, 0, drivePower);
	}
	else if (chassis.data.driveType == AB_OMNI_DRIVE)
	{
		encoderTicksToTarget *= 2; //WARNING: Not sure if this is the right thing to do..Trying to compensate for diagonal wheels..
		AB_OmniDrive(chassis, 0, drivePower);
	}
	else
	{
		//We have an unsupported drive type, and should just abort the mission
		PlaySound(soundException);
		return;
	}

	while (abs(nMotorEncoder[chassis.data.encoder]) < encoderTicksToTarget)
	{
		//Do nothing until we get there, we're already moving.
	}

	//We're there! :D
	AB_StopDriving(chassis);
}


  //---------------------//
 //	CHASSIS API THINGY	//
//---------------------//

//DRIVE POWER
void AB_SetDrivePower(AB_DriveChassis &chassis, int power)
{
  chassis.data.drivePower = AB_Clamp(power, 1, 100);
}
int AB_GetDrivePower(AB_DriveChassis &chassis)
{
  return chassis.data.drivePower;
}

//ROTATION POWER
//Only used in holonomic (omniwheel) driving.
void AB_SetRotationPower(AB_DriveChassis &chassis, int power)
{
  chassis.data.rotationPower = AB_Clamp(power, 1, 100);
}
int AB_GetRotationPower(AB_DriveChassis &chassis)
{
  return chassis.data.rotationPower;
}

//DRIVE TYPE
AB_DriveType AB_GetDriveType(AB_DriveChassis &chassis)
{
	return chassis.data.driveType;
}

//NUM WHEELS
int AB_GetNumDriveWheels(AB_DriveChassis &chassis)
{
	return chassis.data.numWheels;
}

//WHEEL SIZE
void AB_SetWheelDiameter(AB_DriveChassis &chassis, float diameter)
{
	chassis.data.wheelRadius = diameter / 2;
	chassis.data.wheelDiameter = diameter;
	chassis.data.wheelCircumference = diameter * PI;
}
float AB_GetWheelRadius(AB_DriveChassis &chassis)
{
	return chassis.data.wheelRadius;
}
float AB_GetWheelDiameter(AB_DriveChassis &chassis)
{
	return chassis.data.wheelDiameter;
}
float AB_GetWheelCircumference(AB_DriveChassis &chassis)
{
	return chassis.data.wheelCircumference;
}

//DRIVE ENCODER
void AB_SetDriveEncoder(AB_DriveChassis &chassis, tMotor encoder)
{
	chassis.data.encoder = encoder;
}


  //------------------------//
 // CHASSIS INITIALIZATION //
//------------------------//

//General settings, applied to all types of chassis
void AB_InitGeneralChassisSettings(AB_DriveChassis &chassis)
{
	chassis.data.drivePower = 100;
	chassis.data.rotationPower = 100;
	chassis.data.encoderTicksPerRotation = AB_defaultEncoderTicksPerRotation;
}

//Linear chassis
void AB_InitLinearChassis(AB_DriveChassis &chassis, tMotor left, tMotor right)
{
  chassis.wheels.left = left;
  chassis.wheels.right = right;
  chassis.data.driveType = AB_LINEAR_DRIVE;
  chassis.data.numWheels = 2;
  AB_InitGeneralChassisSettings(chassis);
  return;
}
void AB_InitLinearChassis(AB_DriveChassis &chassis, tMotor frontLeft, tMotor frontRight, tMotor backLeft, tMotor backRight)
{
  chassis.wheels.frontLeft = frontLeft;
  chassis.wheels.frontRight = frontRight;
  chassis.wheels.backLeft = backLeft;
  chassis.wheels.backRight = backRight;
  chassis.data.driveType = AB_LINEAR_DRIVE;
  chassis.data.numWheels = 4;
  AB_InitGeneralChassisSettings(chassis);
  return;
}
void AB_InitLinearChassis(AB_DriveChassis &chassis, tMotor frontLeft, tMotor frontRight, tMotor middleLeft, tMotor middleRight, tMotor backLeft, tMotor backRight)
{
  chassis.wheels.frontLeft = frontLeft;
  chassis.wheels.frontRight = frontRight;
  chassis.wheels.left = middleLeft;
  chassis.wheels.right = middleRight;
  chassis.wheels.backLeft = backLeft;
  chassis.wheels.backRight = backRight;
  chassis.data.driveType = AB_LINEAR_DRIVE;
  chassis.data.numWheels = 6;
  AB_InitGeneralChassisSettings(chassis);
  return;
}

//Omniwheel chassis
void AB_InitOmniChassis(AB_DriveChassis &chassis, tMotor front, tMotor right, tMotor left)
{
	chassis.wheels.front = front;
	chassis.wheels.left = left;
	chassis.wheels.right = right;
	chassis.data.driveType = AB_OMNI_DRIVE;
	chassis.data.numWheels = 3;
	AB_InitGeneralChassisSettings(chassis);
}
void AB_InitOmniChassis(AB_DriveChassis &chassis, tMotor frontLeft, tMotor frontRight, tMotor backLeft, tMotor backRight)
{
	chassis.wheels.frontLeft = frontLeft;
	chassis.wheels.frontRight = frontRight;
	chassis.wheels.backLeft = backLeft;
	chassis.wheels.backRight = backRight;
	chassis.data.driveType = AB_OMNI_DRIVE;
	chassis.data.numWheels = 4;
	AB_InitGeneralChassisSettings(chassis);
}
