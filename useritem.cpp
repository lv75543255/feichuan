#include "useritem.h"

#include "chatform.h"
#include "feiproto.h"
#include "global.h"
#include <QUdpSocket>
#include <QRegExp>
UserItem::UserItem(const RSMessage &msg, QTreeWidgetItem *parent) :
    QTreeWidgetItem(parent, UserType),m_hostaddress(msg.senderAdress()),m_chat(NULL),m_flag(true)
{
    m_hostName = msg.hostname();
    m_userName = msg.username();
    m_nickName = msg.content();
    m_groupName = msg.additional();

    QString text(m_nickName);
    if(!m_groupName.isEmpty())
    {
            text.append("(");
            text.append(m_groupName);
            text.append(")");
    }

    setIcon(0,QIcon(":/image/qq.ico"));
    setText(0,text);
}
void UserItem::pushRequest(QTcpSocket *tcp)
{
    if(m_chat != NULL)
    {
        m_chat->pushRequest(tcp);

    }
    else
    {
        delete tcp;
    }
}

void UserItem::pushMessage(const RSMessage &udp)
{
    if(m_chat == NULL)
    {
        m_chat = new ChatForm(this);
    }
    m_chat->pushMessage(udp);

}
void UserItem::sendMessage(const QString & text)
{
        RSMessage message(IPMSG_SENDMSG,text);//maoruiflag
    Global::udpSocket->writeDatagram(message.data(),m_hostaddress,2425);
}
void UserItem::showChat()
{
    if(m_chat == NULL)
    {
        m_chat = new ChatForm(this);
    }
    m_chat->showMessage();
}

const QString & UserItem::nickName()const
{
    return m_nickName;
}
const QString & UserItem::groupName()const
{
        return m_groupName;
}
const QString & UserItem::hostName()const
{
        return m_hostName;
}
const QString & UserItem::userName()const
{
        return m_userName;
}
const QHostAddress & UserItem::hostAddress()const
{
    return m_hostaddress;
}

void UserItem::flicker()
{
    setText(0,m_flag ? ">>" : m_nickName );
    m_flag = !m_flag;
}
