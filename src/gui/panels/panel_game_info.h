#ifndef PANEL_GAME_INFO_H
#define PANEL_GAME_INFO_H

#include <QFrame>
#include <QElapsedTimer>
#include "ui_panel_game_info.h"

class GraphicsBall;

class PanelGameInfo : public QFrame, public Ui::PanelGameInfo
{
    Q_OBJECT

public:
    explicit PanelGameInfo(QWidget *parent = nullptr);
    ~PanelGameInfo();

    void update_panel();

public slots:
    void update_mouse_pos(QPointF mousePos);

private slots:
    void on_btn_connectGui_pressed();

private:
    QElapsedTimer timer;
    const std::string getGameState();
    GraphicsBall* ball_drawer;

signals:
    void toggle_connect_gui();

};
#endif // PANEL_GAME_INFO_H
