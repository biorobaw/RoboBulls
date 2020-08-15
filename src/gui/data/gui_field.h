#ifndef GUI_FIELD_H
#define GUI_FIELD_H

//Proxy between the gui and the field class

class GuiField
{
public:
    GuiField();
    static void updateField();

    static int width();
    static int halfWidth();
    static int length();
    static int halfLength();
    static int getTeamOnSide(int side);

};

#endif // GUI_FIELD_H
