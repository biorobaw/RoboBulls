#include "panel_teams.h"
#include "ui_panel_teams.h"

PanelTeams::PanelTeams(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PanelTeams)
{
    ui->setupUi(this);
}

PanelTeams::~PanelTeams()
{
    delete ui;
}
