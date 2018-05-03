#include "include/config/robot_types.h"

/* Robot types are differential, threeWheelOmni fourWheelOmni
 * Change one to have that ID registered as a robot of that type. */

#if 1
const RobotType robotIDTypes[] = {
 /*0*/	fourWheelOmni,
 /*1*/	fourWheelOmni,
 /*2*/	fourWheelOmni,
 /*3*/	fourWheelOmni,
 /*4*/	fourWheelOmni,
 /*5*/	fourWheelOmni,
 /*6*/	fourWheelOmni,
 /*7*/	fourWheelOmni,
 /*8*/	fourWheelOmni,
 /*9*/	fourWheelOmni,
 /*10*/	fourWheelOmni
};
#endif

const bool robotHasKicker[] = {
 /*0*/	true,
 /*1*/	true,
 /*2*/	true,
 /*3*/	true,
 /*4*/	true,
 /*5*/	true,
 /*6*/	false,
 /*7*/	false,
 /*8*/	false,
 /*9*/	false,
 /*10*/	false
};

//Goalie ID integer
float GOALIE_ID = 5;
float DEFEND_1 = 5;
//float DEFEND_1 = 0;  //By Bo
float DEFEND_2 = 3;
//float ATTACK_1 = 4;
float ATTACK_1 = 0;  //By Bo
float ATTACK_2 = 2;
//float ATTACK_2 = 0;    //By Bo
