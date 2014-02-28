#include "rsfileitem.h"
#include "feiproto.h"
#include "chatform.h"
#include "useritem.h"
#include "rsfilemodel.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QTcpSocket>
#include <QRegExp>
#include <QDateTime>
RSFileItem::RSFileItem(RSFileModel *parent)
        :QObject(reinterpret_cast<QObject*>(parent)),m_file(0),m_tcp(0)
{

}

RSFileItem::RSFileItem(RSFileModel *parent, int packetNo, const QString &data)
        :QObject(reinterpret_cast<QObject*>(parent)),m_packetNo(packetNo),m_data(data),m_tcp(0),m_file(0)
{

}
RSFileItem::RSFileItem(RSFileModel *parent, const QString &file)
        :QObject(reinterpret_cast<QObject*>(parent)),m_file(0),m_tcp(0)
{
        QFileInfo fileInfo(file);
        m_filename = fileInfo.fileName();
        m_filesize = fileInfo.size();
        m_fileattr = 0;
        m_modifyTime = fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss");
}
RSFileItem::~RSFileItem()
{
        delete m_file;
}

bool RSFileItem::parse()
{
        QRegExp reg("(.*):(.*):(.*):(.*):(.*):");
        reg.setMinimal(true);
        reg.indexIn(m_data);
        //    qDebug()<<">>>>>>>>>>>>>>>>";
        if(reg.matchedLength() > 0)
        {
                bool ok;
                QDateTime time;
                m_number = reg.cap(1).toInt();
                m_filename = reg.cap(2);
                m_filesize = reg.cap(3).toLongLong(&ok,16);

                time.setTime_t(reg.cap(4).toUInt(&ok,16));
                m_modifyTime = time.toString("yyyy-MM-dd hh:mm:ss");

                m_fileattr = reg.cap(5).toInt();
                m_externData = m_data.mid(reg.matchedLength());
                return true;
        }

        return false;
}
int RSFileItem::packet()const
{
        return m_packetNo;
}
int RSFileItem::number()const
{
        return m_number;
}
const QString & RSFileItem::filename()const
{
        return m_filename;
}

int RSFileItem::filesize()const
{
        return m_filesize;
}

const QString & RSFileItem::modifyTime()const
{
        return m_modifyTime;
}
int RSFileItem::fileattr()const
{

}
const QString & RSFileItem::fullname()const
{
        return m_filename;
}
void RSFileItem::startGetFile()
{
        ChatForm *chat;
        chat = qobject_cast<ChatForm*>(parent()->parent());
        if(chat == 0)
        {
                qDebug("[%s:%d] chat is null",__FUNCTION__,__LINE__);
                return;
        }
        qDebug()<<"open filename="<<fullname();
        m_file = new QFile(fullname());
        if(!m_file->open(QIODevice::WriteOnly))
        {
                QMessageBox::warning(0,tr("open file"),tr("open file failed"));
                return;
        }
        m_tcp = new QTcpSocket(this);

        connect(m_tcp,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));
        connect(m_tcp,SIGNAL(readyRead()),this,SLOT(onReadyData()));
        connect(m_tcp,SIGNAL(connected()),this,SLOT(onConnected()));
        connect(m_tcp,SIGNAL(disconnected()),this,SLOT(onDisconnected()));

        m_tcp->connectToHost(chat->userItem()->hostAddress(),2425);
}
QString RSFileItem::toGetDiscription(long long offset) const
{
        return QString("%1:%2:%3").arg(QString::number(packet(),16),QString::number(number(),16),QString::number(offset,16));
}
void RSFileItem::onReadyData()
{
        for(;;)
        {
            qint64 nbytes = m_tcp->bytesAvailable();
            if(nbytes > 0)
            {
                    m_file->write(m_tcp->read(nbytes));
                    qDebug()<<"filesize:"<<m_file->size();
                    if(m_file->size() == m_filesize)
                    {
                            qDebug()<<"filesize ok to close:";
                            m_tcp->close();
                    }
            }
            else
            {
                    break;
            }
        }

}
void RSFileItem::onConnected()
{
        RSMessage msg(IPMSG_GETFILEDATA,toGetDiscription());
        m_tcp->write(msg.data());
        qDebug()<<"send data";
}
void RSFileItem::onDisconnected()
{
       delete m_file;
       m_file = 0;
       m_tcp->deleteLater();
       qDebug()<<"disconnect";
}
void RSFileItem::onError(QAbstractSocket::SocketError e)
{
        QMessageBox::warning(0,tr("recive file"), m_tcp->errorString());

        delete m_file;
        m_file = 0;
        m_tcp->deleteLater();
}
