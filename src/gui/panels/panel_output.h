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
private:
    // Keeps track of latest string received by print_output()
    QString output_string = "...";
public:
    explicit PanelOutput(QWidget *parent = nullptr);
    ~PanelOutput();

    void print_output(std::string o);


};

#endif // PANEL_OUTPUT_H
