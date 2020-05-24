#ifndef PANEL_TEAMS_H
#define PANEL_TEAMS_H

#include <QFrame>
#include "ui_panel_teams.h"

class GuiRobot;

class PanelTeams : public QFrame, public Ui::PanelTeams
{
    Q_OBJECT

public:
    explicit PanelTeams(QWidget *parent = nullptr);
    ~PanelTeams();

    void update_panel();

public slots:
    void scroll_to_selected(GuiRobot* robot);


};

#endif // PANEL_TEAMS_H
