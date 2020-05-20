#include "gui_interface.h"
#include "main_window.h"
#include "utilities/point.h"
#include "gui/graphics/graphics_line.h"
#include "data/gui_robot.h"

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
    return GuiRobot::proxies[team][robot_id].overridden;
}


void GuiInterface::show() {
    dash->show();
}

void GuiInterface::drawLine(const Point& A, const Point& B, const double seconds) {
    dash->panel_field->setupLine(A, B, seconds);
}

void GuiInterface::setHidePaths(bool val) {
    dash->panel_field->setHidePaths(val);
}

void GuiInterface::guiPrintRobot(int robotID, std::string output) {
    // TODO: reimplement functionality
    //dash->panel_selected_robot->guiPrintRobot(robotID, output);
}

void GuiInterface::guiPrintTerminal(std::string output) {
    dash->panel_output->print_output(output);
    // dash->guiOutput.insert(0, QString::fromStdString(output));
}

void GuiInterface::drawPoint(const Point& p)
{
    dash->panel_field->setupPoint(p);
}

void GuiInterface::drawRegion(const std::vector<Point> p_vec)
{
    dash->panel_field->setupRegion(p_vec);
}
