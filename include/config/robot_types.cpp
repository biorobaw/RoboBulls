#include "include/config/robot_types.h"
#include "include/config/simulated.h"

/* Robot types are:
 *- differential
 *- threeWheelOmni
 *- fourWheelOmni
 * Change one to have that ID registered as a robot of that type.
 * Or change the #if option on one of the blocks below
 */

#if 1
const RobotType robotIDTypes[] = {
 /*0*/	fourWheelOmni,
 /*1*/	fourWheelOmni,
 /*2*/	fourWheelOmni,
 /*3*/	fourWheelOmni,
 /*4*/	fourWheelOmni,
#if SIMULATED
 /*5*/	fourWheelOmni,
#else
 /*5*/	threeWheelOmni,
#endif
 /*6*/	fourWheelOmni,
 /*7*/	fourWheelOmni,
 /*8*/	fourWheelOmni,
 /*9*/	fourWheelOmni
};
#endif

#if 0
const robotType robotIDTypes[] = {
 /*0*/	differential,
 /*1*/	differential,
 /*2*/	differential,
 /*3*/	differential,
 /*4*/	differential,
#if SIMULATED
 /*5*/	fourWheelOmni,
#else
 /*5*/	differential,
#endif
 /*6*/	differential,
 /*7*/	differential,
 /*8*/	fourWheelOmni,
 /*9*/	differential
};
#endif

//Goalie ID integer
int GOALIE_ID = 5;
