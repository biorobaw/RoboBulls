#ifndef GUI_FIELD_H
#define GUI_FIELD_H


class GuiField
{
public:
    GuiField();
    static void updateField();

    static int fieldWidth();
    static int fieldHalfWidth();
    static int fieldLength();
    static int fieldHalfLength();
    static int getTeamOnSide(int side);

};

#endif // GUI_FIELD_H
