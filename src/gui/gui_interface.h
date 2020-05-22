#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

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

#endif // GUI_INTERFACE_H




