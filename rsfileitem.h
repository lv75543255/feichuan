#ifndef MSGFILE_H
#define MSGFILE_H
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QTcpSocket>
class QFile;
class RSFileModel;
class RSFileItem:public QObject
{
        Q_OBJECT
public:
        explicit RSFileItem(RSFileModel *parent = 0);
        RSFileItem(RSFileModel* parent,int packetNo,const QString &data);
        RSFileItem(RSFileModel* parent,const QString &file);
        ~RSFileItem();

        bool parse();
        int packet()const;//包号,从RSMessage 得到
        int number()const;//文件标识号
        const QString & filename()const;
        int filesize()const;
        const QString & modifyTime()const;
        int fileattr()const;

        const QString & fullname()const;

        QString toSendDiscription(long long offset = 0)const;
        QString toGetDiscription(long long offset = 0)const;
        void setSendSocket(QTcpSocket *tcp);

        void stopGetFile();
        void startGetFile();
signals:
        void update(int percent);
private slots:
        void onError(QAbstractSocket::SocketError);
        void onReadyData();
        void onConnected();
        void onDisconnected();
private:
        int m_packetNo;
        int m_number;
        QString m_filename;
        long long m_filesize;
        QString m_modifyTime;
        int m_fileattr;
        QString m_externData;
        QString m_data;

        QFile *m_file;
        QTcpSocket *m_tcp;
};


#endif // MSGFILE_H
