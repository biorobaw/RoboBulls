#include "guiinterface.h"
#include "mainwindow.h"
#include "selrobotpanel.h"

/* Points of contact with main project:
 *      - nxtrobcomm.cpp: override;         REQUIRED
 *      - move.cpp: override;               REQUIRED
 *      - strategycontroller.cpp: override  NOT REQUIRED
 */

GuiInterface * GuiInterface::gi = NULL;



GuiInterface::GuiInterface() {
    dash = new MainWindow();
}

GuiInterface* GuiInterface::getGuiInterface() {
    if (gi == NULL) {
        gi = new GuiInterface();
    }
    return gi;

}

bool GuiInterface::isOverride() {
    return dash->guiOverride;
}

std::vector<bool> GuiInterface::isOverriddenBot() {
    return dash->overriddenBots;
}

void GuiInterface::show() {
    dash->show();
}
