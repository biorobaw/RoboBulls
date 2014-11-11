#include "guiinterface.h"
#include "mainwindow.h"
#include "selrobotpanel.h"
#include "fieldpanel.h"
#include "utilities/point.h"
#include "guidrawline.h"

/* Points of contact with main project:
 *      - nxtrobcomm.cpp: override;         REQUIRED
 *      - move.cpp: override;               REQUIRED
 *      - strategycontroller.cpp: override  NOT REQUIRED
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

bool GuiInterface::isOverride() {
    return dash->guiOverride;
}

std::vector<bool> GuiInterface::isOverriddenBot() {
    return dash->overriddenBots;
}

void GuiInterface::show() {
    dash->show();
}

void GuiInterface::drawPath(Point A, Point B, double seconds) {
    dash->fieldpanel->setupLine(A, B, seconds);
}

void GuiInterface::setHidePaths(bool hide) {
    dash->fieldpanel->hidePaths = hide;
}

void GuiInterface::guiPrintRobot(int robotID, string output) {
    dash->selrobotpanel->guiPrintRobot(robotID, output);
}

void GuiInterface::guiPrintTerminal(string output) {
    dash->guiPrint(output);
}
