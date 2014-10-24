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

};

#endif // SELROBOTPANEL_H
