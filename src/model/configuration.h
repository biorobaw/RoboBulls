#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QString>
#include <string>
#include <QMap>
#include "utilities/my_yaml.h"



class Configuration : QObject
{
    Q_OBJECT
public:
    static Configuration* get();

    Configuration(std::string file);
    YAML::Node& operator[](QString config_folder);

private:

    QMap<QString,YAML::Node> configs;
    QString config_folder = "config";

    static Configuration* instance;

};

#endif // CONFIGURATION_H
