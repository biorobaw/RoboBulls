#ifndef OBJECTPOSITION_H
#define OBJECTPOSITION_H

#include <QString>
#include <QLabel>
#include "mainwindow.h"
#include "utilities/point.h"
#include <deque>
//class Point;

class ObjectPosition
{
public:
    ObjectPosition(MainWindow * mw);
    MainWindow * dash;
    int teamSize = 6;
    int threadTicker = 0;
    // speed stuff
    int speedModifier = 20; // multiplied with units/cycle to get units/second
    int movementMin = 2;    // minimum speed detected which we will consider actual movement
    Point pastPos;
    Point pastBotPoints[6];
    Point newBotPoints[6];
    void setupPastBotPoints();
    void getPastBotPoints();
    void getNewBotPoints();
//    int botSpeeds[6];
    std::deque<int> botSpeeds;
    void setupBotSpeeds();
    int oldSpeeds[6];
    void getBotSpeeds();
    void getOldSpeeds();
//    std::vector<int[]> botSpeedsRecord;  // holds the last several botSpeeds[] arrays
    std::deque<deque<int> > botSpeedsRecord;
    void updateBotSpeedsRecord(); // holds last several botSpeeds deques

//    void getThreadTicker(int tick);   delete
    // position stuff
    QString getBotCoord(int id);        // Returns the specified robot's x/y position as a QString
    QString getBotOrientString(int id);
    double  getBotOrientDouble(bool myTeam, int id);
    int getBotCoordX(bool myTeam, int id);
    int getBotCoordY(bool myTeam,int id);
    float     getBotSpeed(bool myTeam, int id);
    QString getBallCoord();
    int  getBallCoordX();
    int  getBallCoordY();
    // Mouse stuff
    int getMouseCoordX();
    int getMouseCoordY();

};

#endif // OBJECTPOSITION_H
