#include "gui_game_state.h"
#include "model/game_state.h"

#include "ssl_referee.pb.h"

QString GuiGameState::command = "N/A";

GuiGameState::GuiGameState()
{

}


void GuiGameState::updateData(){
    switch (GameState::getRefereeCommand()) {
        case Referee_Command_STOP                  : command = "STOP                  "; break;
        case Referee_Command_GOAL_YELLOW           : command = "GOAL_YELLOW           "; break;
        case Referee_Command_GOAL_BLUE             : command = "GOAL_BLUE             "; break;
        case Referee_Command_PREPARE_PENALTY_YELLOW: command = "PREPARE_PENALTY_YELLOW"; break;
        case Referee_Command_PREPARE_PENALTY_BLUE  : command = "PREPARE_PENALTY_BLUE  "; break;
        case Referee_Command_PREPARE_KICKOFF_YELLOW: command = "PREPARE_KICKOFF_YELLOW"; break;
        case Referee_Command_PREPARE_KICKOFF_BLUE  : command = "PREPARE_KICKOFF_BLUE  "; break;
        case Referee_Command_DIRECT_FREE_YELLOW    : command = "DIRECT_FREE_YELLOW    "; break;
        case Referee_Command_DIRECT_FREE_BLUE      : command = "DIRECT_FREE_BLUE      "; break;
        case Referee_Command_INDIRECT_FREE_YELLOW  : command = "INDIRECT_FREE_YELLOW  "; break;
        case Referee_Command_INDIRECT_FREE_BLUE    : command = "INDIRECT_FREE_BLUE    "; break;
        case Referee_Command_TIMEOUT_YELLOW        : command = "TIMEOUT_YELLOW        "; break;
        case Referee_Command_TIMEOUT_BLUE          : command = "TIMEOUT_BLUE          "; break;
        case Referee_Command_HALT                  : command = "HALT                  "; break;
        case Referee_Command_NORMAL_START          : command = "NORMAL_START          "; break;
        case Referee_Command_FORCE_START           : command = "FORCE_START           "; break;
        case Referee_Command_BALL_PLACEMENT_YELLOW : command = "BALL_PLACEMENT_YELLOW "; break;
        case Referee_Command_BALL_PLACEMENT_BLUE   : command = "BALL_PLACEMENT_BLUE   "; break;
    } ;

}


QString GuiGameState::getRefereeCommand(){
    return command;
}

