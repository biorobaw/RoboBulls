#include "panel_game_info.h"
#include "gui/data/gui_ball.h"
#include "gui/data/gui_ball.h"
#include "gui/graphics/graphics_ball.h"
#include "gui/data/gui_game_state.h"
#include "model/constants.h"

PanelGameInfo::PanelGameInfo(QWidget *parent) :
    QFrame(parent)
{
    setupUi(this);
    timer.start();

    // putting ball icon into GUI
    QGraphicsScene *scene_ballIcon = new QGraphicsScene;
    scene_ballIcon->addItem( ball_drawer = new GraphicsBall(true));
    ball_drawer->setScale(0.65);
    gView_ball->setScene(scene_ballIcon);
    gView_ball->scale(.2, .2);
    gView_ball->scale(1, -1);
    gView_ball->hide();
    gView_ball->show();

    connect(GuiBall::gui_ball,SIGNAL(color_changed()),gView_ball,SLOT(repaint()));

}

PanelGameInfo::~PanelGameInfo()
{
}


void PanelGameInfo::update_mouse_pos(QPointF mousePos){
    lcd_mouse_coordX->display((int)mousePos.rx());
    lcd_mouse_coordY->display((int)mousePos.ry());
}

void PanelGameInfo::update_panel(){

    // update ball information
    lcd_ball_coordX->display((int)GuiBall::getPosition().x);
    lcd_ball_coordY->display((int)GuiBall::getPosition().y);
    label_ball_vel->setText("v= " + QString::number(GuiBall::getSpeed(),'f',2));
    label_ball_acc->setText("a= TODO");

    // Update score and time remaining
    auto game_state = GuiGameState::get();
    label_blue_goals->setText(QString::number(game_state->getGoals(ROBOT_TEAM_BLUE)));
    label_yellow_goals->setText(QString::number(game_state->getGoals(ROBOT_TEAM_YELLOW)));
    label_time_remaining->setText("Time Left: " + QString::number(game_state->getRemainingTime()));


    // TODO: knowing the time since the start of the program doesn't seem to useful
    // it might be better replacing this widget with something else
    auto milli_seconds = timer.elapsed();
    int seconds = ( milli_seconds / ( 1000 )      ) % 60;
    int minutes = ( milli_seconds / ( 1000*60 )   ) % 60;
    int hours   = ( milli_seconds / ( 1000*60*60) );
    if(hours > 99) hours = 99;

    // assembling string and printing to GUI
    QString time =  QString::number(hours).rightJustified(2, '0')
            + ":" + QString::number(minutes).rightJustified(2,'0')
            + ":" + QString::number(seconds).rightJustified(2,'0');
    lcd_clock->display(time);



    //Populating the gameState label with the current refbox command
    label_game_state->setText(game_state->getRefereeCommandAsStr());



}





void PanelGameInfo::on_btn_connectGui_pressed()
{
    static bool gui_connected = true;
    gui_connected = !gui_connected;
    if(gui_connected) btn_connectGui->setText("Disconnect Gui");
    else btn_connectGui->setText("Connect Gui");
    emit toggle_connect_gui();
}
