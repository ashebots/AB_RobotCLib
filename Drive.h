#pragma systemFile //This is so the compiler doesn't complain about unused functions
#pragma once //This is to make sure we don't include the same module twice

//Make sure the Misc module is loaded
#include "Misc.h"

  //---------//
 // GLOBALS //
//---------//

const int AB_defaultEncoderTicksPerRotation = 1440; //Tetrix


  //----------//
 // TYPEDEFS //
//----------//

typedef enum AB_DriveType
{
	AB_TANK_DRIVE, //Differential drive. 2, 4, or 6 wheels, all facing one direction

	AB_KIWI_OMNI_DRIVE, //3 wheels in triangle, at 120 degrees
	AB_ORDINAL_OMNI_DRIVE, //4 wheels on corners, at 45 degrees
	AB_CARDINAL_OMNI_DRIVE, //4 wheels on edges, at 90 degrees
	AB_SLIDE_OMNI_DRIVE, //4+ wheels on on sides all facing one direction, then 1+ in the middle perpendicular

	AB_MECANUM_DRIVE,

	AB_NUM_DRIVETYPES
};

typedef struct
{
	//Used in Cardinal omni drive
	tMotor back;

	//Used in Kiwi and Cardinal omni drive
	tMotor front;

	//Used in 2 and 6 wheel tank drive, as well as Kiwi and Cardinal omni drive
	tMotor left;
	tMotor right;

	//Used in 4 and 6 wheel tank drive, as well as Ordinal omni drive
	tMotor frontLeft;
	tMotor frontRight;
	tMotor backLeft;
	tMotor backRight;
} AB_DriveWheels;

typedef struct
{
  int drivePower;
  int rotationPower;
  int numMotors;
  AB_DriveType driveType;

  //Size of drive wheels
  //It doesn't matter what unit of measurement is used, it just needs to be consistent
  float wheelRadius;
  float wheelDiameter;
  float wheelCircumference;
  //Distance between the drive wheels //DESCRIBE DIS MOE BETTA
  float drivebaseDiameter;
  float drivebaseCircumference;

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
	if (chassis.data.driveType == AB_TANK_DRIVE)
	{
	  y = AB_Clamp(y, -100, 100);
	  x = AB_Clamp(x, -100, 100);
	  int leftSidePower = (y + x) * chassis.data.drivePower / 100;
	  int rightSidePower = (y + -x) * chassis.data.drivePower / 100;
	  leftSidePower = AB_Clamp(leftSidePower, -100, 100);
	  rightSidePower = AB_Clamp(rightSidePower, -100, 100);

	  if ((chassis.data.numMotors == 2) || (chassis.data.numMotors == 6))
	  {
		  motor[chassis.wheels.left] = leftSidePower;
		  motor[chassis.wheels.right] = rightSidePower;
		}
		if ((chassis.data.numMotors == 4) || (chassis.data.numMotors == 6))
		{
			motor[chassis.wheels.frontLeft] = leftSidePower;
		  motor[chassis.wheels.frontRight] = rightSidePower;
		  motor[chassis.wheels.backLeft] = leftSidePower;
		  motor[chassis.wheels.backRight] = rightSidePower;
		}
	}
	else
	{
		playSound(soundException);
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
	int leftSidePower;
	int rightSidePower;
	//We only accept a range of -100 to 100. Scale joystick values accordingly. See AB_Scale() and AB_ScaleJoystick().
	if (chassis.data.driveType == AB_TANK_DRIVE)
	{
	  leftInput = AB_Clamp(leftInput, -100, 100);
	  rightInput = AB_Clamp(rightInput, -100, 100);
	  leftSidePower = leftInput * chassis.data.drivePower / 100;
	  rightSidePower = rightInput * chassis.data.drivePower / 100;
	  leftSidePower = AB_Clamp(leftSidePower, -100, 100);
	  rightSidePower = AB_Clamp(rightSidePower, -100, 100);

	  //Debug
	  writeDebugStreamLine("--------------------");
	  writeDebugStreamLine("leftInput = %d", leftInput);
	  writeDebugStreamLine("rightInput = %d", rightInput);
	  writeDebugStreamLine("leftSidePower = %d", leftSidePower);
	  writeDebugStreamLine("rightSidePower = %d", rightSidePower);

	  if ((chassis.data.numMotors == 2) || (chassis.data.numMotors == 6))
	  {
		  motor[chassis.wheels.left] = leftSidePower;
		  motor[chassis.wheels.right] = rightSidePower;
		}
		if ((chassis.data.numMotors == 4) || (chassis.data.numMotors == 6))
		{
			motor[chassis.wheels.frontLeft] = leftSidePower;
		  motor[chassis.wheels.frontRight] = rightSidePower;
		  motor[chassis.wheels.backLeft] = leftSidePower;
		  motor[chassis.wheels.backRight] = rightSidePower;
		}
	}
	else
	{
		playSound(soundException);
	}
}

void AB_OmniDrive(AB_DriveChassis chassis, int x, int y, int rotation = 0)
{
	//We only accept a range of -100 to 100. Scale joystick values accordingly. See AB_Scale() and AB_ScaleJoystick().
	if ((chassis.data.driveType == AB_ORDINAL_OMNI_DRIVE) || (chassis.data.driveType == AB_CARDINAL_OMNI_DRIVE))
	{
		x = AB_Clamp(x, -100, 100);
		y = AB_Clamp(y, -100, 100);

		int m1;
		int m2;
		int m3;
		int m4;

		if (chassis.data.driveType == AB_ORDINAL_OMNI_DRIVE)
		{
			m1 = (-x) + (-y);
			m2 = (-x) + (y);
			m3 = (x) + (-y);
			m4 = (x) + (y);
		}
		else //CARDINAL
		{
		}

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

		if (chassis.data.driveType == AB_ORDINAL_OMNI_DRIVE)
		{
			motor[chassis.wheels.frontLeft] = m1;
			motor[chassis.wheels.frontRight] = m2;
			motor[chassis.wheels.backLeft] = m3;
			motor[chassis.wheels.backRight] = m4;
		}
		else
		{
			motor[chassis.wheels.front] = m1;
			motor[chassis.wheels.left] = m2;
			motor[chassis.wheels.right] = m3;
			motor[chassis.wheels.back] = m4;
		}
	}
	//We weren't given an omni chassis
	else
	{
		playSound(soundException);
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


int AB_CalcEncoderTicksForDistance(AB_DriveChassis &chassis, float distance)
{
	float rotationsToTarget = distance / chassis.data.wheelCircumference;
	return (int) (rotationsToTarget * chassis.data.encoderTicksPerRotation);
}

//Use encoder(s) to drive a precise distance forward
void AB_DriveForDistance(AB_DriveChassis &chassis, float distance, int drivePower)
{
	//Sanitize input
	drivePower = AB_Clamp(drivePower, -100, 100);
	drivePower = drivePower * sgn(distance); //Inverts drivePower if distance is negative

	//Figure out how long we should drive
	int encoderTicksToTarget = AB_CalcEncoderTicksForDistance(chassis, distance);

	//WARNING: THIS WILL ONLY WORK WITH A 2 MOTOR TANK DRIVE ROBOT, WITH ENCODERS ON BOTH WHEELS
	//YEAH, I'M LAZY. DEAL WITH IT WHEN WE NEED IT.

	nMotorEncoder[chassis.wheels.left] = 0;
	nMotorEncoder[chassis.wheels.right] = 0;

	nMotorEncoderTarget[chassis.wheels.left] = encoderTicksToTarget;
	nMotorEncoderTarget[chassis.wheels.right] = encoderTicksToTarget;

	AB_TankDrive(chassis, drivePower, drivePower);

	while ((nMotorRunState[chassis.wheels.left] != runStateIdle) || (nMotorRunState[chassis.wheels.right] != runStateIdle))
	{
		EndTimeSlice();
	}

	AB_StopDriving(chassis);
}

//Use encoder(s) to rotate the robot a 'precise' number of degrees.
//A positve degree value rotates the robot to the right, a negative value rotates left. Just like a compass
//360 degrees is equal to 1 rotation. Just like a compass
//Ex: a value of -900 would rotate the robot 2 and a half times to the left
void AB_RotateDegrees(AB_DriveChassis &chassis, int degrees, int drivePower)
{
	//Sanitize input
	drivePower = AB_Clamp(drivePower, -100, 100);

	//Figure out how much to rotate
	float encoderTicksToTarget = chassis.data.drivebaseCircumference / chassis.data.wheelCircumference;
	encoderTicksToTarget /= (360.0 / abs(degrees));
	encoderTicksToTarget *= chassis.data.encoderTicksPerRotation;

	//WARNING: THIS WILL ONLY WORK WITH A 2 MOTOR TANK DRIVE ROBOT, WITH ENCODERS ON BOTH WHEELS
	//YEAH, I'M LAZY. DEAL WITH IT WHEN WE NEED IT.

	nMotorEncoder[chassis.wheels.left] = 0;
	nMotorEncoder[chassis.wheels.right] = 0;

	nMotorEncoderTarget[chassis.wheels.left] = encoderTicksToTarget;
	nMotorEncoderTarget[chassis.wheels.right] = encoderTicksToTarget;

	int rotationPower = drivePower * sgn(degrees); //whether we should turn left or right
	AB_TankDrive(chassis, rotationPower, -rotationPower);

	while ((nMotorRunState[chassis.wheels.left] != runStateIdle) || (nMotorRunState[chassis.wheels.right] != runStateIdle))
	{
		EndTimeSlice();
	}

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
//Only used in omniwheel driving.
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

//NUM MOTORS
int AB_GetNumDriveMotors(AB_DriveChassis &chassis)
{
	return chassis.data.numMotors;
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

//DRIVE BASE SIZE
void AB_SetDrivebaseDiameter(AB_DriveChassis &chassis, float distance)
{
	chassis.data.drivebaseDiameter = distance;
	chassis.data.drivebaseCircumference = distance * PI;
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
void AB_InitTankChassis(AB_DriveChassis &chassis, tMotor left, tMotor right)
{
  chassis.wheels.left = left;
  chassis.wheels.right = right;
  chassis.data.driveType = AB_TANK_DRIVE;
  chassis.data.numMotors = 2;
  AB_InitGeneralChassisSettings(chassis);
  return;
}
void AB_InitTankChassis(AB_DriveChassis &chassis, tMotor frontLeft, tMotor frontRight, tMotor backLeft, tMotor backRight)
{
  chassis.wheels.frontLeft = frontLeft;
  chassis.wheels.frontRight = frontRight;
  chassis.wheels.backLeft = backLeft;
  chassis.wheels.backRight = backRight;
  chassis.data.driveType = AB_TANK_DRIVE;
  chassis.data.numMotors = 4;
  AB_InitGeneralChassisSettings(chassis);
  return;
}
void AB_InitTankChassis(AB_DriveChassis &chassis, tMotor frontLeft, tMotor frontRight, tMotor middleLeft, tMotor middleRight, tMotor backLeft, tMotor backRight)
{
  chassis.wheels.frontLeft = frontLeft;
  chassis.wheels.frontRight = frontRight;
  chassis.wheels.left = middleLeft;
  chassis.wheels.right = middleRight;
  chassis.wheels.backLeft = backLeft;
  chassis.wheels.backRight = backRight;
  chassis.data.driveType = AB_TANK_DRIVE;
  chassis.data.numMotors = 6;
  AB_InitGeneralChassisSettings(chassis);
  return;
}

//Omniwheel chassis
void AB_InitKiwiOmniChassis(AB_DriveChassis &chassis, tMotor front, tMotor right, tMotor left)
{
	chassis.wheels.front = front;
	chassis.wheels.left = left;
	chassis.wheels.right = right;
	chassis.data.driveType = AB_KIWI_OMNI_DRIVE;
	chassis.data.numMotors = 3;
	AB_InitGeneralChassisSettings(chassis);
}
void AB_InitOrdinalOmniChassis(AB_DriveChassis &chassis, tMotor frontLeft, tMotor frontRight, tMotor backLeft, tMotor backRight)
{
	chassis.wheels.frontLeft = frontLeft;
	chassis.wheels.frontRight = frontRight;
	chassis.wheels.backLeft = backLeft;
	chassis.wheels.backRight = backRight;
	chassis.data.driveType = AB_ORDINAL_OMNI_DRIVE;
	chassis.data.numMotors = 4;
	AB_InitGeneralChassisSettings(chassis);
}
void AB_InitCardinalOmniChassis(AB_DriveChassis &chassis, tMotor frontLeft, tMotor frontRight, tMotor backLeft, tMotor backRight)
{
	chassis.wheels.frontLeft = frontLeft;
	chassis.wheels.frontRight = frontRight;
	chassis.wheels.backLeft = backLeft;
	chassis.wheels.backRight = backRight;
	chassis.data.driveType = AB_CARDINAL_OMNI_DRIVE;
	chassis.data.numMotors = 4;
	AB_InitGeneralChassisSettings(chassis);
}
