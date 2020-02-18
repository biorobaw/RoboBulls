#include "guiinterface.h"
#include "mainwindow.h"
#include "selrobotpanel.h"
#include "fieldpanel.h"
#include "src/utilities/point.h"
#include "guidrawline.h"

/* Points of contact with main project:
 *      - gotopose.cpp
 *      - strategycontroller.cpp:
 * Overriding override:
 *      - HALT command from refbox
 */

GuiInterface * GuiInterface::gi = NULL;

GuiInterface::GuiInterface() {
    dash = new MainWindow();
}

GuiInterface* GuiInterface::getGuiInterface() {
    if (gi == NULL) {
        gi = new GuiInterface();
    }
    return gi;
}

bool GuiInterface::isOverriddenBot(int team, int robot_id) {
    return dash->overriddenBots[team][robot_id];
}

//Returns the robot that is overridden and selected, or -1 if none.
int  GuiInterface::getSelOverBot()
{
    return dash->fieldpanel->selectedBot;
}

void GuiInterface::show() {
    dash->show();
}

void GuiInterface::drawLine(const Point& A, const Point& B, const double seconds) {
    dash->fieldpanel->setupLine(A, B, seconds);
}

void GuiInterface::setHidePaths(bool hide) {
    dash->fieldpanel->hidePaths = hide;
}

void GuiInterface::guiPrintRobot(int robotID, std::string output) {
    dash->selrobotpanel->guiPrintRobot(robotID, output);
}

void GuiInterface::guiPrintTerminal(std::string output) {
    dash->guiPrint(output);
}

void GuiInterface::drawPoint(const Point& p)
{
    dash->fieldpanel->setupPoint(p);
}

void GuiInterface::drawRegion(const std::vector<Point> p_vec)
{
    dash->fieldpanel->setupRegion(p_vec);
}
