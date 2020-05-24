#ifndef TAB_TEAM_H
#define TAB_TEAM_H

#include <QFrame>

#include "ui_tab_team.h"
#include "model/constants.h"

class FrameRobot;
class GuiRobot;

// Class defines a tab using in class PanelTeams
// It displays a list of all robots in a team along with the team information and related controls
// This class extends the frame defined in tab_team.ui

class TabTeam : public QFrame, public Ui::TabTeam
{
    Q_OBJECT
public:
    explicit TabTeam(QWidget *parent = nullptr);
    ~TabTeam();

    void set_team(int id); // sets the team id
    void update_tab();     // updates the infromation of the team

public slots:
    void show_robot(GuiRobot* robot); // switches between tabs and scrolls as necessary to display the selected robot
    void on_button_release_clicked(); // makes the gui relinquish the control of all robots in the team
    void on_button_override_clicked(); // makes the gui take control (override) of all robots in the team


private:
    int team_id;    // team displayed by this tab
    FrameRobot* robot_frames[MAX_ROBOTS_PER_TEAM]; // the frames of all robots

};

#endif // TAB_TEAM_H
