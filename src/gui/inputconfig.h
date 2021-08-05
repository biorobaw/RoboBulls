#ifndef INPUTCONFIG_H
#define INPUTCONFIG_H

#include <QWidget>

namespace Ui {
class inputConfig;
}

class inputConfig : public QWidget
{
    Q_OBJECT

public:
    explicit inputConfig(QWidget *parent = nullptr);
    ~inputConfig();
    void grsim_submit(bool isClicked);
    void yisibot_submit(bool isClicked);
    void rpi2019_submit(bool isClicked);
    void rpi2019_load_robots(int numRobots);
private slots:


    void on_robot_proxy_activated(const QString &arg1);

private:
    Ui::inputConfig *ui;
};

#endif // INPUTCONFIG_H
