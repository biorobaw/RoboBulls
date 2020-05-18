#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iosfwd>
#include <string>
#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "model/constants.h"

// Helper classes forward declarations
class GuiField;
class GuiBotLabel;
class GuiDrawLine;
class GuiInterface;

//Dashboard Forward declarations
class RobotPanel;
class PanelField;
class SelRobotPanel;
class SelRobotPanel;
class GamePanel;
class GuiInterface;
class GuiDrawLine;
class QMainWindow;
class QGraphicsItem;
class QGraphicsEllipseItem;
class QGraphicsRectItem;
class QGraphicsScene;
class RobotTeam;


//Main Project forwards
class GameState;
class Robot;

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT

public:
    // moved from public
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Class pointers
    RobotPanel * robotpanel;

    // Debug functions
    // Key Bindings
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);


    int getSelectedTeamId();
    void setSelectedTeamId(int team_id);
    std::string getSelectedTeamName();
    RobotTeam* getSelectedTeam();

    void control_selected_robot(float vx, float vy, float w);
    void dribble_selected_robot(bool dribble);
    void kick_selected_robot(float kick=5.0);



private:

    int selected_team_id = ROBOT_TEAM_BLUE;

    void setupKeyShortcuts();

    // my pointer
    static MainWindow *mw;

    // pointer to MainWindow
    static MainWindow* window;

    void process_user_input();

    void connect_slots();

private slots:
    // Threads
    void coreLoop();
// Widget slots
    // bot controls
    void on_check_botOverride_clicked(bool checked);
    void on_btn_override_all_released();
    void on_btn_override_none_released();
    // field
    void on_check_fieldGrid_clicked();
    void on_combo_gridScale_currentIndexChanged(int index);
    void on_check_coloredGoals_clicked();
    void on_combo_fieldColor_currentIndexChanged(int index);
    void on_check_showIDs_stateChanged(int arg1);
    void on_combo_botScale_currentIndexChanged(int index);
    void on_btn_toggleTeamColor_clicked();

    void select_robot(int team, int robot);

};

#endif // MAINWINDOW_H
