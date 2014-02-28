#include "logindialog.h"
#include "ui_logindialog.h"
#include "global.h"
#include <QSettings>
LoginDialog::LoginDialog(const QString &user, const QString &group, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::LoginDialog)
{
        ui->setupUi(this);
        ui->lineEditName->setText(user);
        ui->lineEditGroup->setText(group);
}

LoginDialog::~LoginDialog()
{
        delete ui;
}
QString LoginDialog::userName() const
{
        return ui->lineEditName->text();
}
QString LoginDialog::groupName() const
{
        return ui->lineEditGroup->text();
}
