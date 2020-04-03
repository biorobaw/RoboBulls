#include "gui/mainwindow.h"
#include "ui_mainwindow.h"
#include "gamepanel.h"

GamePanel::GamePanel(MainWindow * mw) {
    dash = mw;
    dash->ui->btn_multithread->hide();
    dash->ui->label_multithreaded->hide();
    timer.start();
}

void GamePanel::guiClock() {
    auto milli_seconds = timer.elapsed();
    int seconds = ( milli_seconds / ( 1000 )      ) % 60;
    int minutes = ( milli_seconds / ( 1000*60 )   ) % 60;
    int hours   = ( milli_seconds / ( 1000*60*60) );
    if(hours > 99) hours = 99;


    // assembling string and printing to GUI
    QString time =  QString::number(hours).rightJustified(2, '0')
            + ":" + QString::number(minutes).rightJustified(2,'0')
            + ":" + QString::number(seconds).rightJustified(2,'0');
    dash->ui->lcd_clock->display(time);
}


