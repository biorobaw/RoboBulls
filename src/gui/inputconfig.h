#ifndef INPUTCONFIG_H
#define INPUTCONFIG_H

#include <QWidget>
#include "utilities/my_yaml.h"
#include <QLineEdit>

namespace Ui {
class inputConfig;
}

class inputConfig : public QWidget
{
    Q_OBJECT

public:
    explicit inputConfig(QWidget *parent = nullptr, bool isYellow = false);
    ~inputConfig();
    void grsim_submit(bool isClicked);
    void yisibot_submit(bool isClicked);
    void rpi2019_submit(bool isClicked);
    void normal_game_submit(bool isClicked);
    void strategy_tester_submit(bool isClicked);
    void rpi2019_load_robots(int numRobots);
    void none_submit(bool isClicked);


private slots:


    void on_robot_proxy_activated(const QString &arg1);

    void on_strategy_controller_activated(const QString &arg1);



    void on_pushButton_clicked();

private:
    Ui::inputConfig *ui;
    std::string team;
    YAML::Node team_node;
    std::map<std::string, QWidget*> params;
    std::map<std::string, QWidget*> strategy_params;





};

#endif // INPUTCONFIG_H
