#pragma systemFile //This is so the compiler doesn't complain about unused functions
#pragma once //This is to make sure we don't include the same module twice

#define AB_CORE_MODULE


//http://stackoverflow.com/a/1060544
//These are for having something to set reference paramater defaults.
int AB_NULL_INT = 0;
float AB_NULL_FLOAT = 0.0;


//Use these to allow the user to choose what type of operator to use for certain functions.
//List taken from Wikipedia (http://en.wikipedia.org/wiki/Operators_in_C_and_C%2B%2B)
/*
Ex: For the DriveForDistance function, passing AND will wait until both encoders reach the target,
but passing OR will wait until either wheel encoder reaches the target.
*/
typedef enum AB_Operator
{
	//Logical
	NOT,
	AND,
	OR,
} AB_Operator;


/*
#define AB_DEBUG_MESSAGE(message, error_type, prefix = "Debug") { \
	do { \
		if (AB_debugModeEnabled == true) { \
			writeDebugStreamLine(""); \
		} \
  } while(0) \
}
*/
