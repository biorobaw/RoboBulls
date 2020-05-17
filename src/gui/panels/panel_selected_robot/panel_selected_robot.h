#ifndef PANEL_SELECTED_ROBOT_H
#define PANEL_SELECTED_ROBOT_H

#include <QFrame>

#include "ui_panel_selected_robot.h"

class PanelSelectedRobot : public QFrame, public Ui::PanelSelectedRobot
{
    Q_OBJECT

public:
    explicit PanelSelectedRobot(QWidget *parent = nullptr);
    ~PanelSelectedRobot();

    void hidePanel();


    //Returns name for a robot behavior
    static QStringList getKeyWords(std::string behavior);
    void guiPrintRobot(int robotID, std::string output);
    void printBehavior(int team_id, int robot_id);
    void setupSelRobotPanel();


    QString botBehavior[10] = {""};

public slots:
    void update_selected_robot(int team, int robot);

private:

    int selected_team = -1;
    int selected_robot = -1;

    void update_team_color();

};

#endif // PANEL_SELECTED_ROBOT_H
