#ifndef GETBEHAVIOR_H
#define GETBEHAVIOR_H

#include "model/robot.h"

class MainWindow;

class GetBehavior
{
public:
    GetBehavior();
    void setupBehaviors();


    const std::string& getBehaviorName(Robot* robot);
    void printBehavior();

private:
    MainWindow * dash;
};

#endif // GETBEHAVIOR_H
