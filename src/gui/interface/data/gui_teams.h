#ifndef GUI_TEAMS_H
#define GUI_TEAMS_H

#include <QObject>
#include <QString>

//Proxy between the gui and the ball

class GuiTeams : public QObject
{
    Q_OBJECT
public:

    static QString getRobotType(int team_id); // returns the robot type used by the team
    static QString getController(int team_id); // returns the controller used by the team
    static QString getStrategy(int team_id);   // returns the current strategy of the team

    static bool isControlled(int team_id);
    static void initData();

public slots:
    void updateRobotType(int team, QString type);
    void updateController(int team, QString controller);
    void updateStrategy(int team, QString strategy);

private:
    QString robot_type[2] = {"none"}; // stores the robot types used by each team
    QString controller[2] = {"none"}; // stores the controllers used by each team
    QString strategy[2]   = {"none"}; // stores the current strategy of each team

    GuiTeams();
    static GuiTeams* instance;


};

#endif // GUI_TEAMS_H
