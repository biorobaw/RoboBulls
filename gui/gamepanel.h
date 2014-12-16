#ifndef GAMEPANEL_H
#define GAMEPANEL_H
#include <QObject>

class MainWindow;

class GamePanel : public QObject
{
    Q_OBJECT

public:
    GamePanel(MainWindow * mw);
    MainWindow *dash;
    // game clock
    int seconds = 0;
    int minutes = 0;
    int hours = 0;
    QString secondsZero = "0";
    QString minutesZero = "0";
    QString hoursZero = "0";
    bool tick_tock;
    bool tick;
    bool tock;
    bool lastWasTock = true;
//    void getTickTock(int ticker);

public slots:
    void guiClock(int milliseconds);

};

#endif // GAMEPANEL_H
