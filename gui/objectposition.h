#ifndef OBJECTPOSITION_H
#define OBJECTPOSITION_H

#include <QString>
#include <QLabel>
#include "mainwindow.h"

class ObjectPosition
{
public:
    ObjectPosition(MainWindow * mw);
    MainWindow * dash;
    QString getBotCoord(int id);        // Returns the specified robot's x/y position as a QString
    QString getBotOrientString(int id);
    double  getBotOrientDouble(bool myTeam, int id);
    int getBotCoordX(bool myTeam, int id);
    int getBotCoordY(bool myTeam,int id);
    int     getBotSpeed(std::vector<QLabel*> c, int id);
    QString getBallCoord();
    int  getBallCoordX();
    int  getBallCoordY();
    // Mouse stuff
    int getMouseCoordX();
    int getMouseCoordY();

};

#endif // OBJECTPOSITION_H
