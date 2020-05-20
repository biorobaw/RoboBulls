#include "panel_output.h"
#include <QScrollBar>

PanelOutput::PanelOutput(QWidget *parent) :
    QFrame(parent)
{
    setupUi(this);
    text_output->setTextColor(Qt::white);
}

PanelOutput::~PanelOutput()
{

}


void PanelOutput::print_output(std::string output) {
//    guiOutput.insert(0, QString::fromStdString(output));

    if (output_string.toStdString() == output) {

    } else {
        // recording this string
        output_string = QString::fromStdString(output);
        // converting received string to QString for printing
        QString msg = QString::fromStdString(output);
        text_output->append(msg);
        // Scrolling to bottom of text box
        QScrollBar *sb = text_output->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
}
