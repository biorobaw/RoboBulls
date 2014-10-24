#include "selrobotpanel.h"
#include "mainwindow.h"

SelRobotPanel::SelRobotPanel(MainWindow * mw) {
    dash = mw;
}


void SelRobotPanel::setGuiOverride() {
    // Required for Override to work with Vision
    if (dash->ui->check_botOverride->isChecked()) {
        dash->guiOverride = true;
    } else {
        dash->guiOverride = false;
    }

}

