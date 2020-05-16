#ifndef PANEL_OUTPUT_H
#define PANEL_OUTPUT_H

#include <QFrame>
#include "ui_panel_output.h"

namespace Ui {
class PanelOutput;
}

class PanelOutput : public QFrame, public Ui::PanelOutput
{
    Q_OBJECT

public:
    explicit PanelOutput(QWidget *parent = nullptr);
    ~PanelOutput();

};

#endif // PANEL_OUTPUT_H
