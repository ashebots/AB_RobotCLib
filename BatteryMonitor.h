//CLEANUP THIS FILE SOMETIME

#pragma systemFile //This is so the compiler doesn't complain about unused functions
#pragma once //This is to make sure we don't include the same module twice

#ifndef __COMMON_H__
#include "drivers/common.h"
#endif //__COMMON_H__

#ifndef __HTSMUX_H__
#include "drivers/hitechnic-sensormux.h" //HiTechnic Sensor Multiplexer driver by Xander Soldaat. Link in readme
#endif //__HTSMUX_H__

#define AB_BATTERYMONITOR_MODULE


bool AB_BatteryMonitor_isRunning = false;

short AB_BatteryMonitor_delayPassive = 10000; //Milliseconds
short AB_BatteryMonitor_delayActive = 2000; //Milliseconds
short _AB_BatteryMonitor_delay = AB_BatteryMonitor_delayPassive;

short AB_BatteryMonitor_thresholdNXT = 7500; //Millivolts //Made up value, should be tested
short AB_BatteryMonitor_thresholdTETRIX = 10500; //Millivolts //Made up value, should be tested

//Internal variables for tracking what SMUX(es?) to check batteries of.
tSensors _AB_BatteryMonitor_SMUXList[4];
short _AB_BatteryMonitor_numSMUX = 0;
//Adds an entry to the list of SMUX(es?) to check the batteries of.
void AB_BatteryMonitorAddSMUX(tSensors smux)
{
	if (_AB_BatteryMonitor_numSMUX < 4)
	{
		_AB_BatteryMonitor_SMUXList[_AB_BatteryMonitor_numSMUX] = smux;
		_AB_BatteryMonitor_numSMUX++;
	}
}

task _AB_BatteryMonitor()
{
	while (true) {
		_AB_BatteryMonitor_delay = AB_BatteryMonitor_delayPassive;
		//Check the NXT battery level
		if (nAvgBatteryLevel < AB_BatteryMonitor_thresholdNXT)
		{
			//Maybe send an error through the debug stream here? Might slow down the program, though..
			playSound(soundException);
			_AB_BatteryMonitor_delay = AB_BatteryMonitor_delayActive;
		}

		//Check the TETRIX battery level
		if (externalBatteryAvg < AB_BatteryMonitor_thresholdTETRIX)
		{
			//Maybe send an error through the debug stream here? Might slow down the program, though..
			playSound(soundException);
			_AB_BatteryMonitor_delay = AB_BatteryMonitor_delayActive;
		}

		//Check the power status of all the SMUX(es?) we that are registered
		if (_AB_BatteryMonitor_numSMUX > 0) //If the user has registered any SMUX(es?)
		{
			for (int i=0; i<_AB_BatteryMonitor_numSMUX; i++)
			{
				if (HTSMUXreadPowerStatus(_AB_BatteryMonitor_SMUXList[i]) == true) //True means there is a power problem with the SMUX
				{
					//Maybe send an error through the debug stream here? Might slow down the program, though..
					playSound(soundException);
					_AB_BatteryMonitor_delay = AB_BatteryMonitor_delayActive;
				}
			}
		}

		wait1Msec(_AB_BatteryMonitor_delay);
	}
}

void AB_BatteryMonitor_Start()
{
	startTask(_AB_BatteryMonitor);
	AB_BatteryMonitor_isRunning = true;
}
void AB_BatteryMonitor_Stop()
{
	stopTask(_AB_BatteryMonitor);
	AB_BatteryMonitor_isRunning = false;
}
