#ifndef FEIPROTO_H
#define FEIPROTO_H
#include <QString>
#include <QByteArray>
#include <QHostAddress>
#include "ipmsg.h"
class RSMessage
{
public:
    RSMessage();
    RSMessage(int action,const QString & content, const QString & additional = QString());
    RSMessage(const QByteArray & data,const QHostAddress & address = QHostAddress(), quint16 port = 2425);

    void fill(int version,int packet,int action,const QString & content, const QString & additional);

    void setVersion(const QString & versionString);
    void setVersion(int version);

    bool parse();

    int version()const;
    const QString& versionString()const;

    int packet()const;
    const QString& packetString()const;
    const QString& hostname()const;
    const QString& username()const;
    int command()const;
    const QString& commandString()const;
    const QString& content()const;
    const QString& additional()const;

    const QHostAddress & senderAdress()const;
    quint16  senderPort()const;

    const QByteArray& data()const;

    static RSMessage entry();
    static RSMessage exit();
    static RSMessage ansEntry();
private:
    //1:100:shirouzu:jupiter:32:Hello
    //version:packet:hostname:username:cmd:extern
    QString m_verion;
    QString m_packet;
    QString m_hostname;
    QString m_username;
    QString m_action;
    QString m_content;
    QString m_additional;

    QByteArray m_data;
    QHostAddress m_senderAdress;
    quint16 m_senderPort;
private:
    static int packetNo();
    static int m_packetNo;
};

#endif // FEIPROTO_H
