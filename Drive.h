#ifndef MISC_DEFINED
  #include "Misc.h"
#endif


typedef struct
{
  tMotor left;
  tMotor right;
  tMotor frontLeft;
  tMotor frontRight;
  tMotor backLeft;
  tMotor backRight;
} DriveWheels;

typedef struct
{
  int drivePower;
  int driveClamp;
} DriveData;

typedef struct
{
  DriveWheels wheels;
  DriveData data;
} DriveChassis;



void ArcadeDrive(DriveChassis chassis, int y, int x)
{
  //We only accept a range of -100 to 100. Scale joystick values accordingly.
  y = clamp(y, -100, 100);
  x = clamp(x, -100, 100);
  int left = (y + x) * chassis.data.drivePower / 100;
  int right = (y + -x) * chassis.data.drivePower / 100;
  motor[chassis.wheels.left] = clamp(left, -100, 100);
  motor[chassis.wheels.right] = clamp(right, -100, 100);

  return;
}

//This is apparently "fancier"/"prettier" than normal ArcadeDrive().
//We couldn't tell much of a difference.
//This function was ported from the OpenVex library.
//http://personalpages.tds.net/~jwbacon/OpenVex/
void ArcadeDriveFancy(DriveChassis chassis, int y, int x)
{
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
	  swap(left_power,right_power);
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
	    swap(left_power,right_power);

	  /* Negate for quadrants 3 and 4 */
	  left_power = -left_power;
	  right_power = -right_power;
  }
  motor[chassis.wheels.left] = left_power;
  motor[chassis.wheels.right] = right_power;
}

void TankDrive(DriveChassis chassis, int left, int right)
{
  motor[chassis.wheels.left] = clamp(left * chassis.data.drivePower / 100, -100, 100);
  motor[chassis.wheels.right] = clamp(right * chassis.data.drivePower / 100, -100, 100);

  return;
}

void Init2WheelChassis(DriveChassis &chassis, tMotor left, tMotor right)
{
  chassis.wheels.left = left;
  chassis.wheels.right = right;

  chassis.data.drivePower = 100;


  return;
}

void Init3WheelChassis()
{

}

void Init4WheelChassis(DriveChassis &chassis, tMotor frontLeft, tMotor frontRight, tMotor backLeft, tMotor backRight)
{
  chassis.wheels.frontLeft = frontLeft;
  chassis.wheels.frontRight = frontRight;
  chassis.wheels.backLeft = backLeft;
  chassis.wheels.backRight = backRight;

  chassis.data.drivePower = 100;


  return;
}

void SetDrivePower(DriveChassis &chassis, int power)
{
  chassis.data.drivePower = clamp(power, 1, 100);
}

/*
task main()
{
  DriveChassis chassis;
  Init2WheelChassis(chassis, myLeftWheel, myRightWheel);

  ArcadeDrive(chassis, 70, 20);
  --OR--
  TankDrive(chassis, 20, 70);
}
*/
