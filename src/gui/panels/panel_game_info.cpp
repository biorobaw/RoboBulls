#include "panel_game_info.h"
#include "gui/data/gui_ball.h"
#include "model/game_state.h"
#include "gui/data/gui_ball.h"
#include "gui/graphics/graphics_ball.h"

PanelGameInfo::PanelGameInfo(QWidget *parent) :
    QFrame(parent)
{
    setupUi(this);
    timer.start();

    // putting ball icon into GUI
    QGraphicsScene *scene_ballIcon = new QGraphicsScene;
    scene_ballIcon->addItem( ball_drawer = new GraphicsBall(true));
    gView_ball->setScene(scene_ballIcon);
    gView_ball->scale(.2, .2);
    gView_ball->scale(1, -1);
    gView_ball->hide();
    ball_drawer->setScale(0.65);

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
    label_ball_acc->setText("a= Not implemented");

    // Displaying ball icon
    // TODO:
    // the only reason to redisplay is that the
    // icon may change size / color, this is pointless
    gView_ball->update();
    if (gView_ball->isHidden()) {
        gView_ball->show();
    }

    // Update score and time remaining
    label_blue_goals->setText(QString::number(GameState::getBlueGoals()));
    label_yellow_goals->setText(QString::number(GameState::getYellowGoals()));
    label_time_remaining->setText("Time Left: " + QString::number(GameState::getRemainingTime()));


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
    label_game_state->setText(QString::fromStdString(getGameState()));

    //Populating the ballVel label with current ball velocity
    //Fix so that velocity reading are rounded to 2nd or 3rd decimal



}

const std::string PanelGameInfo::getGameState()
{

    switch(GameState::getRefereeCommand())
    {
    case 'S':    //stop game
        return "Stop Game";
        break;
    case 'G':    //Blue Goal
        return "Blue Goal";
        break;
    case 'g':    //Yellow Goal
        return "Yellow Goal";
        break;
    case 'p':   //Yellow Penalty Kick
        return "Yellow Penalty Kick";
        break;
    case 'P':   //Blue Penalty Kick
        return "Blue Penalty Kick";
        break;
    case 'k':   //Yellow Kickoff
        return "Yellow Kickoff";
        break;
    case 'K':   //Blue Kickoff
        return "Blue Kickoff";
        break;
    case 'f':   //Yellow Free Kick
        return "Yellow Free Kick";
        break;
    case 'F':   //Blue Free Kick
        return "Blue Free Kick";
        break;
    case 'i':   //Yellow Indirect Kick
        return "Yellow Indirect Kick";
        break;
    case 'I':   //Blue Indirect kick
        return "Blue Indirect Kick";
        break;
    case 'T':
    case 't':
    case 'H':    //Halt
        return "Halt";
        break;
    case ' ':    //Normal game play
        return "Normal game play";
        break;
    case 's':    //Force Start
        return "Force Start";
        break;
    default:    //Anything Else
        return "Defaul GameState";
        break;
    };
}




void PanelGameInfo::on_btn_connectGui_pressed()
{
    static bool gui_connected = true;
    gui_connected = !gui_connected;
    if(gui_connected) btn_connectGui->setText("Disconnect Gui");
    else btn_connectGui->setText("Connect Gui");
    emit toggle_connect_gui();
}
