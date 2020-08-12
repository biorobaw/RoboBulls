#ifndef REFCOMM_H
#define REFCOMM_H

#include <QUdpSocket>
#include <QMutex>

using namespace std;

enum Referee_Command : int;
class GameState;

namespace YAML {
    class Node;
}

/*! @brief Communication module to receive information from the RefBox
 * @author Origin. Narges Ghaedi, JamesW
 * @details
 * Retrieves and parses a legacy packet from the
 * [SSL Referee Box](http://robocupssl.cpe.ku.ac.th/referee:start),
 * and fills out the GameModel.<br><br>
 * <b>Referee Box Command Quick Reference:</b>
 */

class SSLGameControllerListener : public QObject
{
    Q_OBJECT
public:
    SSLGameControllerListener(YAML::Node* comm_node);

    static void copyState(GameState*);

//public slots:
//    void run();

private slots:
    void process_package();

private:

    static SSLGameControllerListener* instance;

    QString net_address;
    int    port;

    Referee_Command command;
    Referee_Command command_previous;
    int time_left = 0;
    int goals[2] = {0};

    QMutex mutex;


    QUdpSocket* socket = new QUdpSocket(this);
    void restart_socket();

};

#endif // REFCOMM_H
