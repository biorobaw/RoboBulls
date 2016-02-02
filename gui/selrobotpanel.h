#ifndef SELROBOTPANEL_H
#define SELROBOTPANEL_H
#include <QString>

class Robot;
class MainWindow;

//The panel at the top left when a robot is selected

class SelRobotPanel
{
public:
    SelRobotPanel(MainWindow * mw);
    MainWindow * dash;
    QString botBehavior[10];

    void guiPrintRobot(int robotID, std::string output);
    void printBehavior(int id);
    void setupSelRobotPanel();
    void hide();    //Hides the selected bot panel and resets selected/overridden bot

public slots:
    void updateSelectedBotPanel(int id);

private:
    //Returns the name for a robot's behavior
    const std::string& getBehaviorName(Robot* robot);
};

#endif // SELROBOTPANEL_H
