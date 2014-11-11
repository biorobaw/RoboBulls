#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <vector>
#include <iostream>
#include <string.h>

class MainWindow;
class Point;
class SelRobotPanel;
class FieldPanel;

class GuiInterface
{
public:
    GuiInterface();
    static GuiInterface * getGuiInterface();
    bool isOverride();
    std::vector<bool> isOverriddenBot();
    void show();
    void drawPath(Point A, Point B, double seconds = 1);
    void setHidePaths(bool hide);
    void guiPrintRobot(int robotID, std::string output);
    void guiPrintTerminal(std::string output);

private:
    static GuiInterface * gi;
    MainWindow * dash;
};

#endif // GUIINTERFACE_H




