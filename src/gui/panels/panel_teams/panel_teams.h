#ifndef PANEL_TEAMS_H
#define PANEL_TEAMS_H

#include <QFrame>

namespace Ui {
class PanelTeams;
}

class PanelTeams : public QFrame
{
    Q_OBJECT

public:
    explicit PanelTeams(QWidget *parent = nullptr);
    ~PanelTeams();

private:
    Ui::PanelTeams *ui;
};

#endif // PANEL_TEAMS_H
