#pragma systemFile //This is so the compiler doesn't complain about unused functions
#pragma once //This is to make sure we don't include the same module twice

#define AB_BATTERYMONITOR_MODULE


bool AB_batteryMonitorIsRunning = false;

short AB_batteryMonitorDelay_Passive = 10000; //Milliseconds
short AB_batteryMonitorDelay_Active = 2000; //Milliseconds
short _AB_batteryMonitorDelay = AB_batteryMonitorDelay_Passive;

short AB_batteryMonitorThresholdNXT = 9123; //Millivolts //Made up value, should be tested
short AB_batteryMonitorThresholdTETRIX = 9123; //Millivolts //Made up value, should be tested

task _AB_BatteryMonitor()
{
	while (true) {
		_AB_batteryMonitorDelay = AB_batteryMonitorDelay_Passive;
		//Check the NXT battery level
		if (nAvgBatteryLevel < AB_batteryMonitorThresholdNXT)
		{
			//Maybe send an error through the debug stream here? Might slow down the program, though..
			playSound(soundException);
			_AB_batteryMonitorDelay = AB_batteryMonitorDelay_Active;
		}

		//Check the TETRIX battery level
		if (externalBatteryAvg < AB_batteryMonitorThresholdTETRIX)
		{
			//Maybe send an error through the debug stream here? Might slow down the program, though..
			playSound(soundException);
			_AB_batteryMonitorDelay = AB_batteryMonitorDelay_Active;
		}

		//Figure out how to check SMUX batteries

		wait1Msec(_AB_batteryMonitorDelay);
	}
}

void AB_StartBatteryMonitor()
{
	startTask(_AB_BatteryMonitor);
	AB_batteryMonitorIsRunning = true;
}
void AB_StopBatteryMonitor()
{
	stopTask(_AB_BatteryMonitor);
	AB_batteryMonitorIsRunning = false;
}
