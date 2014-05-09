#include "kick.h"

Kick::Kick()
{
}

void Kick::kickTheBall()
{
    int count = 0;
    roboKick = 1;
    if (count > 4)
        roboKick = 0;
    count++;
}

//void Kick::perform(Robot *rob)
//{

//}
