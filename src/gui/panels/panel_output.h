#ifndef PANEL_OUTPUT_H
#define PANEL_OUTPUT_H

#include <QFrame>
#include "ui_panel_output.h"

// Class defines a frame to log strings in the gui used by main_window
// The strings are defined by the gui_interface
// This class extends the frame defined in panel_output.ui

class PanelOutput : public QFrame, public Ui::PanelOutput
{
    Q_OBJECT
public:
    explicit PanelOutput(QWidget *parent = nullptr);
    ~PanelOutput();

    void print_output(std::string o);

private:
    // Keeps track of latest string received by print_output()
    QString output_string = "...";

};

#endif // PANEL_OUTPUT_H
