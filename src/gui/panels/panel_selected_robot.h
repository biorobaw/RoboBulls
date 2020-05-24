#ifndef PANEL_SELECTED_ROBOT_H
#define PANEL_SELECTED_ROBOT_H

#include <QFrame>
#include "ui_panel_selected_robot.h"


class GraphicsRobot;

// Class defines the frame used by main_window  to display the selected robot
// The class extends the frame defined in panel_selected_robot.ui

class PanelSelectedRobot : public QFrame, public Ui::PanelSelectedRobot
{
    Q_OBJECT

public:
    explicit PanelSelectedRobot(QWidget *parent = nullptr);
    ~PanelSelectedRobot();

    void update_panel(); // updates the information shown in the panel

public slots:
    void update_selected_robot(); // updates the robot associated to the panel
    void on_check_override_stateChanged(int arg1); // processes check box events that control whether to override the robot


private:

    void update_colors(int team); // auxiliary function to update the colors when updating the robot
    QGraphicsScene icon_robot;    // scene to display robot icon
    GraphicsRobot* robot_drawer;  // graphics item to display robot icon

};

#endif // PANEL_SELECTED_ROBOT_H
