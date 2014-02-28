#include "global.h"
#include "platform.h"
#include <QHostInfo>

QUdpSocket * Global::udpSocket = 0;
QTcpServer * Global::tcpServer = 0;
QSystemTrayIcon * Global::sysTray = 0;
QString Global::userName;
QString Global::groupName;
QString Global::localUserName = OS::getUserName();
QString Global::localHostName = QHostInfo::localHostName();
