#ifndef CHATFORM_H
#define CHATFORM_H

#include <QWidget>

class RSMessage;
class UserItem;
class QTcpSocket;
class RSFileModel;

namespace Ui {
    class ChatForm;
}

class ChatForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChatForm(UserItem * item,QWidget *parent = 0);
    void pushRequest(QTcpSocket * tcp);
    void pushMessage(const RSMessage & msg);
    void showMessage();
    const UserItem* userItem()const;

    ~ChatForm();
public slots:
    void sendMessage();
    void onTimerOut();

    void onRecvClicked();
    void onAddClicked();
    void onAddFolderClicked();
private:
    Ui::ChatForm *ui;
    //UI
    UserItem *m_item;
    QTimer *m_timer;
    QTcpSocket * tcp;
    RSFileModel *recvModel;
    RSFileModel *sendModel;
};

#endif // CHATFORM_H
