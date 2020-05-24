#ifndef PANEL_GAME_INFO_H
#define PANEL_GAME_INFO_H

#include <QFrame>
#include <QElapsedTimer>
#include "ui_panel_game_info.h"

class GraphicsBall;

// Class defines the frame used by main_window  to display game information
// This class extends the frame defined in panel_game_info.ui

class PanelGameInfo : public QFrame, public Ui::PanelGameInfo
{
    Q_OBJECT

public:
    explicit PanelGameInfo(QWidget *parent = nullptr);
    ~PanelGameInfo();

    void update_panel(); // updates the information to be displayed

public slots:
    void update_mouse_pos(QPointF mousePos); // updates the lcd's displaying the mouse position in field coordinates
    void on_btn_connectGui_pressed(); // slot that processess clicks to the toggle connect gui button

signals:
    void toggle_connect_gui(); // signal emitted when the toggle connect gui button is pressed

private:
    QElapsedTimer timer; // timer used to count the time since the start of the program
    GraphicsBall* ball_drawer; // graphics item to draw the ball icon



};
#endif // PANEL_GAME_INFO_H
