#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_main_window.h"


// This class defines the gui's main window, it's composed of the panels under the folder panels
// The class extends the frame defined in main_window.ui

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
    void keyPressEvent(QKeyEvent *event) override; // function to process key press events
    void keyReleaseEvent(QKeyEvent *event) override; // function to process key release events


private:

    void setupKeyShortcuts();  // set's up shortcut keys

    void connect_slots(); // connects signals with slots

    bool gui_connected = true; // defines whether the gui is enabled or disabled

private slots:
    // Threads
    void coreLoop(); // function called by a timer to update the gui
    void toggle_connect_gui(); // toggles whether the gui is enabled or disabled



};

#endif // MAIN_WINDOW_H
