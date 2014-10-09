#ifndef CONFIG_SIMULATED_H
#define CONFIG_SIMULATED_H

/* This configuration macro is used to compile the RoboBulls project
 * using the grSim simulator or the real-world vision cameras.
 * Since both are mostly the same, most changes take place in the
 * /communication module, but you can also use this constant to better-define
 * tolerances such as distance tolerances.
 * define 0 = Not simulated; real world
 * define 1 = Simulated; grSim
 */

#define SIMULATED 0

//bool SIMULATED;

#endif
