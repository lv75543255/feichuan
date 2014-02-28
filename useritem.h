#ifndef USERITEM_H
#define USERITEM_H

#include <QTreeWidgetItem>
#include <QHostAddress>
#include <QObject>
class ChatForm;
class RSMessage;
class QUdpSocket;
class QTcpSocket;
class UserItem :public QTreeWidgetItem
{
public:
    explicit UserItem(const RSMessage & msg,QTreeWidgetItem *parent = 0);

    void pushRequest(QTcpSocket * tcp);
    void pushMessage(const RSMessage & udp);
    void sendMessage(const QString & text);

    void showChat();

    void flicker();
    const QHostAddress & hostAddress()const;
    const QString & nickName()const;
    const QString & groupName()const;
    const QString & hostName()const;
    const QString & userName()const;

private:
    QString m_hostName;
    QString m_userName;
    QString m_nickName;
    QString m_groupName;
    QHostAddress m_hostaddress;

    ChatForm *m_chat;
    bool m_flag;

};

#endif // USERITEM_H
