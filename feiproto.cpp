#include "feiproto.h"
#include "QHostAddress"
#include "global.h"
#include <QRegExp>
#include <QStringList>

int RSMessage::m_packetNo = 0;
static void dumphex(const char* p,int size)
{
        fprintf(stderr,"%p:",p);
        const unsigned char *begain =(const unsigned char *) p;
        const unsigned char *end =(const unsigned char *) p + size;
        while(begain < end)
        {
                fprintf(stderr,"%02x ",*begain);
                begain++;
        }
        fprintf(stderr,"\n");
}

RSMessage::RSMessage()
{

}
RSMessage::RSMessage(const QByteArray &data, const QHostAddress &address, quint16 port)
        :m_data(data),m_senderAdress(address),m_senderPort(port)
{

}

RSMessage::RSMessage(int action,const QString & content, const QString & additional)
{
        fill(1,packetNo(),action,content,additional);
}
void RSMessage::setVersion(const QString &versionString)
{
        m_verion = versionString;
}
void RSMessage::setVersion(int version)
{
        m_verion = QString::number(version);
}

void RSMessage::fill(int version, int packet, int action, const QString &content, const QString &additional)
{
        QByteArray & data = m_data;
        m_verion = QString::number(version);
        m_packet = QString::number(packet);
        m_action = QString::number(action);
        m_content = content;
        m_additional = additional;

        data.clear();
        data.append(m_verion.toAscii());
        data.append(':');
        data.append(m_packet.toAscii());
        data.append(':');
        data.append(Global::localUserName.toAscii());
        data.append(':');
        data.append(Global::localHostName.toAscii());
        data.append(':');
        data.append(m_action.toAscii());
        data.append(':');
        data.append(content.toAscii());
        data.append('\0');
        data.append(additional.toAscii());
        data.append('\0');
}

RSMessage RSMessage::entry()
{
        return RSMessage(IPMSG_BR_ENTRY,Global::userName,Global::groupName);
}
RSMessage RSMessage::exit()
{
        return RSMessage(IPMSG_BR_EXIT,Global::userName,Global::groupName);

}
RSMessage RSMessage::ansEntry()
{
        return RSMessage(IPMSG_ANSENTRY,Global::userName,Global::groupName);
}
int RSMessage::packetNo()
{
        return m_packetNo++;
}

bool RSMessage::parse()
{
        bool ret = false;
        const QByteArray & data = m_data;
        QString str = QString::fromAscii(data.data(),data.size());
        QRegExp reg("(.*):(.*):(.*):(.*):(.*):");
        reg.setMinimal(true);
        reg.indexIn(str);
        if(reg.matchedLength() > 0)
        {
                m_verion = reg.cap(1);
                m_packet = reg.cap(2);
                m_username = reg.cap(3);
                m_hostname = reg.cap(4);
                m_action = reg.cap(5);

                const QStringList& list = str.mid(reg.matchedLength()).split(QChar('\0'));
                QStringList::ConstIterator  begin = list.begin();
                QStringList::ConstIterator  end = list.end();

                if(begin != end)
                {
                        m_content = *begin;
                }
                begin++;
                if(begin != end)
                {
                        m_additional = *begin;
                }

                ret = true;
        }

        return ret;
}
const QByteArray& RSMessage::data()const
{
        return m_data;
}
int RSMessage::version()const
{
        return m_verion.toInt();
}
const QString & RSMessage::versionString()const
{
        return m_verion;
}

int RSMessage::packet()const
{
        return m_packet.toInt();
}
const QString& RSMessage::packetString()const
{
        return m_packet;
}

const QString& RSMessage::hostname()const
{
        return m_hostname;
}
const QString& RSMessage::username()const
{
        return m_username;
}
int RSMessage::command()const
{
        return m_action.toInt();
}
const QString& RSMessage::commandString()const
{
        return m_action;
}

const QHostAddress & RSMessage::senderAdress()const
{
        return m_senderAdress;
}
quint16  RSMessage::senderPort()const
{
        return m_senderPort;
}
const QString& RSMessage::content()const
{
        return m_content;
}
const QString& RSMessage::additional()const
{
        return m_additional;
}
