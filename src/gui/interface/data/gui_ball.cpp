#include "gui_ball.h"
#include "model/ball.h"
#include "ssl/ssl_vision_listener.h"
#include "gui_game_state.h"

GuiBall* GuiBall::gui_ball = new GuiBall();

GuiBall::GuiBall(){

}

Point GuiBall::getPosition(){
    return *GuiGameState::get()->getBall();
}

Point GuiBall::getVelocity(){
    return GuiGameState::get()->getBall()->getVelocity();
}

float GuiBall::getSpeed(){
    return GuiGameState::get()->getBall()->getSpeed();
}



