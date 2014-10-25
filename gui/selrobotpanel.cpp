#include "selrobotpanel.h"
#include "mainwindow.h"
#include "robotpanel.h"
#include "objectposition.h"
#include <QScrollBar>

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

void SelRobotPanel::updateSelectedBotPanel(int id) {
    int v = 0;
    if (id == -1) {
        dash->ui->gView_robot_prime->hide();
        dash->ui->dial_botSpeed->setValue(0);
        dash->ui->lcd_botSpeed->display(0);
        dash->ui->lcd_orient_prime->display("0");
        dash->ui->lcd_coordX_prime->display("0");
        dash->ui->lcd_coordY_prime->display("0");
        dash->ui->dial_botOrient_prime->setValue(0);
        dash->ui->box_primeBot->setTitle(" ");
        dash->ui->text_primeBot->setText(" ");
    } else {
        v = dash->getVelocity(id);
        dash->ui->gView_robot_prime->setScene(dash->robotpanel->botIconSelScenes[id]);
        dash->ui->gView_robot_prime->show();
        dash->ui->dial_botSpeed->setValue(v);
        dash->ui->lcd_botSpeed->display(v);
        dash->ui->lcd_orient_prime->display(dash->objectPos->getBotOrientString(id));
        dash->ui->lcd_coordX_prime->display(dash->objectPos->getBotCoordX(true, id));
        dash->ui->lcd_coordY_prime->display(dash->objectPos->getBotCoordY(true,id));
        dash->ui->dial_botOrient_prime->setValue(dash->objectPos->getBotOrientDouble(true, id));
        dash->ui->box_primeBot->setTitle("Robot " + QString::number(id));

        // Text field
        dash->ui->text_primeBot->setTextColor(Qt::white);
        dash->ui->text_primeBot->setText(dash->botBehavior[id]);
        QScrollBar *sb = dash->ui->text_primeBot->verticalScrollBar();
        sb->setValue(sb->maximum());
    }

}

