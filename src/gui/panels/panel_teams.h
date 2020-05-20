#ifndef PANEL_TEAMS_H
#define PANEL_TEAMS_H

#include <QFrame>
#include "ui_panel_teams.h"


class PanelTeams : public QFrame, public Ui::PanelTeams
{
    Q_OBJECT

public:
    explicit PanelTeams(QWidget *parent = nullptr);
    ~PanelTeams();

    void update_panel();

    void show_robot(int team, int id);

//    void PanelTeams::scroll_to_robot(int robot);

private:
};

#endif // PANEL_TEAMS_H
