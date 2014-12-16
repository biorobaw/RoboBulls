#include "include/config/tolerances.h"
#include "include/config/simulated.h"
#include "include/config/globals.h"

#if SIMULATED
 const int ROBOT_RADIUS   = 100;
 const int DIST_TOLERANCE = 80;
 const int ROT_TOLERANCE  = 5*(M_PI/180);
#else
 const int ROBOT_RADIUS   = 200;
 const int DIST_TOLERANCE = 100;
 const int ROT_TOLERANCE  = 10*(M_PI/180);
#endif

 const int ROBOT_SIZE = ROBOT_RADIUS * 2.5;
