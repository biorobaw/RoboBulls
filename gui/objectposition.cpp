#include "objectposition.h"
#include "fieldpanel.h"
#include "gamepanel.h"
#include "model/gamemodel.h"
#include "mainwindow.h"

//Helper Classes
#include "guiscene.h"

ObjectPosition::ObjectPosition(MainWindow * mw) {
    dash = mw;
}

void ObjectPosition::setupPastBotPoints() {
    for (int i=0; i<dash->teamSize_blue; i++) {
        if (dash->gamemodel->find(i, dash->gamemodel->getMyTeam()) != NULL) {
            pastBotPoints[i].x = 0;
            pastBotPoints[i].y = 0;
            // also, let's fill the oldSpeeds array
            oldSpeeds[i] = 0;
        }
    }
}

void ObjectPosition::getPastBotPoints() {
//    if (dash->gamepanel->tick && dash->gamepanel->seconds%2 == 0) {
//    if (dash->gamepanel->tick) {
        for (int i=0; i<dash->teamSize_blue; i++) {
            if (dash->gamemodel->find(i, dash->gamemodel->getMyTeam()) != NULL) {
                pastBotPoints[i].x = getBotCoordX(true,i);
                pastBotPoints[i].y = getBotCoordY(true,i);
            }
        }
//    }
}

void ObjectPosition::getNewBotPoints() {
    for (int i=0; i<dash->teamSize_blue; i++) {
        if (dash->gamemodel->find(i, dash->gamemodel->getMyTeam()) != NULL) {
            newBotPoints[i].x = getBotCoordX(true,i);
            newBotPoints[i].y = getBotCoordY(true,i);
        }
    }
}

void ObjectPosition::setupBotSpeeds() {
    for (int i=0; i<dash->teamSize_blue; i++) {
        if (dash->gamemodel->find(i, dash->gamemodel->getMyTeam()) != NULL) {
           botSpeeds.push_back(0);
        }
    }
}

void ObjectPosition::getBotSpeeds() {
    for (int i=0; i<dash->teamSize_blue; i++) {
        if (dash->gamemodel->find(i, dash->gamemodel->getMyTeam()) != NULL) {
            float s = 0;
            float c;
            Point currentPos;
            Point pastPos;
            currentPos.x = newBotPoints[i].x;
            currentPos.y = newBotPoints[i].y;
            pastPos.x = pastBotPoints[i].x;
            pastPos.y = pastBotPoints[i].y;

//            c = ( pow((currentPos.x - pastPos.x), 2) + pow((currentPos.y - pastPos.y), 2) );
            c = Measurments::distance(currentPos,pastPos);

            s = sqrt(c);
            botSpeeds[i] = s;
        }
    }

}

void ObjectPosition::getOldSpeeds() {
    for (int i=0; i<dash->teamSize_blue; i++) {
        if (dash->gamemodel->find(i, dash->gamemodel->getMyTeam()) != NULL) {
            oldSpeeds[i] = botSpeeds[i];
        }
    }
}

void ObjectPosition::updateBotSpeedsRecord() {
    botSpeedsRecord.push_front(botSpeeds);
    if (botSpeedsRecord.size() > 10) {
        botSpeedsRecord.pop_back();
    }
}

int ObjectPosition::getVelocity(int id) {
    int velocity = 0;
    int wheels = 0;
    int LF = 0;
    int RF = 0;
    int LB = 0;
    int RB = 0;

    if ( dash->gamemodel->find(id, dash->gamemodel->getMyTeam())->type() == fourWheelOmni ) {
//        selrobotpanel->guiPrintRobot(id,"fourWheelOmni");
//        if (SIMULATED) {
            LF = dash->gamemodel->find(id, dash->gamemodel->getMyTeam())->getLF();
            RF = dash->gamemodel->find(id, dash->gamemodel->getMyTeam())->getRF();
            LB = dash->gamemodel->find(id, dash->gamemodel->getMyTeam())->getLB();
            RB = dash->gamemodel->find(id, dash->gamemodel->getMyTeam())->getRB();
//        } else {
//            LF = nxtrobcomm->gui_left_front;
//            RF = nxtrobcomm->gui_right_front;
//            LB = nxtrobcomm->gui_left_back;
//            RB = nxtrobcomm->gui_right_back;
//        }
//        cout << "4wheel Robot " << id << ": " << LF << ", " << RF << "\n";
            velocity += LF;
            wheels++;
            velocity += RF;
            wheels++;
            velocity += LB;
            wheels++;
            velocity += RB;
            wheels++;
    } else if ( dash->gamemodel->find(id, dash->gamemodel->getMyTeam())->type() == differential ) {
//        selrobotpanel->guiPrintRobot(id,"differential");
//        if (SIMULATED) {
            LF = dash->gamemodel->find(id, dash->gamemodel->getMyTeam())->getL();
            RF = dash->gamemodel->find(id, dash->gamemodel->getMyTeam())->getR();
//        } else {
//            LF = nxtrobcomm->gui_left;
//            RF = nxtrobcomm->gui_right;
//        }
//        cout << "diff Robot " << id << ": " << LF << ", " << RF << "\n";
            velocity += LF;
            wheels++;
            velocity += RF;
            wheels++;
    } else if ( dash->gamemodel->find(id, dash->gamemodel->getMyTeam())->type() == threeWheelOmni ) {
//        selrobotpanel->guiPrintRobot(id,"threeWheelOmni");
//        if (SIMULATED) {
            LF = dash->gamemodel->find(id, dash->gamemodel->getMyTeam())->getLF();
            RF = dash->gamemodel->find(id, dash->gamemodel->getMyTeam())->getRF();
//        } else {
//            LF = nxtrobcomm->gui_left_front;
//            RF = nxtrobcomm->gui_right_front;
//        }
        int b = dash->gamemodel->find(id, dash->gamemodel->getMyTeam())->getB();
//        cout << "3wheel Robot " << id << ": " << LF << ", " << RF << "\n";

            velocity += LF;
            wheels++;
            velocity += RF;
            wheels++;
            velocity += b;
            wheels++;
    }

    if (velocity != 0 && wheels != 0)
        velocity /= wheels;

//    selrobotpanel->guiPrintRobot(id,"Wheels: " + to_string(LF) + " & " + to_string(RF));
    return velocity;

}

//void ObjectPosition::getThreadTicker(int tick) {
//    threadTicker = tick;
//}

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


float ObjectPosition::getBotSpeed(bool myTeam, int id) {
    float s = 0;
    double c;
    Point currentPos;
    Point pastPos;
    if (myTeam) {
        currentPos.x = newBotPoints[id].x;
        currentPos.y = newBotPoints[id].y;
        pastPos.x = pastBotPoints[id].x;
        pastPos.y = pastBotPoints[id].y;
    }

    c = ( pow((currentPos.x - pastPos.x), 2) + pow((currentPos.y - pastPos.y), 2) );

    s = sqrt(c);


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
