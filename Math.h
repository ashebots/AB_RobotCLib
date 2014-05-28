/*
=========================================
=THIS FILE IS FOR MATHY FUNCTIONS! WOOO.=
=========================================
*/


//One of these has to be made for everything
//we want to use our fir_filter with.
//Example below.
typedef struct
{
  int size;
  int *history;
  int position;
  bool full;
} fir_data;


void init_fir_data(fir_data &data, int *history, int size)
{
  data.size = size;
  data.history = history;
  //data.history = malloc(data.size * sizeof(int));
  data.position = 0;
  data.full = false;
}

//http://en.wikipedia.org/wiki/Finite_impulse_response
//Averages a looping array thingy.
//Good for continually averaging things like ultrasonic sensors.
//Example below.
int fir_filter(fir_data &data, int input)
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

//FINITE IMPULSE RESPONSE FILTER usage example
/*
task main()
{
  const int range1Size = 100;
  int range1History[ySize];
  fir_data range1Data;
  init_fir_data(range1Data, range1History, range1Size);

  while (fir_filter(range1Data, SensorValue(RangeFinder1)) < 25 || range1Data.full == false) {}
}
*/
