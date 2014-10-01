#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <QFrame>
#include "model/gamemodel.h"
#include "model/robot.h"
#include <QLabel>
#include "guicomm.h"

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"


//class GuiComm;
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    //Q_INVOKABLE static void printLabel(); DESTROY?
    QString getBotCoord(int id); // Returns the specified robot's x/y position as a QString
    QString getBotOrient(int id);
    double  getBotOrientDouble(int id);
    double getBotCoordX(int id);
    double getBotCoordY(int id);
    QImage  getStatusImg(int id);
    bool    hasChangedQString(QLabel* l, QString s);
    bool    hasChangedQImage(QLabel* l, QImage s);
    int     getBotSpeed(std::vector<QLabel*> c, int id);
    QString getBallCoord();
    double  getBallCoordX();
    double  getBallCoordY();
    QString getRemTime();
    void printBots();
    void printBall();
    void gameModelUpdated();



protected:
    GameModel *gamemodel;
    Robot *robot;
    GuiComm *guicomm;


signals:
    void labelPrinted();
    void updateLabel();
    void printLabel();


private:
    Ui::MainWindow *ui;
    QList<GuiComm*> threads;

private slots:
    void myclicked();
    void on_pushButton_clicked();
    void launch(int value);
};

#endif // MAINWINDOW_H
