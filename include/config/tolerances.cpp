#include "math.h"
#include "include/config/tolerances.h"
#include "include/config/simulated.h"
#include "include/config/globals.h"

#if SIMULATED
 const int ROBOT_RADIUS   = 100;
 const int DIST_TOLERANCE = 40;
 const double ROT_TOLERANCE  = 5*(M_PI/180);
 const double DEF_AREA_TOL = ROBOT_RADIUS + 10;
#else
 const int ROBOT_RADIUS   = 200;
 const int DIST_TOLERANCE = 40;
 const double ROT_TOLERANCE  = 5*(M_PI/180);
 const double DEF_AREA_TOL = ROBOT_RADIUS + 10;
#endif
