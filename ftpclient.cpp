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
    // Prepare curl command
    QStringList arguments;
    arguments << QString("ftp://%1:%2/web/homass/").arg(host).arg(port)
              << "--user" << QString("%1:%2").arg(username).arg(password);

    //qDebug() << "I am executing the command: curl" << arguments.join(" ");

    // Run curl
    process.start("curl", arguments);


    if (!process.waitForFinished(10000)) { // Wait a maximum of 10 seconds
        QString error = process.errorString();
        qDebug() << "Błąd curl:" << error;
        emit errorOccurred(error);
        return;
    }


    // Download Answer
    QString response = QString::fromUtf8(process.readAllStandardOutput());
    qDebug() << "FTP server response:" << response;

    // Download errors if any
    QString errorOutput = QString::fromUtf8(process.readAllStandardError());
    if (!errorOutput.isEmpty()) {
        qDebug() << "curl errors:" << errorOutput;
        emit errorOccurred(errorOutput);
        return;
    }

    // Divide the answer into lines
    QStringList lines = response.split("\n", Qt::SkipEmptyParts);
    QStringList directories;

    // Filter directories
    for (const QString &line : lines) {
        if (line.contains("drwxr")) { // Directories have 'drwxr'
            QStringList parts = line.split(" ", Qt::SkipEmptyParts);
            if (parts.size() > 8) {
                directories.append(parts.last());
            }
        }
    }

    // Broadcast directory list signal
    emit directoryListFetched(directories);
}
