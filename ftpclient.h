#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>
#include <QStringList>

class FtpClient : public QObject {
    Q_OBJECT
public:
    explicit FtpClient(QObject *parent = nullptr);

    // Funkcja do ustawienia parametrów połączenia
    void setConnectionDetails(const QString &host, const QString &username, const QString &password, int port = 21);

    // Funkcja do pobrania listy katalogów
    void fetchDirectoryList();

signals:
    // Sygnał emitowany po pomyślnym pobraniu listy katalogów
    void directoryListFetched(const QStringList &directories);

    // Sygnał emitowany w przypadku błędu
    void errorOccurred(const QString &errorMessage);

private:
    QString host;
    QString username;
    QString password;
    int port;
};

#endif // FTPCLIENT_H
