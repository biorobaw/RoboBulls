#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iosfwd>
#include <string>
#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "model/constants.h"

// Helper classes forward declarations
class GraphicsField;
class GraphicsLabel;
class GraphicsLine;
class GuiInterface;

//Dashboard Forward declarations
class GuiInterface;
class GraphicsLine;
class QMainWindow;
class QGraphicsItem;
class QGraphicsEllipseItem;
class QGraphicsRectItem;
class QGraphicsScene;
class RobotTeam;
class GuiRobot;


//Main Project forwards
class GameState;
class Robot;

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT

public:
    static MainWindow* dash;

    // moved from public
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    // Debug functions
    // Key Bindings
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);


    int getSelectedTeamId();
    void setSelectedTeamId(int team_id);
    std::string getSelectedTeamName();
    RobotTeam* getSelectedTeam();

    void signal_new_robot_selected(GuiRobot* robot);


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



};

#endif // MAINWINDOW_H
