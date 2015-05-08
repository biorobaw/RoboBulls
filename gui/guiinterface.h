#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <vector>
#include <iosfwd>
#include <string>

class MainWindow;
class Point;
class SelRobotPanel;
class FieldPanel;

class GuiInterface
{
public:
    GuiInterface();
    static GuiInterface * getGuiInterface();
    const std::vector<bool>& isOverriddenBot();
    void show();
    void drawPath(Point A, Point B, double seconds = 1);
    void setHidePaths(bool hide);
    void guiPrintRobot(int robotID, std::string output);
    void guiPrintTerminal(std::string output);

    //Returns the robot that is overriden and selected, or -1 if none.
    int  getSelOverBot();
    void setSelOverBot(int);

private:
    int selOverBot;
    static GuiInterface * gi;
    MainWindow * dash;
};

#endif // GUIINTERFACE_H




