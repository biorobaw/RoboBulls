#ifndef GETBEHAVIOR_H
#define GETBEHAVIOR_H
#include <QObject>
#include "model/robot.h"

class MainWindow;

class GetBehavior
{

public:
    GetBehavior(MainWindow * mw);
    MainWindow * dash;
    void setupBehaviors();


    const string& getBehaviorName(Robot* robot);
    void printBehavior();


};

#endif // GETBEHAVIOR_H
