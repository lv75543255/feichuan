#ifndef COMMON_H
#define COMMON_H
#include <QString>
class QUdpSocket;
class QTcpServer;
class QSystemTrayIcon;
namespace Global
{
extern QUdpSocket *udpSocket;
extern QTcpServer *tcpServer;
extern QSystemTrayIcon *sysTray;
extern QString userName;
extern QString groupName;
extern QString localUserName;
extern QString localHostName;
}
#endif // COMMON_H
