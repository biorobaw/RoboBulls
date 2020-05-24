#ifndef GUIGAMESTATE_H
#define GUIGAMESTATE_H

#include <QString>

class GuiGameState
{
public:
    GuiGameState();
    static void updateData(); // updates the data

    static QString getRefereeCommand(); // returns the las command received from the referee


private:
    static QString command; // last command received from the referee
};

#endif // GUIGAMESTATE_H
