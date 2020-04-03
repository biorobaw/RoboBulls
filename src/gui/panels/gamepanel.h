#ifndef GAMEPANEL_H
#define GAMEPANEL_H
#include <QObject>
#include <QElapsedTimer>

class MainWindow;

//GamePanel is the time and ball position indicator across the top

class GamePanel : public QObject
{
    Q_OBJECT

public:
    GamePanel(MainWindow * mw);
    MainWindow *dash;
    QElapsedTimer timer;


public slots:
    void guiClock();

};

#endif // GAMEPANEL_H
