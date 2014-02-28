#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
        Q_OBJECT
        
public:
        explicit LoginDialog(const QString & user,const QString & group = QString(),QWidget *parent = 0);
        QString  userName()const;
        QString  groupName()const;
        ~LoginDialog();

private:
        Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
