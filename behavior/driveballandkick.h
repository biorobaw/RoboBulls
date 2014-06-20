#ifndef DRIVEBALLANDKICK_H
#define DRIVEBALLANDKICK_H

#include "behavior/behavior.h"
#include "utilities/paramlist.h"

using namespace std;

class DriveBallAndKick : public Behavior
{
public:
    DriveBallAndKick(const ParameterList& list);
    void perform(Robot *);

private:
    enum states {driving, kicking, idling} state;
};

#endif // DRIVEBALLANDKICK_H
