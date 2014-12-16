#ifndef OBJECTPOSITION_H
#define OBJECTPOSITION_H

#include <QString>
#include "utilities/point.h"
#include <deque>

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
    void setupBotSpeeds();
    void getBotSpeeds();
    void getOldSpeeds();
    void updateBotSpeedsRecord();   // holds last several botSpeeds deques
    int getVelocity(int id);
    int   threadTicker = 0;
    int   speedModifier = 100;      // multiplied with units/cycle to get units/second
    int   movementMin = 2;          // minimum (unmodified) speed detected that we can consider actual movement
    Point pastPos;
    Point pastBotPoints[10];
    Point newBotPoints[10];
    int   oldSpeeds[10];
    std::deque<int> botSpeeds;
    std::deque<deque<int> > botSpeedsRecord;

    // position stuff
    QString getBotCoord(int id);        // Returns the specified robot's x/y position as a QString
    QString getBotOrientString(int id);
    double  getBotOrientDouble(bool myTeam, int id);
    int     getBotCoordX(bool myTeam, int id);
    int     getBotCoordY(bool myTeam,int id);
    float   getBotSpeed(bool myTeam, int id);
    QString getBallCoord();
    int     getBallCoordX();
    int     getBallCoordY();

    // Mouse stuff
    int getMouseCoordX();
    int getMouseCoordY();

    // Behavior parsing
    QStringList getKeyWords(string behavior);
};

#endif // OBJECTPOSITION_H
