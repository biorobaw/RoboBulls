#include "gamepanel.h"
#include "mainwindow.h"

GamePanel::GamePanel(MainWindow * mw) {
    dash = mw;
}

void GamePanel::getTickTock(int ticker) {
    if (ticker%2 == 0) {
        if (lastWasTock) {
            tick = true;
            tock = false;
            lastWasTock = false;
        } else {
            tick = false;
            tock = true;
            lastWasTock = true;
        }
    } else {
        tick = false;
        tock = false;
    }
}

void GamePanel::guiClock(int milliseconds) {
    // seconds
    if (milliseconds%20 == 0) {
        seconds++;
    }
    if (seconds > 59) {
        seconds = 0;
    }
    if (seconds < 10) {
        secondsZero = "0";
    } else {
        secondsZero = "";
    }
    // minutes
    if (seconds == 0 && milliseconds%20 == 0) {
        minutes++;
    }
    if (minutes < 10) {
        minutesZero = "0";
    } else {
        minutesZero = "";
    }
    if (minutes > 59) {
        minutes = 0;
    }
    // hours
    if (minutes == 0 && seconds == 0 && milliseconds%20 == 0) {
        hours++;
    }
    if (hours > 59) {
        hours = 0;
    }
    // for scenario that will never happen
    if (hours > 99) {
        seconds = 59;
        minutes = 59;
        hours = 99;
    }
    // assembling string and printing to GUI
    QString time = hoursZero + QString::number(hours) + ":" + minutesZero + QString::number(minutes) + ":" + secondsZero + QString::number(seconds);
    dash->ui->lcd_clock->display(time);
}


