/*
MIT License

Copyright (c) 2024 Damien Golding

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Don't use it to find and eat babies ... unless you're really REALLY hungry ;-)
*/
#include "Logger.hpp"

QString Logger::fileName = QDir::currentPath() % QDir::separator() % "log.txt";
bool Logger::isLogging = false;
QFile Logger::outFile(Logger::fileName);
static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);

Logger::Logger(QObject *parent)
    : QObject{parent}{}

void Logger::install()
{
    Logger::outFile.setFileName(Logger::fileName);
    if( !Logger::outFile.open(QIODevice::Append) ){
        qCritical() << "Inable to open log file:"<<Logger::fileName<<"for QIODeveice::Append";
        return;
    }
    Logger::isLogging = true;
    qInstallMessageHandler(Logger::messageHandler);
}

void Logger::uninstall()
{
    if( Logger::outFile.isOpen() ){
        Logger::outFile.close();
    }
    qInstallMessageHandler(nullptr);
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &ctxt, const QString &msg)
{
    if( Logger::isLogging ){
        QString txt;
        switch (type){
        case QtInfoMsg:{
            txt = QString("[INFO] %1").arg(msg);
            break;
        }
        case QtDebugMsg:{
#ifdef QT_DEBUG
            txt = QString("[DEBUG] %1").arg(msg);
#endif
            break;
        }
        case QtWarningMsg:{
            txt = QString("[WARNING] %1").arg(msg);
            break;
        }
        case QtCriticalMsg:{
            txt = QString("[CRITICAL] %1").arg(msg);
            break;
        }
        case QtFatalMsg:{
            txt = QString("[FATAL] %1").arg(msg);
            break;
        }
        }
        if( !txt.isEmpty()){
            if( !Logger::outFile.isOpen() ){
                Logger::outFile.setFileName(Logger::fileName);
                Logger::outFile.open(QIODevice::Append);
            }
            QTextStream ts(&Logger::outFile);
            ts << txt << " - "
               << QDateTime::currentDateTime().toString() << " - "
               << " from file: " << ctxt.file
               << " in funtion: " << ctxt.function
               <<  " at line: " << ctxt.line << "\r\n";
            ts.flush();
        }
        (*QT_DEFAULT_MESSAGE_HANDLER)(type,ctxt,txt);
    }
    else{
        (*QT_DEFAULT_MESSAGE_HANDLER)(type,ctxt,msg);
    }
}
