#include "chatform.h"
#include "ui_chatform.h"
#include "feiproto.h"
#include "useritem.h"
#include "rsfileitem.h"
#include "global.h"
#include "rsfilemodel.h"
#include <QTime>
#include <QTimer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTableView>
#include <QPushButton>
#include <QGridLayout>
#include <QTextEdit>
#include <QListWidget>
#include <QFileDialog>
#include <QSystemTrayIcon>

ChatForm::ChatForm(UserItem *item, QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ChatForm),
        m_item(item),m_timer(NULL)
{
        Ui::ChatForm * ui = this->ui;
        ui->setupUi(this);

        setWindowTitle(tr("Chating with ") +  m_item->nickName());
        ui->labelName->setText(item->nickName());
        ui->labelGroup->setText(item->groupName());
        ui->labelUser->setText(item->userName());
        ui->labelHost->setText(item->hostName());

        recvModel = new RSFileModel(this);
        ui->recvView->setModel(recvModel);
        sendModel = new RSFileModel(this);
        ui->sendView->setModel(sendModel);

        ui->recvView->setColumnWidth(0,70);
        ui->recvView->setColumnWidth(1,180);
        ui->sendView->setColumnWidth(0,70);
        ui->sendView->setColumnWidth(1,180);

        connect(ui->sendMsgButton,SIGNAL(clicked()),this,SLOT(sendMessage()));
        connect(ui->reciveButton,SIGNAL(clicked()),this,SLOT(onRecvClicked()));
        connect(ui->fileButton,SIGNAL(clicked()),this,SLOT(onAddClicked()));
        connect(ui->folderButton,SIGNAL(clicked()),this,SLOT(onAddFolderClicked()));
        tcp  = new QTcpSocket(this);
}

ChatForm::~ChatForm()
{

}
void ChatForm::pushRequest(QTcpSocket * tcp)
{

}

void ChatForm::pushMessage(const RSMessage &message)
{
        if(message.command() & IPMSG_FILEATTACHOPT)
        {

                const QStringList &files = message.additional().split(QChar('\a'));
                qDebug()<<"have files"<<files;
                foreach(const QString &file,files)
                {
                        qDebug()<<file.size()<<":"<<file;
                        RSFileItem *item = new  RSFileItem(recvModel,message.packet(),file);
                        if(item->parse())
                        {
                                recvModel->addRSFileItem(item);
                        }
                        else
                        {
                                delete item;
                        }
                }

        }

        ui->textEdit->append(QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; color:#0000ff;\">%1  %2</span></p>")
                             .arg(m_item->nickName(),QTime::currentTime().toString("hh:mm:ss")));
        ui->textEdit->append("   " + message.content());

        if(isHidden())
        {
                Global::sysTray->showMessage(tr("Message"),tr("New message from ") + m_item->nickName());
                if(m_timer == NULL)
                {
                        m_timer = new QTimer(this);
                        connect(m_timer,SIGNAL(timeout()),this,SLOT(onTimerOut()));
                        m_timer->start(1000);
                }
        }
}
void ChatForm::sendMessage()
{
        const QString & text = ui->sendEdit->toPlainText();
        m_item->sendMessage(text);
        ui->sendEdit->clear();
        ui->textEdit->append(QString("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; color:#0000ff;\">%1  %2</span></p>")
                             .arg("Me ",QTime::currentTime().toString("hh:mm:ss")));
        ui->textEdit->append("   " +text );
}
void ChatForm::showMessage()
{
        if(m_timer)
        {
                m_timer->stop();
                m_timer = NULL;
        }
        show();
}
const UserItem* ChatForm::userItem()const
{
     return m_item;
}
void ChatForm::onTimerOut()
{
        //    qDebug()<<"flicker";
        m_item->flicker();
}

void ChatForm::onRecvClicked()
{
        //file recive
        const QModelIndex & index = ui->recvView->currentIndex();
        if(index.isValid())
        {
                RSFileItem *file = recvModel->getRSFileItem(index);
                file->startGetFile();

        }
}
void ChatForm::onAddClicked()
{

        QString file = QFileDialog::getOpenFileName(this,tr("Open file"),qgetenv("HOME"));
        sendModel->addRSFileItem(new RSFileItem(sendModel,file));
}
void ChatForm::onAddFolderClicked()
{
        QString file = QFileDialog::getExistingDirectory(this,tr("Open file"),qgetenv("HOME"));
        sendModel->addRSFileItem(new RSFileItem(sendModel,file));
}
