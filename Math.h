//This is to make sure we don't include the same module twice
#pragma once

/*
=========================================
=THIS FILE IS FOR MATHY FUNCTIONS! WOOO.=
=========================================
*/


//One of these has to be made for everything
//we want to use our AB_FirFilter with.
//Example below.
typedef struct
{
  int size;
  int *history;
  int position;
  bool full;
} AB_FirData;


void AB_InitFirFilter(AB_FirData &data, int *history, int size)
{
  data.size = size;
  data.history = history;
  data.position = 0;
  data.full = false;
}

//http://en.wikipedia.org/wiki/Finite_impulse_response
//Averages a looping array thingy.
//Good for continually averaging things like ultrasonic sensors.
//Example below.
int AB_FirFilter(AB_FirData &data, int input)
{
  data.history[data.position] = input; //Put our new value into the history

  int allValues = 0; //Average
  for (int i=0;i<data.size;i++)
  {
    allValues += data.history[i];
  }
  int average = allValues / data.size;

  if (data.position == data.size) //If we've filled our whole array, it's safe to use it.
  {
    data.full = true; //CHECK THIS EVERYWHERE YOU CHECK THE OUTPUT!
  }

  data.position++;
  if (data.position == data.size)
  {
    data.position = 0; //Increment our position, or wrap it back to zero
  }

  return average; //REMEMBER TO MAKE SURE WE'RE FULL!
}

bool AB_FirFull(AB_FirData data)
{
	return data.full;
}

//FINITE IMPULSE RESPONSE FILTER usage example
/*
task main()
{
  const int range1Size = 100;
  int range1History[ySize];
  AB_FirData range1Data;
  init_AB_FirData(range1Data, range1History, range1Size);

  while (AB_FirFilter(range1Data, SensorValue(RangeFinder1)) < 25 || range1Data.full == false) {}
}
*/
