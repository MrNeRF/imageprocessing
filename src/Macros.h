#ifndef MACROS_H
#define MACROS_H

#include <cmath>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>

namespace MathHelper
{
template<typename T>
constexpr T degreeToRadians(const T degree)
{
    T radians = degree * M_PI / 180.f;
    return radians;
}

const double PI = (4.0 * std::atan2(1.0, 1.0));
}

// Checks if a debugger is attached
[[maybe_unused]] static bool CheckForDebugger(void)
{    
  char buf[256];   
  int fd = open("/proc/self/status", O_RDONLY);
  if (fd == -1) 
  {
    return false;  
  }
  const int len = read(fd, buf, sizeof(buf));
  bool rc = false;
  if (len > 0) {
    const char *const debuggerPid = "TracerPid:\t";
    buf[len - 1] = '\0';
    const char *p = strstr(buf, debuggerPid);
    if (p != NULL) {
      rc = (strncmp(p + strlen(debuggerPid), "0\n", 2) != 0);
    }
  }
  close(fd);
  return rc;
}

#ifndef NDEBUG
#include <csignal>
// This macro 
#define ASSERT(cond)																						\
{																											\
	if (!(cond) && CheckForDebugger())																		\
	{																										\
		std::cerr << "ASSERTION FAILED:" << __FILE__ << ", " << __FUNCTION__ << ", Line: " << __LINE__ << '\n';	\
					raise(SIGTRAP);																			\
				}																							\
				else if(!(cond))																			\
				{																							\
		std::cerr << "ASSERTION FAILED:" << __FILE__ << ", " << __FUNCTION__ << ", Line: " << __LINE__ << '\n';	\
	}																										\
}
#else
#define ASSERT(cond)																						\
	if (!(cond))																							\
	{																										\
		std::cerr << "ASSERTION FAILED:" << __FILE__ << ", " << __FUNCTION__ << ", Line: " << __LINE__ << '\n';	\
	}																						
#endif

#endif

