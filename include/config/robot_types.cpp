#include "include/config/robot_types.h"

/* Robot types are:
 *- differential
 *- threeWheelOmni
 *- fourWheelOmni
 * Change one to have that ID registered as a robot of that type.
 * Or change the #if option on one of the blocks below
 */

#if 1
const robotType robotIDTypes[] = {
 /*0*/	fourWheelOmni,
 /*1*/	differential,
 /*2*/	differential,
 /*3*/	differential,
 /*4*/	differential,
 /*5*/	threeWheelOmni,
 /*6*/	differential,
 /*7*/	differential,
 /*8*/	differential,
 /*9*/	differential
};
#endif

#if 0
const robotType robotIDTypes[] = {
 /*0*/	fourWheelOmni,
 /*1*/	fourWheelOmni,
 /*2*/	fourWheelOmni,
 /*3*/	fourWheelOmni,
 /*4*/	fourWheelOmni,
 /*5*/	fourWheelOmni,
 /*6*/	fourWheelOmni,
 /*7*/	fourWheelOmni,
 /*8*/	fourWheelOmni,
 /*9*/	fourWheelOmni
};
#endif

#if 0
const robotType robotIDTypes[] = {
 /*0*/	threeWheelOmni,
 /*1*/	threeWheelOmni,
 /*2*/	threeWheelOmni,
 /*3*/	threeWheelOmni,
 /*4*/	threeWheelOmni,
 /*5*/	threeWheelOmni,
 /*6*/	threeWheelOmni,
 /*7*/	threeWheelOmni,
 /*8*/	threeWheelOmni,
 /*9*/	threeWheelOmni,
};
#endif
