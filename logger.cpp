#include "logger.h"
#include <QDateTime>
#include <QTextStream>
#include <QCoreApplication>

QString Logger::fileName = "";
QFile Logger::file;

Logger::Logger()
{

}

void Logger::write(QString type, QString text){
    fileName = QCoreApplication::applicationDirPath() +
               "/Logs/" +
               QDateTime::currentDateTime().toString("yyyyMMdd") +
               "_log.txt";
    file.setFileName(fileName);
    if (file.exists())
        file.open(QIODevice::Append | QIODevice::Text);
    else
        file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << "[" << type << "]" <<QDateTime::currentDateTime().toString("[hh:mm:ss] ") << text << endl;
    file.close();
}
