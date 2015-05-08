#ifndef GETBEHAVIOR_H
#define GETBEHAVIOR_H
#include <string>

class Robot;
class MainWindow;

class GetBehavior
{

public:
    GetBehavior(MainWindow * mw);
    MainWindow * dash;
    void setupBehaviors();


    const std::string& getBehaviorName(Robot* robot);
    void printBehavior();
};

#endif // GETBEHAVIOR_H
