#ifndef CONFIG_TRACE_H
#define CONFIG_TRACE_H

/* Prints out which behaviors and skills are running 
 * Utilize this macro in behaviors (the perform function)
 * to print out a list of in what order behaviors are being performed.
 * This is a debugging info macro. Additionally, specific module debugging
 * should be under a separate macro defined in the behavior/skill file.
 */
#define TRACE 1

#endif
