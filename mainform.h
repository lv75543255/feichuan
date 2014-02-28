#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <QMap>
#include <QHostAddress>
namespace Ui {
    class MainForm;
}
class QSystemTrayIcon;
class QTreeWidgetItem;
class QUdpSocket;
class RSMessage;
class QSettings;
class UserItem;
class MainForm : public QWidget
{
    Q_OBJECT
public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();
public slots:
    void start();
    void onReadyRead();
    void onConnection();
    void onTreeItemDoubleClicked(QTreeWidgetItem*,int);
protected:
    void closeEvent(QCloseEvent *);
private:
    void processMessage(const RSMessage &);
    Ui::MainForm *ui;
    QTreeWidgetItem * itemFriends;

    QMap<QString,UserItem*> list;

    QSettings *m_setting;
};

#endif // MAINFORM_H
