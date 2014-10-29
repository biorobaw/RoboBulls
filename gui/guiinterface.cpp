#include "guiinterface.h"
#include "mainwindow.h"
#include "selrobotpanel.h"

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
