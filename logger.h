#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <QString>
using namespace std;
#include <QFile>

class Logger{
private:
    static QString fileName;
    static QFile file;
public:
    Logger();
    static void write(QString type, QString text);
};

const QString LOG_WARNING = "Warning";
const QString LOG_ERROR = "Error";
const QString LOG_INFO = "INFO";

#endif // LOGGER_H
