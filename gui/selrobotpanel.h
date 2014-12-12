#ifndef SELROBOTPANEL_H
#define SELROBOTPANEL_H

#include <QString>
#include "teamsize.h"

class MainWindow;

class SelRobotPanel
{
public:
    SelRobotPanel(MainWindow * mw);
    MainWindow * dash;
    QString botBehavior[10];

    void setGuiOverride();
    void guiPrintRobot(int robotID, std::string output);
    void setupSelRobotPanel();


public slots:
    void updateSelectedBotPanel(int id);

};

#endif // SELROBOTPANEL_H
