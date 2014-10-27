#ifndef SELROBOTPANEL_H
#define SELROBOTPANEL_H

#include <QString>
#include "mainwindow.h"

class SelRobotPanel
{
public:
    SelRobotPanel(MainWindow * mw);
    MainWindow * dash;
    QString botBehavior[6];

    void setGuiOverride();
    void guiPrintRobot(int robotID, std::string output);
    void setupSelRobotPanel();


public slots:
    void updateSelectedBotPanel(int id);

};

#endif // SELROBOTPANEL_H
