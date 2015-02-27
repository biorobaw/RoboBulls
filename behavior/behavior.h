#ifndef BEHAVIOR_H
#define BEHAVIOR_H

class Robot;

/*
 * General Behavior class which inheritas other behaviors
 * Narges Ghaedi ``et al``
 */
class Behavior
{
public:
    virtual ~Behavior();
    virtual void perform(Robot*) = 0;
    virtual bool isFinished();
};


#endif
