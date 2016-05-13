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
    void drawLine(const Point& A, const Point& B, const double seconds = 1);
    void setHidePaths(bool hide);
    void guiPrintRobot(int robotID, std::string output);
    void guiPrintTerminal(std::string output);
    void drawPoint(const Point& p);
    void drawRegion(const std::vector<Point>);

    //Returns the robot that is overriden and selected, or -1 if none.
    int  getSelOverBot();

private:
    static GuiInterface * gi;
    MainWindow * dash;
};

#endif // GUIINTERFACE_H




