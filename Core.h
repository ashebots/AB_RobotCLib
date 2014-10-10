#pragma systemFile //This is so the compiler doesn't complain about unused functions
#pragma once //This is to make sure we don't include the same module twice

#define AB_CORE_MODULE


//http://stackoverflow.com/a/1060544
//These are for having something to set reference paramater defaults.
int AB_NULL_INT = 0;
float AB_NULL_FLOAT = 0.0;




/*
#define AB_DEBUG_MESSAGE(message, error_type, prefix = "Debug") { \
	do { \
		if (AB_debugModeEnabled == true) { \
			writeDebugStreamLine(""); \
		} \
  } while(0) \
}
*/
