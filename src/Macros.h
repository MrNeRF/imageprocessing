#ifndef MACROS_H
#define MACROS_H

#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

// Checks if a debugger is attached
static bool CheckForDebugger(void) 
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


