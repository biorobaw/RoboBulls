#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QObject>
#include <QSet>
#include "utilities/point.h"
#include "constants.h"
#include "robot/navigation/path_planning/move_collisions.h"

enum Referee_Command : int;
class Robot;
class Ball;


class GameState : public QObject, public Collisions
{
    Q_OBJECT
public:

    GameState(QObject* parent=nullptr);
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


    /*! setFlipXCoordinates is used to simplify team control logic assuming
        teams are in the negative side of the field
        if theyr are not, coordinates are flipped for the logic and then
        fixed again before sending the speeds to the robots */
    void setFlipXCoorinates(bool flip_x);

protected:

    friend class SSLVisionListener;
    friend class SSLGameControllerListener;

    // Field objects
    Robot* robots[2][MAX_ROBOTS_PER_TEAM];
    QSet<Robot*> team_robots_in_field[2];
    QSet<Robot*> all_robots_in_field;
    Ball*  ball;

    // state of objects
    Robot* robot_with_ball     = nullptr;
    double last_time_with_ball = -10000;

    // Game state
    Referee_Command referee_command;         //The last referee command received
    Referee_Command referee_command_previous; //The previous gamestate
    bool referee_command_changed = false;

    char   goals[2]       = {0};  // Number of scores yellow goals // TODO: is it correct that it is a char???
    int    remaining_time =  0;   // Remaining time in seconds
    double time_stamp     =  0;   // simulted time stamp of last update


    void setRobotWithBall();
    bool hasBall(Robot* robot);



};

//! @}

#endif // GAMEMODEL_H
