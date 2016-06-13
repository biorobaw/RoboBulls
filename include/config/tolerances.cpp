#include "math.h"
#include "include/config/tolerances.h"
#include "include/config/simulated.h"
#include "include/config/globals.h"

#if SIMULATED
 const int ROBOT_RADIUS   = 100;
 const int DIST_TOLERANCE = 40;
 const double ROT_TOLERANCE  = 5.0*(M_PI/180);
 const double DEF_AREA_TOL = ROBOT_RADIUS + 10;

 const double DRIBBLE_FRWD_SPD = 70;
 const double DRIBBLE_BACK_SPD = 7;
 const double DRIBBLE_TURN_RATE = 0.1;
#else
 const int ROBOT_RADIUS   = 120;
 const int DIST_TOLERANCE = 40;
 const double ROT_TOLERANCE  = 5*(M_PI/180);
 const double DEF_AREA_TOL = ROBOT_RADIUS + 10;

 const double DRIBBLE_FRWD_SPD = 70;
 const double DRIBBLE_BACK_SPD = 7;
 const double DRIBBLE_TURN_RATE = 0.1;
#endif
