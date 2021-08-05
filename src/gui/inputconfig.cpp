#include "inputconfig.h"
#include "ui_inputconfig.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSpinBox>


inputConfig::inputConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inputConfig)
{
    ui->setupUi(this);

}

inputConfig::~inputConfig()
{
    delete ui;
}


void inputConfig::grsim_submit(bool isClicked)
{

}
void inputConfig::yisibot_submit(bool isClicked)
{

}

void inputConfig::rpi2019_submit(bool isClicked)
{

}

void inputConfig::rpi2019_load_robots(int numRobots)
{
    int rcount = ui->robot_proxy_form->rowCount();
    for (int  i = 0; i <  rcount ; i++)
    {
        if (!(i == 0 || i == rcount-1))
            ui->robot_proxy_form->removeRow(1);
    }

    for (int i = 0; i < numRobots; i++)
    {
        QString robotNum = "Robot " + QString::number(i+1);
        QLabel* robot = new QLabel(robotNum, this);
        QLineEdit* addr = new QLineEdit(this);
        QLineEdit* port = new QLineEdit(this);
        QLabel* addr_label= new QLabel("Address", this);
        QLabel* port_label= new QLabel("Port", this);
        //QHBoxLayout* temp = new QHBoxLayout(this);
        ui->robot_proxy_form->addRow(robot);
        ui->robot_proxy_form->addRow(addr_label, addr);
        ui->robot_proxy_form->addRow(port_label, port);
    }
}

void inputConfig::on_robot_proxy_activated(const QString &arg1)
{
    QString proxy = arg1.toLower();
    int rcount = ui->robot_proxy_form->rowCount();
    for (int  i = 0; i <  rcount ; i++)
        ui->robot_proxy_form->removeRow(0);
    if (proxy == "grsim")
    {

        QLineEdit* addr = new QLineEdit(this);
        QLineEdit* port = new QLineEdit(this);
        QLabel* addr_label= new QLabel("Address", this);
        QLabel* port_label= new QLabel("Port", this);
        QPushButton* submit = new QPushButton("Submit", this);
        //QHBoxLayout* temp = new QHBoxLayout(this);

        ui->robot_proxy_form->addRow(addr_label, addr);
        ui->robot_proxy_form->addRow(port_label, port);
        ui->robot_proxy_form->addRow(submit);

        connect(submit, &QPushButton::clicked, this, &inputConfig::grsim_submit );





    }
    else if (proxy == "yisibot")
    {

        QLineEdit* frequency = new QLineEdit(this);
        QLineEdit* usb_port = new QLineEdit(this);
        QLabel* frequency_label= new QLabel("Address", this);
        QLabel* usb_port_label= new QLabel("Port", this);
        QPushButton* submit = new QPushButton("Submit", this);
        //QHBoxLayout* temp = new QHBoxLayout(this);

        ui->robot_proxy_form->addRow(frequency_label, frequency);
        ui->robot_proxy_form->addRow(usb_port_label, usb_port);
        ui->robot_proxy_form->addRow(submit);

        connect(submit, &QPushButton::clicked, this, &inputConfig::yisibot_submit );

    }
    else if (proxy == "rpi_2019")
    {

        QSpinBox* numRobots = new QSpinBox(this);
        QLabel* numRObots_label = new QLabel(this);
        ui->robot_proxy_form->addRow(numRObots_label, numRobots);
        connect(numRobots, QOverload<int>::of(&QSpinBox::valueChanged), this, &inputConfig::rpi2019_load_robots);
        QPushButton* submit = new QPushButton("Submit", this);
        ui->robot_proxy_form->addRow(submit);
        connect(submit, &QPushButton::clicked, this, &inputConfig::rpi2019_submit );

    }
}

