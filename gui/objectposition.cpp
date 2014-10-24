#include "objectposition.h"
#include "fieldpanel.h"

ObjectPosition::ObjectPosition(MainWindow * mw) {
    dash = mw;
}

QString ObjectPosition::getBotCoord(int id) {
    QString qPos    = "no connection";
    std::vector<Robot*> team = dash->gamemodel->getMyTeam();

    if (team.at(0) != NULL){
        std::string posRob = dash->gamemodel->find(id, team)->getRobotPosition().toString();
        qPos = QString::fromStdString(posRob);
    }
    return qPos;

}

int ObjectPosition::getBotCoordX(bool myTeam, int id) {
    int x  = 0;
    std::vector<Robot*> team;
    if (myTeam) {
        team = dash->gamemodel->getMyTeam();
    } else {
        team = dash->gamemodel->getOponentTeam();
    }
    x = dash->gamemodel->find(id, team)->getRobotPosition().x;
    return x;
}

int ObjectPosition::getBotCoordY(bool myTeam, int id) {
    int y  = 0;
    std::vector<Robot*> team;
    if (myTeam) {
        team = dash->gamemodel->getMyTeam();
    } else {
        team = dash->gamemodel->getOponentTeam();
    }
    y = dash->gamemodel->find(id, team)->getRobotPosition().y;
    return y;
}

QString ObjectPosition::getBotOrientString(int id) {
    QString qOrient = "no connection";
    std::vector<Robot*> team = dash->gamemodel->getMyTeam();
    std::string sOrient;
    double  dRads = 0;
    int     iRads = 0;
    dRads = dash->gamemodel->find(id, team)->getOrientation(); // angle in radians
    dRads *= (180/M_PI);
    iRads = dRads;
    sOrient = std::to_string(iRads);
    qOrient = QString::fromStdString(sOrient);
    return qOrient;

}

double ObjectPosition::getBotOrientDouble(bool myTeam, int id) {
    double o  = 0;
    std::vector<Robot*> team;
    if (myTeam) {
        team = dash->gamemodel->getMyTeam();
    } else {
        team = dash->gamemodel->getOponentTeam();
    }
    o = dash->gamemodel->find(id, team)->getOrientation();
    o *= (180/M_PI);
    return o;
}


int ObjectPosition::getBotSpeed(std::vector<QLabel*> c, int id) {
    int s = 0;
    return s;
}

QString ObjectPosition::getBallCoord() {
    QString b;  // return value
    std::string posBallXY = dash->gamemodel->getBallPoint().toString();
    b = QString::fromStdString(posBallXY);

    return b;
}

int ObjectPosition::getBallCoordX() {
    int b;
    b = dash->gamemodel->getBallPoint().x;

    return b;
}

int ObjectPosition::getBallCoordY() {
    int b;
    b = dash->gamemodel->getBallPoint().y;
    return b;
}

int ObjectPosition::getMouseCoordX() {
    int x = dash->fieldpanel->scene->mousePoint.x()-100;
    return x;
}

int ObjectPosition::getMouseCoordY() {
    int y = dash->fieldpanel->scene->mousePoint.y()-100;
    return y;
}
