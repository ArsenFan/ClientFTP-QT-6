#include "ftpclient.h"
#include <QProcess>
#include <QDebug>

FtpClient::FtpClient(QObject *parent) : QObject(parent) {
}

void FtpClient::setConnectionDetails(const QString &host, const QString &username, const QString &password, int port) {
    this->host = host;
    this->username = username;
    this->password = password;
    this->port = port;
}

void FtpClient::fetchDirectoryList() {
    QProcess process;
    // Przygotuj polecenie curl
    QStringList arguments;
    arguments << QString("ftp://%1:%2/web/homass/").arg(host).arg(port)
              << "--user" << QString("%1:%2").arg(username).arg(password);

    qDebug() << "Wykonuję polecenie: curl" << arguments.join(" ");

    // Uruchom curl.exe
    process.start("curl", arguments);


    if (!process.waitForFinished(10000)) { // Czekaj maksymalnie 10 sekund
        QString error = process.errorString();
        qDebug() << "Błąd curl:" << error;
        emit errorOccurred(error);
        return;
    }


    // Pobierz odpowiedź
    QString response = QString::fromUtf8(process.readAllStandardOutput());
    qDebug() << "Odpowiedź serwera FTP:" << response;

    // Pobierz błędy, jeśli są
    QString errorOutput = QString::fromUtf8(process.readAllStandardError());
    if (!errorOutput.isEmpty()) {
        qDebug() << "Błędy curl:" << errorOutput;
        emit errorOccurred(errorOutput);
        return;
    }

    // Podziel odpowiedź na linie
    QStringList lines = response.split("\n", Qt::SkipEmptyParts);
    QStringList directories;

    // Filtruj katalogi
    for (const QString &line : lines) {
        if (line.contains("drwxr")) { // Katalogi mają 'drwxr'
            QStringList parts = line.split(" ", Qt::SkipEmptyParts);
            if (parts.size() > 8) {
                directories.append(parts.last());
            }
        }
    }

    // Emituj sygnał z listą katalogów
    emit directoryListFetched(directories);
}
