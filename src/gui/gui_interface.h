#ifndef GUI_INTERFACE_H
#define GUI_INTERFACE_H

#include <vector>
//#include <iosfwd>
#include <string>

class MainWindow;
class Point;
class Robot;

// This class provides an interface to use the gui
// It is meant to be used by the main program to control the gui

class GuiInterface
{
public:
    GuiInterface();
    static GuiInterface * getGuiInterface();


    // TODO: all these should be signals to avoid multithreading errors

    void show(); // commands the gui to start showing
    void drawPoint(Point p); // adds a point to be drawn by the gui
    void drawLine(Point A, Point B, const double seconds = 0.01); // adds a line to be drawn by the gui
    void drawRegion(const std::vector<Point>); // sets a region to be drawn by the gui
    void setHidePaths(bool hide); // determines whether to show or hide the paths
    void guiPrintRobot(int robotID, std::string output); // prints a string to the associated robot's output (seen in selected robot panel) TODO: reimplement
    void guiPrintTerminal(std::string output); // prints a string to the gui's ouput panel


private:
    static GuiInterface * gi; // singleton instance of the gui interface
    MainWindow * dash = nullptr; // pointer to the main window
};

#endif // GUI_INTERFACE_H




