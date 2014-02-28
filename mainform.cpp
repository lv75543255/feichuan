#include "mainform.h"
#include "ui_mainform.h"
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer>
#include <QTextCodec>
#include <QSettings>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include "useritem.h"
#include "feiproto.h"
#include "global.h"
#include "logindialog.h"
//#define TEST_SELF //(开启自我聊天，可能用于测试)
MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
      ,m_setting(0)
{
        Ui::MainForm * ui = this->ui;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb18030"));
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

    ui->setupUi(this);
    setWindowTitle("Qiuge");

    ui->toolButton->setIcon(QIcon(":/image/qq.ico"));
    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(onTreeItemDoubleClicked(QTreeWidgetItem*,int)));
    ui->treeWidget->autoExpandDelay();
    itemFriends = new QTreeWidgetItem(ui->treeWidget);
    itemFriends->setText(0,tr("My Friends"));
    itemFriends->setExpanded(true);

    Global::sysTray = new QSystemTrayIcon(this);
    Global::sysTray->setIcon(QIcon(":/image/qq.ico"));
    QMenu * menu = new QMenu(this);
    Global::sysTray->setContextMenu(menu);
//    Global::sysTray->setUserData();
    menu->addAction(tr("show"),this,SLOT(show()));
    menu->addAction(tr("hide"),this,SLOT(hide()));
    menu->addAction(tr("max"),this,SLOT(showMaximized()));
    menu->addAction(tr("normal"),this,SLOT(showNormal()));
    menu->addAction(tr("quit"),this,SLOT(close()));

    QTimer::singleShot(0,this,SLOT(start()));

}

MainForm::~MainForm()
{
        if(Global::udpSocket != NULL)
        {
//                Global::udpSocket->writeDatagram(RSMessage::exit().data(),QHostAddress::Broadcast,2425);

                sentBrocast(Global::udpSocket,RSMessage::exit().data(),2425);
        }
        delete ui;
}

void MainForm::start()
{
        QSettings *setting = new QSettings("user.ini",QSettings::IniFormat,this);
        setting->beginGroup("user");
        Global::userName = setting->value("username").toString();
        Global::groupName = setting->value("groupname").toString();
        setting->endGroup();
        m_setting = setting;
        Global::sysTray->show();
        if(Global::userName.isEmpty())
        {
                LoginDialog login(Global::localUserName);

                if(login.exec() == QDialog::Accepted)
                {
                        Global::userName = login.userName();
                        Global::groupName = login.groupName();
                        qDebug()<<Global::userName<<Global::groupName;
                        setting->beginGroup("user");
                        setting->setValue("username",Global::userName);
                        setting->setValue("groupname",Global::groupName);
                        setting->endGroup();
                        setting->sync();
                }
                else
                {
                        close();
                        return;
                }
        }


    Global::udpSocket = new QUdpSocket(this);
    Global::udpSocket->setSocketOption(QUdpSocket::MulticastLoopbackOption,0);

    if(Global::udpSocket->bind(2425/*,QUdpSocket::ShareAddress*/))
    {
        connect(Global::udpSocket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
//        Global::udpSocket->writeDatagram(RSMessage::entry().data(),QHostAddress::Broadcast,2425);

        sentBrocast(Global::udpSocket,RSMessage::entry().data(),2425);

    }
    else
    {
        QMessageBox::critical(this,tr("Socket Error"),Global::udpSocket->errorString());
        close();
    }

    Global::tcpServer = new QTcpServer(this);

    if(Global::tcpServer->listen(QHostAddress::Any,2425))
    {
            connect(Global::tcpServer,SIGNAL(newConnection()),this,SLOT(onConnection()));
        qDebug()<<"tcp listen ok";
    }
    else
    {
        QMessageBox::critical(this,tr("Socket Error"),Global::tcpServer->errorString());
        close();
    }
    show();
}

void MainForm::onReadyRead()
{
    while (Global::udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(Global::udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        Global::udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);

        RSMessage message(datagram,sender,senderPort);
        qDebug()<<sender<<":"<<datagram;
        if(message.parse())
        {
                qDebug()<<"\n=========udp=========>";
                processMessage(message);
                qDebug()<<"=====================<\n";
        }
    }
}
void MainForm::processMessage(const RSMessage & message)
{
    int command = message.command();
    switch(command & 0x000000ff)
    {
    case IPMSG_BR_ENTRY:
    {
        if(isOtherAddress(message.senderAdress()))
        {
            Global::udpSocket->writeDatagram(RSMessage::ansEntry().data(),message.senderAdress(),message.senderPort());
        }
    }
        break;
    case IPMSG_ANSENTRY:
    {
           UserItem *item ;
           item = list.value(message.senderAdress().toString());
           if(item == NULL)
           {
                   item = new UserItem(message,itemFriends);
                   list.insert(message.senderAdress().toString(),item);
           }
    }
        break;
    case IPMSG_BR_EXIT:
    {
        if(isOtherAddress(message.senderAdress()))
        {
            UserItem *item = list.take(message.senderAdress().toString());
            if(item != NULL)
            {
                delete item;
            }
        }

    }
        break;

    case IPMSG_SENDMSG:
    {
        UserItem *item = list.value(message.senderAdress().toString());
        if(item == NULL)
        {
                item = new UserItem(message,itemFriends);
                list.insert(message.senderAdress().toString(),item);
        }
        //
        if(command & IPMSG_SENDCHECKOPT)
        {
                RSMessage msg(IPMSG_RECVMSG,QString::number(message.packet()));//maoruiflag
            Global::udpSocket->writeDatagram(msg.data(),message.senderAdress(),message.senderPort());
        }
        item->pushMessage(message);


    }
        break;
    case IPMSG_RECVMSG:
    {

    }
        break;
    default:
        break;
    }
}

void MainForm::onTreeItemDoubleClicked(QTreeWidgetItem* item,int index)
{
    UserItem * userItem = dynamic_cast<UserItem*>(item);
    if(userItem != NULL)
    {
        userItem->showChat();
    }
}
void MainForm::onConnection()
{
        QTcpSocket *tcp = Global::tcpServer->nextPendingConnection();
        UserItem *item = list.value(tcp->peerAddress().toString());
        if(item != NULL)
        {
            item->pushRequest(tcp);

        }
        else
        {
            delete tcp;
        }
}
void MainForm::closeEvent(QCloseEvent * e)
{
    qApp->quit();
}

void MainForm::sentBrocast(QUdpSocket *udp, const QByteArray &data, quint16 port)
{
   const QList<QNetworkInterface> & networkInterfaces = QNetworkInterface::allInterfaces();
   foreach (const QNetworkInterface & interface, networkInterfaces) {
       const QList<QNetworkAddressEntry> & addressEntrys = interface.addressEntries();
       foreach (const QNetworkAddressEntry & entry, addressEntrys) {
           const QHostAddress & ip = entry.ip();
           qDebug()<<"brocast to:"<<entry.ip()<<"("<<entry.netmask()<<","<<entry.broadcast()<<")";
           if(!ip.isLoopback() && ip.protocol() == QAbstractSocket::IPv4Protocol)
           {
                 udp->writeDatagram(data,entry.broadcast(),port);
           }
       }

   }

}

bool MainForm::isOtherAddress(const QHostAddress &address)
{
#ifndef TEST_SELF
    const QList<QNetworkInterface> & networkInterfaces = QNetworkInterface::allInterfaces();
    foreach (const QNetworkInterface & interface, networkInterfaces) {
        const QList<QNetworkAddressEntry> & addressEntrys = interface.addressEntries();
        foreach (const QNetworkAddressEntry & entry, addressEntrys) {
            qDebug()<<"compare address:"<<entry.ip()<<":"<<address;
            if(entry.ip() == address)
            {
                return false;
            }
        }

    }
#endif
    return true;
}


