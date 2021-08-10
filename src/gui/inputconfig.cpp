#include "inputconfig.h"
#include "ui_inputconfig.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSpinBox>
#include "utilities/my_yaml.h"
#include <fstream>


inputConfig::inputConfig(QWidget *parent, bool isYellow) :
    QWidget(parent),
    ui(new Ui::inputConfig)
{
    ui->setupUi(this);
    team = isYellow ? "TEAM_YELLOW" : "TEAM_BLUE";
    team_node = YAML::LoadFile(std::string("./config") + "/team.yaml");
    ui->team_label->setText(QString::fromStdString(team));
}

inputConfig::~inputConfig()
{
    delete ui;
}


void inputConfig::grsim_submit(bool isClicked)
{
    team_node[team]["ROBOT_PROXY"]["TYPE"] = "grsim";
    team_node[team]["ROBOT_PROXY"]["ADDR"] = ((QLineEdit*)params["grsim_addr"])->text().toStdString();
    team_node[team]["ROBOT_PROXY"]["PORT"] = ((QLineEdit*)params["grsim_port"])->text().toStdString();
    //qInfo() << team_node[team]["ROBOT_PROXY"]["ADDR"];
    std::ofstream fout("config/team.yaml");
    if (fout)
        fout << team_node;
    else
        qInfo() << "File not found";
    ui->confirm_robot_proxy->setText("Robot Proxy Updated: GrSim");
}
void inputConfig::yisibot_submit(bool isClicked)
{
    team_node[team]["ROBOT_PROXY"]["TYPE"] = "yisibot";
    team_node[team]["ROBOT_PROXY"]["FREQUENCY"] = ((QLineEdit*)params["yisibot_frequency"])->text().toStdString();
    team_node[team]["ROBOT_PROXY"]["USB_PORT"] = ((QLineEdit*)params["yisibot_usb_port"])->text().toStdString();
    //qInfo() << team_node[team]["ROBOT_PROXY"]["ADDR"];
    std::ofstream fout("config/team.yaml");
    if (fout)
        fout << team_node;
    else
        qInfo() << "File not found";
    ui->confirm_robot_proxy->setText("Robot Proxy Updated: Yisibot");
}

void inputConfig::rpi2019_submit(bool isClicked)
{
    team_node[team]["ROBOT_PROXY"]["TYPE"] = "rpi_2019";
    for (int i = 0; i < params.size()/2 ; i ++ ) {
        team_node[team]["ROBOT_PROXY"]["ROBOT"][i+1]["ADDR"] = ((QLineEdit*)params["robot" + std::to_string(i+1) + "_addr"])->text().toStdString();
        team_node[team]["ROBOT_PROXY"]["ROBOT"][i+1]["PORT"] = ((QLineEdit*)params["robot" + std::to_string(i+1) + "_port"])->text().toStdString();
    }
    std::ofstream fout("config/team.yaml");
    if (fout)
        fout << team_node;
    else
        qInfo() << "File not found";
    ui->confirm_robot_proxy->setText("Robot Proxy Updated: RPI_2019");
}
void inputConfig::normal_game_submit(bool isClicked)
{
    team_node[team]["STRATEGY_CONTROLLER"]["ID"] = "NORMAL_GAME";


    if (((QSpinBox*)strategy_params["goalie"])->value() != 99) team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["GOALIE"] = ((QSpinBox*)strategy_params["goalie"])->value();
        else team_node[team]["STRATEGY_CONTROLLER"]["ROLES"].remove("GOALIE");
    if (((QSpinBox*)strategy_params["attack1"])->value() != 99)team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["ATTACK1"] = ((QSpinBox*)strategy_params["attack1"])->value();
        else team_node[team]["STRATEGY_CONTROLLER"]["ROLES"].remove("ATTACK1");
    if (((QSpinBox*)strategy_params["attack2"])->value() != 99)team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["ATTACK2"] = ((QSpinBox*)strategy_params["attack2"])->value();
        else team_node[team]["STRATEGY_CONTROLLER"]["ROLES"].remove("ATTACK2");
    if (((QSpinBox*)strategy_params["attack3"])->value() != 99)team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["ATTACK3"] = ((QSpinBox*)strategy_params["attack3"])->value();
        else team_node[team]["STRATEGY_CONTROLLER"]["ROLES"].remove("ATTACK3");
    if (((QSpinBox*)strategy_params["attack4"])->value() != 99)team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["ATTACK4"] = ((QSpinBox*)strategy_params["attack4"])->value();
        else team_node[team]["STRATEGY_CONTROLLER"]["ROLES"].remove("ATTACK4");
    if (((QSpinBox*)strategy_params["defend1"])->value() != 99)team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["DEFEND1"] = ((QSpinBox*)strategy_params["defend1"])->value();
        else team_node[team]["STRATEGY_CONTROLLER"]["ROLES"].remove("DEFEND1");
    if (((QSpinBox*)strategy_params["defend2"])->value() != 99)team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["DEFEND2"] = ((QSpinBox*)strategy_params["defend2"])->value();
        else team_node[team]["STRATEGY_CONTROLLER"]["ROLES"].remove("DEFEND2");
    if (((QSpinBox*)strategy_params["defend3"])->value() != 99)team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["DEFEND3"] = ((QSpinBox*)strategy_params["defend3"])->value();
        else team_node[team]["STRATEGY_CONTROLLER"]["ROLES"].remove("DEFEND3");
    std::ofstream fout("config/team.yaml");
    if (fout)
        fout << team_node;
    else
        qInfo() << "File not found";
    ui->confirm_startegy->setText("Strategy Updated: Normal Game");

}
void inputConfig::strategy_tester_submit(bool isClicked)
{
    team_node[team]["STRATEGY_CONTROLLER"]["ID"] = "STRATEGY_TESTER";
    std::ofstream fout("config/team.yaml");
    if (fout)
        fout << team_node;
    else
        qInfo() << "File not found";
    ui->confirm_startegy->setText("Strategy Updated: Strategy Tester");
}

void inputConfig::rpi2019_load_robots(int numRobots)
{
    params.clear();
    int rcount = ui->robot_proxy_form->rowCount();
    for (int  i = 0; i <  rcount ; i++)
    {
        if (!(i == 0 ))
            ui->robot_proxy_form->removeRow(1);
    }

    for (int i = 0; i < numRobots; i++)
    {
        std::string def_addr = team_node[team]["ROBOT_PROXY"]["ROBOT"][i+1]["ADDR"] ? team_node[team]["ROBOT_PROXY"]["ROBOT"][i+1]["ADDR"].as<std::string>() : "";
        std::string def_port = team_node[team]["ROBOT_PROXY"]["ROBOT"][i+1]["PORT"] ? team_node[team]["ROBOT_PROXY"]["ROBOT"][i+1]["PORT"].as<std::string>() : "";
        QString robotNum = "Robot " + QString::number(i+1);
        QLabel* robot = new QLabel(robotNum, this);
        QLineEdit* addr = new QLineEdit(this);
        QLineEdit* port = new QLineEdit(this);
        QLabel* addr_label= new QLabel("Address", this);
        QLabel* port_label= new QLabel("Port", this);
        //QHBoxLayout* temp = new QHBoxLayout(this);
        addr->setText(QString::fromStdString(def_addr));
        port->setText(QString::fromStdString(def_port));
        params.insert(std::make_pair("robot" + std::to_string(i+1) + "_addr", addr));
        params.insert(std::make_pair("robot" + std::to_string(i+1) + "_port", port));
        ui->robot_proxy_form->addRow(robot);
        ui->robot_proxy_form->addRow(addr_label, addr);
        ui->robot_proxy_form->addRow(port_label, port);

    }
    QPushButton* submit = new QPushButton("Submit", this);
    ui->robot_proxy_form->addRow(submit);
    connect(submit, &QPushButton::clicked, this, &inputConfig::rpi2019_submit );
}

void inputConfig::on_robot_proxy_activated(const QString &arg1)
{
    QString proxy = arg1.toLower();
    int rcount = ui->robot_proxy_form->rowCount();
    for (int  i = 0; i <  rcount ; i++)
        ui->robot_proxy_form->removeRow(0);
    params.clear();
    if (proxy == "grsim")
    {
        std::string def_addr = team_node[team]["ROBOT_PROXY"]["ADDR"] ? team_node[team]["ROBOT_PROXY"]["ADDR"].as<std::string>() : "";
        std::string def_port = team_node[team]["ROBOT_PROXY"]["PORT"] ? team_node[team]["ROBOT_PROXY"]["PORT"].as<std::string>() : "";
        QLineEdit* addr = new QLineEdit(this);
        QLineEdit* port = new QLineEdit(this);
        QLabel* addr_label= new QLabel("Address", this);
        QLabel* port_label= new QLabel("Port", this);
        QPushButton* submit = new QPushButton("Submit", this);
        addr->setText(QString::fromStdString(def_addr));
        port->setText(QString::fromStdString(def_port));

        ui->robot_proxy_form->addRow(addr_label, addr);
        ui->robot_proxy_form->addRow(port_label, port);
        ui->robot_proxy_form->addRow(submit);
        params.insert(std::make_pair("grsim_addr", addr));
        params.insert(std::make_pair("grsim_port", port));

        connect(submit, &QPushButton::clicked, this, &inputConfig::grsim_submit );





    }
    else if (proxy == "yisibot")
    {
        params.clear();
        std::string def_frequency = team_node[team]["ROBOT_PROXY"]["FREQUENCY"] ? team_node[team]["ROBOT_PROXY"]["FREQUENCY"].as<std::string>() : "";
        std::string def_usb_port = team_node[team]["ROBOT_PROXY"]["USB_PORT"] ? team_node[team]["ROBOT_PROXY"]["USB_PORT"].as<std::string>() : "";
        QLineEdit* frequency = new QLineEdit(this);
        QLineEdit* usb_port = new QLineEdit(this);
        QLabel* frequency_label= new QLabel("Address", this);
        QLabel* usb_port_label= new QLabel("Port", this);
        QPushButton* submit = new QPushButton("Submit", this);
        //QHBoxLayout* temp = new QHBoxLayout(this);
        frequency->setText(QString::fromStdString(def_frequency));
        usb_port->setText(QString::fromStdString(def_usb_port));

        ui->robot_proxy_form->addRow(frequency_label, frequency);
        ui->robot_proxy_form->addRow(usb_port_label, usb_port);
        ui->robot_proxy_form->addRow(submit);

        params.insert(std::make_pair("yisibot_frequency", frequency));
        params.insert(std::make_pair("yisibot_usb_port", usb_port));

        connect(submit, &QPushButton::clicked, this, &inputConfig::yisibot_submit );

    }
    else if (proxy == "rpi_2019")
    {

        QSpinBox* numRobots = new QSpinBox(this);
        QLabel* numRObots_label = new QLabel("Number of Robots", this);
        ui->robot_proxy_form->addRow(numRObots_label, numRobots);
        connect(numRobots, QOverload<int>::of(&QSpinBox::valueChanged), this, &inputConfig::rpi2019_load_robots);


    }
}


void inputConfig::on_strategy_controller_activated(const QString &arg1)
{
    QString controller = arg1.toLower();
    int rcount = ui->strategy_controller_form->rowCount();
    for (int  i = 0; i <  rcount ; i++)
        ui->strategy_controller_form->removeRow(0);
    if (controller == "normal game")
    {




        QSpinBox* goalie = new QSpinBox(this);
        QSpinBox* attack1 = new QSpinBox(this);
        QSpinBox* attack2 = new QSpinBox(this);
        QSpinBox* attack3 = new QSpinBox(this);
        QSpinBox* attack4 = new QSpinBox(this);
        QSpinBox* defend1 = new QSpinBox(this);
        QSpinBox* defend2 = new QSpinBox(this);
        QSpinBox* defend3 = new QSpinBox(this);
        QLabel* goalie_label= new QLabel("Goalie", this);
        QLabel* attack1_label= new QLabel("Attack1", this);
        QLabel* attack2_label= new QLabel("Attack2", this);
        QLabel* attack3_label= new QLabel("Attack3", this);
        QLabel* attack4_label= new QLabel("Attack4", this);
        QLabel* defend1_label= new QLabel("Defend1", this);
        QLabel* defend2_label= new QLabel("Defend2", this);
        QLabel* defend3_label= new QLabel("Defend3", this);
        QPushButton* submit = new QPushButton("Submit", this);
        //QHBoxLayout* temp = new QHBoxLayout(this);

        // The ternery operator set the deafult value to zero if the role doesn't exist in the yaml file
        int def_goalie =  team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["GOALIE"] ? team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["GOALIE"].as<int>() : 99 ;
        int def_attack1 = team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["ATTACK1"] ? team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["ATTACK1"].as<int>() : 99 ;
        int def_attack2 = team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["ATTACK2"] ? team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["ATTACK2"].as<int>() : 99 ;
        int def_attack3 = team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["ATTACK3"] ? team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["ATTACK3"].as<int>() : 99 ;
        int def_attack4 = team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["ATTACK4"] ? team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["ATTACK4"].as<int>() : 99 ;
        int def_defend1 = team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["DEFEND1"] ? team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["DEFEND1"].as<int>() : 99 ;
        int def_defend2 = team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["DEFEND2"] ? team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["DEFEND2"].as<int>() : 99 ;
        int def_defend3 = team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["DEFEND3"] ? team_node[team]["STRATEGY_CONTROLLER"]["ROLES"]["DEFEND3"].as<int>() : 99 ;
        goalie->setValue(def_goalie);
        attack1->setValue(def_attack1);
        attack2->setValue(def_attack2);
        attack3->setValue(def_attack3);
        attack4->setValue(def_attack4);
        defend1->setValue(def_defend1);
        defend2->setValue(def_defend2);
        defend3->setValue(def_defend3);


        strategy_params.insert(std::make_pair("goalie", goalie));
        strategy_params.insert(std::make_pair("attack1", attack1));
        strategy_params.insert(std::make_pair("attack2", attack2));
        strategy_params.insert(std::make_pair("attack3", attack3));
        strategy_params.insert(std::make_pair("attack4", attack4));
        strategy_params.insert(std::make_pair("defend1", defend1));
        strategy_params.insert(std::make_pair("defend2", defend2));
        strategy_params.insert(std::make_pair("defend3", defend3));

        ui->strategy_controller_form->addRow(goalie_label, goalie);
        ui->strategy_controller_form->addRow(attack1_label, attack1);
        ui->strategy_controller_form->addRow(attack2_label, attack2);
        ui->strategy_controller_form->addRow(attack3_label, attack3);
        ui->strategy_controller_form->addRow(attack4_label, attack4);
        ui->strategy_controller_form->addRow(defend1_label, defend1);
        ui->strategy_controller_form->addRow(defend2_label, defend2);
        ui->strategy_controller_form->addRow(defend3_label, defend3);
        ui->strategy_controller_form->addRow(submit);

        connect(submit, &QPushButton::clicked, this, &inputConfig::normal_game_submit );





    }
    else if (controller == "strategy tester")
    {


        QPushButton* submit = new QPushButton("Use Strategy Tester", this);

        ui->strategy_controller_form->addRow(submit);

        connect(submit, &QPushButton::clicked, this, &inputConfig::strategy_tester_submit );

    }

}





void inputConfig::on_pushButton_clicked()
{
    this->setAttribute(Qt::WA_QuitOnClose, true);
    this->close();


}

