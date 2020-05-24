#ifndef PANEL_TEAMS_H
#define PANEL_TEAMS_H

#include <QFrame>
#include "ui_panel_teams.h"

class GuiRobot;

// Class defines the frame used by main_window  to display teams info
// The class extends the frame defined in panel_teams.ui

class PanelTeams : public QFrame, public Ui::PanelTeams
{
    Q_OBJECT

public:
    explicit PanelTeams(QWidget *parent = nullptr);
    ~PanelTeams();

    void update_panel(); // updates the information displayed

public slots:
    void scroll_to_selected(GuiRobot* robot); // switches tabs and scrolls as neccessary to show the given robot
    void override_team(); // overrides all robots on selected tab team
    void release_team();  // relinquish gui control of all robots in selected tab

};

#endif // PANEL_TEAMS_H
