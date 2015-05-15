#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iosfwd>
#include <string>
#include <QtWidgets/QMainWindow>

// Helper classes forward declarations
class GuiField;
class GuiBall;
class GuiBotLabel;
class GuiDrawLine;
class GuiComm;
class GuiScene;
class GuiInterface;

//Dashboard Forward declarations
class RobotPanel;
class FieldPanel;
class SelRobotPanel;
class ObjectPosition;
class GuiRobot;
class SelRobotPanel;
class GamePanel;
class GuiInterface;
class GuiDrawLine;
class QMainWindow;
class QGraphicsItem;
class QGraphicsEllipseItem;
class QGraphicsRectItem;
class QGraphicsScene;
class GetBehavior;

//Main Project forwards
class GameModel;
class Robot;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // moved from public
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Class pointers
    Ui::MainWindow *ui;
    GameModel *gamemodel;
    RobotPanel * robotpanel;
    FieldPanel * fieldpanel;
    SelRobotPanel * selrobotpanel;
    ObjectPosition * objectPos;
    GamePanel *gamepanel;
    GuiDrawLine *guidrawline;
    GetBehavior *getbehavior;

    QString getRemTime();
    // Debug functions
    void drawLine( int originX, int originY, int endX, int endY );
    void guiPrint(std::string output);
    void updateBallInfo();
    // Key Bindings
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void setMyVelocity();
    // team sizes; used all over the place
    int teamSize_blue;
    int teamSize_yellow;
    // manual override thing
    std::vector<bool> overriddenBots;   // keeps track of whether each bot is overridden

    std::string myTeam;

private:
    void setupKeyShortcuts();
    void checkTeamColors();
    void setFocusOnField();

    // my pointer
    static MainWindow *mw;
    GuiComm* guimodel;

    // Keeps track of latest string received by guiPrint()
    QString guiOutput = "...";

    // pointer to MainWindow
    static MainWindow* window;

    // For controlling bots manually
    int myVelocity;

private slots:
    // Threads
    void coreLoop(int value);
    void clockLoop(int tick);
    void handleJoystickInput();
// Widget slots
    void on_btn_connectGui_clicked();
    // bot controls
    void on_btn_botForward_pressed();
    void on_btn_botForward_released();
    void on_btn_botTurnRight_pressed();
    void on_btn_botTurnRight_released();
    void on_btn_botReverse_pressed();
    void on_btn_botReverse_released();
    void on_btn_botTurnLeft_pressed();
    void on_btn_botTurnLeft_released();
    void on_btn_botKick_pressed();
    void on_btn_botKick_released();
    void on_btn_botDrible_pressed();
    void on_btn_botDrible_released();
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
    void on_btn_rotateField_right_clicked();
    void on_btn_rotateField_left_clicked();
    void on_btn_multithread_clicked();
    void on_btn_toggleTeamColor_clicked();
};

#endif // MAINWINDOW_H
