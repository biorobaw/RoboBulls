#include "gui_game_state.h"
#include "model/game_state.h"
#include "ssl/ssl_game_controller_listener.h"
#include "model/team/team.h"
#include "model/robot/robot.h"
#include "model/robot/navigation/robot_pilot.h"

#include "ssl_referee.pb.h"


GuiGameState* GuiGameState::gui_game_state = nullptr;

GuiGameState::GuiGameState()
{
    // replace the robot array with the gui robot array
    for(int i=0; i<2; i++)
        for(int j=0; j<MAX_ROBOTS_PER_TEAM; j++){
            delete robots[i][j];
            gui_robots[i][j] = new GuiRobot(nullptr,i,j); // do not delete this, it will be deleted by parent destructor
            robots[i][j] = gui_robots[i][j];
        }
}


GuiGameState* GuiGameState::get(){
    gui_game_state = gui_game_state ? gui_game_state : new GuiGameState();
    return gui_game_state;
}

GuiRobot* GuiGameState::getRobot(int team_id, int robot_id){
    return get()->gui_robots[team_id][robot_id];
}

QString GuiGameState::getRefereeCommandAsStr(){
    switch (getRefereeCommand()) {
        case Referee_Command_STOP                  : return  "STOP                  ";
        case Referee_Command_GOAL_YELLOW           : return  "GOAL_YELLOW           ";
        case Referee_Command_GOAL_BLUE             : return  "GOAL_BLUE             ";
        case Referee_Command_PREPARE_PENALTY_YELLOW: return  "PREPARE_PENALTY_YELLOW";
        case Referee_Command_PREPARE_PENALTY_BLUE  : return  "PREPARE_PENALTY_BLUE  ";
        case Referee_Command_PREPARE_KICKOFF_YELLOW: return  "PREPARE_KICKOFF_YELLOW";
        case Referee_Command_PREPARE_KICKOFF_BLUE  : return  "PREPARE_KICKOFF_BLUE  ";
        case Referee_Command_DIRECT_FREE_YELLOW    : return  "DIRECT_FREE_YELLOW    ";
        case Referee_Command_DIRECT_FREE_BLUE      : return  "DIRECT_FREE_BLUE      ";
        case Referee_Command_INDIRECT_FREE_YELLOW  : return  "INDIRECT_FREE_YELLOW  ";
        case Referee_Command_INDIRECT_FREE_BLUE    : return  "INDIRECT_FREE_BLUE    ";
        case Referee_Command_TIMEOUT_YELLOW        : return  "TIMEOUT_YELLOW        ";
        case Referee_Command_TIMEOUT_BLUE          : return  "TIMEOUT_BLUE          ";
        case Referee_Command_HALT                  : return  "HALT                  ";
        case Referee_Command_NORMAL_START          : return  "NORMAL_START          ";
        case Referee_Command_FORCE_START           : return  "FORCE_START           ";
        case Referee_Command_BALL_PLACEMENT_YELLOW : return  "BALL_PLACEMENT_YELLOW ";
        case Referee_Command_BALL_PLACEMENT_BLUE   : return  "BALL_PLACEMENT_BLUE   ";
    } ;
    return "ERROR";
}


void GuiGameState::update(){
    GameState::update();

    // copy robot info that is not copied int the game state update:
    for(int i=0; i<2; i++){
        for(int j=0; j<MAX_ROBOTS_PER_TEAM; j++){
            gui_robots[i][j]->update();

        }
    }


}
