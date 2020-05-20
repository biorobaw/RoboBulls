#ifndef TAB_TEAM_H
#define TAB_TEAM_H

#include <QFrame>

#include "ui_tab_team.h"
#include "model/constants.h"

class FrameRobot;

class TabTeam : public QFrame, public Ui::TabTeam
{
    Q_OBJECT

public:
    explicit TabTeam(QWidget *parent = nullptr);
    ~TabTeam();

    void set_team(int id);
    void update_tab();

    void show_robot(int id);

private:
    int team_id;
    FrameRobot* robot_frames[MAX_ROBOTS_PER_TEAM];

private slots:
    void on_btn_override_all_released();
    void on_btn_override_none_released();

};

#endif // TAB_TEAM_H
