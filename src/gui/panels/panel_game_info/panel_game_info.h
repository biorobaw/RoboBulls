#ifndef PANEL_GAME_INFO_H
#define PANEL_GAME_INFO_H

#include <QFrame>
#include <QElapsedTimer>
#include "ui_panel_game_info.h"


class PanelGameInfo : public QFrame, public Ui::PanelGameInfo
{
    Q_OBJECT

public:
    explicit PanelGameInfo(QWidget *parent = nullptr);
    ~PanelGameInfo();

    void update_data();
    void update_mouse_position(QPointF pos);
    void update_ball_position();
    void update_clock();

public slots:
    void on_btn_connectGui_clicked();

private:
    QElapsedTimer timer;
};

#endif // PANEL_GAME_INFO_H
