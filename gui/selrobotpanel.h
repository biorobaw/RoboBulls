#ifndef SELROBOTPANEL_H
#define SELROBOTPANEL_H

#include <QString>
#include "mainwindow.h"

class SelRobotPanel
{
public:
    SelRobotPanel(MainWindow * mw);
    MainWindow * dash;

    void setGuiOverride();
public slots:
    void updateSelectedBotPanel(int id);

};

#endif // SELROBOTPANEL_H
