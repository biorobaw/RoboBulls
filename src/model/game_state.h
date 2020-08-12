#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QSet>
#include "utilities/point.h"
#include "constants.h"

enum Referee_Command : int;
class Robot;
class Ball;


class GameState
{
public:

    GameState();
    ~GameState();
    void update();


    // ========= GETTER FUNCTIONS ==============
    QSet<Robot*>& getFieldRobots(int team_id);
    QSet<Robot*>& getFieldRobots();

    Robot* getRobot(int team_id, int robot_id);
    Robot* getBlueRobot(int id);
    Robot* getYellowRobot(int id);
    Ball*  getBall();

    Robot* getRobotWithBall();

    Referee_Command getRefereeCommand();
    Referee_Command getRefereePreviousCommand();

    char getGoals(int team_id);
    char getBlueGoals();
    char getYellowGoals();
    int  getRemainingTime();

    bool hasRefereeCommandChanged();
    void clearRefereeCommandChanged();


protected:

    friend class SSLVisionListener;
    friend class SSLGameControllerListener;

    // Field objects
    Robot* robots[2][MAX_ROBOTS_PER_TEAM];
    QSet<Robot*> team_robots_in_field[2];
    QSet<Robot*> all_robots_in_field;
    Ball*  ball;

    // state of objects
    Robot* robot_with_ball = nullptr;

    // Game state
    Referee_Command referee_command;         //The last referee command received
    Referee_Command referee_command_previous; //The previous gamestate
    bool referee_command_changed = false;

    char   goals[2]      ;  //Number of scores yellow goals // TODO: is it correct that it is a char???
    int    remaining_time ;  //Remaining time in seconds


    void setRobotWithBall();
    bool hasBall(Robot* robot);



};

//! @}

#endif // GAMEMODEL_H
