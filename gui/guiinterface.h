#ifndef GUIINTERFACE_H
#define GUIINTERFACE_H

#include <vector>
#include <iostream>

class MainWindow;
class Point;

class GuiInterface
{
public:
    GuiInterface();
    static GuiInterface * getGuiInterface();
    bool isOverride();
    std::vector<bool> isOverriddenBot();
    void show();
    void drawPath(Point A, Point B, int seconds = 1);

private:
    static GuiInterface * gi;
    MainWindow * dash;
};

#endif // GUIINTERFACE_H




