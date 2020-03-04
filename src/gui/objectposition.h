#ifndef OBJECTPOSITION_H
#define OBJECTPOSITION_H

#include <QString>
#include <deque>
#include "utilities/point.h"
#include "robot/robot.h"
#include "model/team.h"

using std::string;
using std::deque;
class MainWindow;

class ObjectPosition
{
public:
    ObjectPosition(MainWindow * mw);
    MainWindow * dash;

     // speed stuff
    void setupPastBotPoints();
    void getPastBotPoints();
    void getNewBotPoints();
    void getBotSpeeds();
    void getOldSpeeds();
//    void updateBotSpeedsRecord();   // holds last several botSpeeds deques
    int getVelocity(int id);
    int   threadTicker = 0;
    int   speedModifier = 100;      // multiplied with units/cycle to get units/second
    int   movementMin = 2;          // minimum (unmodified) speed detected that we can consider actual movement
    Point pastPos;
    Point pastBotPoints[10];
    Point newBotPoints[10];
    int   oldSpeeds[10];
    int botSpeeds[2][MAX_ROBOTS] = {{0}};
//    std::deque<int* > botSpeedsRecord;

    // position stuff
//    QString getBotCoord(Robot* robot);        // Returns the specified robot's x/y position as a QString
//    int     getBotCoordY(int myTeam,int id);
//    int     getBotCoordX(int myTeam, int id);

    QString getBotOrientString(Robot* robot);
    double  getBotOrientDouble(Robot* robot);
    float   getBotSpeed(int id);
    int     getBallCoordX();
    int     getBallCoordY();

    // Mouse stuff
    int getMouseCoordX();
    int getMouseCoordY();

    // Behavior parsing
    QStringList getKeyWords(string behavior);
};

#endif // OBJECTPOSITION_H
