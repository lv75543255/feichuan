#ifndef RSFILESOCKET_H
#define RSFILESOCKET_H
#include <QTimer>
#include <QTcpSocket>
#include <QPersistentModelIndex>
class RSFileModel;
class RSFileSocket : public QObject
{
    Q_OBJECT
public:
        explicit RSFileSocket(const QPersistentModelIndex &index);

private:
        QPersistentModelIndex m_index;
        QTcpSocket tcpSocket;
        QTimer timer;
};

#endif // RSFILESOCKET_H
