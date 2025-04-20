#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>
#include <QStringList>

class FtpClient : public QObject {
    Q_OBJECT
public:
    explicit FtpClient(QObject *parent = nullptr);

    // Function for setting connection parameters
    void setConnectionDetails(const QString &host, const QString &username, const QString &password, int port = 21);

    // Function to download directory list
    void fetchDirectoryList();

signals:
    // Signal emitted after successful download of directory list
    void directoryListFetched(const QStringList &directories);

    // Signal emitted in case of error
    void errorOccurred(const QString &errorMessage);

private:
    QString host;
    QString username;
    QString password;
    int port;
};

#endif // FTPCLIENT_H
