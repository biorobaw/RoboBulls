#ifndef PANEL_SELECTED_ROBOT_H
#define PANEL_SELECTED_ROBOT_H

#include <QFrame>

#include "ui_panel_selected_robot.h"

class GraphicsRobot;

class PanelSelectedRobot : public QFrame, public Ui::PanelSelectedRobot
{
    Q_OBJECT

public:
    explicit PanelSelectedRobot(QWidget *parent = nullptr);
    ~PanelSelectedRobot();

    void update_panel();

public slots:
    void update_selected_robot();

private:

    void update_team_color(int team);
    QGraphicsScene icon_robot;
    GraphicsRobot* robot_drawer;

private slots:
    void on_check_botOverride_clicked(bool check);


};

#endif // PANEL_SELECTED_ROBOT_H
