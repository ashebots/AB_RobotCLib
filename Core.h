//This is to make sure we don't include the same module twice
#pragma once

#define AB_DEBUG_MESSAGE(message, error_type, prefix = "Debug") { \
	do { \
		if (AB_debugModeEnabled == true) { \
			writeDebugStreamLine(""); \
		} \
  } while(0) \
}
