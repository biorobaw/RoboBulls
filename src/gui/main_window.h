#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

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

#include "ui_main_window.h"

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


private:

    void setupKeyShortcuts();
    void process_user_input();

    void connect_slots();

    bool gui_connected = true;

private slots:
    // Threads
    void coreLoop();
    void toggle_connect_gui();



};

#endif // MAIN_WINDOW_H
