#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <vector>
#include <iosfwd>
#include <string>

class MainWindow;
class Point;
class SelRobotPanel;

class GuiInterface
{
public:
    GuiInterface();
    static GuiInterface * getGuiInterface();
    bool isOverriddenBot(int team, int robot_id);
    void show();
    void drawLine(const Point& A, const Point& B, const double seconds = 0.01);
    void setHidePaths(bool hide);
    void guiPrintRobot(int robotID, std::string output);
    void guiPrintTerminal(std::string output);
    void drawPoint(const Point& p);
    void drawRegion(const std::vector<Point>);


private:
    static GuiInterface * gi;
    MainWindow * dash;
};

#endif // GUIINTERFACE_H




