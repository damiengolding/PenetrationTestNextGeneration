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
#pragma once

#include "inc/CommandLineHandler.hpp"
#include <QFontInfo>

// Argument processing

void processFile(const QString &dgmlSource,
                 const QString &conversionType,
                 const QString &outputFileStem,
                 const QString &command){
    QScopedPointer<QFile> file( new QFile(dgmlSource ) ) ;
    qInfo() << "[info] Starting to process input";
    if( !file->open(QIODevice::ReadOnly)  ){
        qWarning() << "[warning] Could not open"<<dgmlSource<<"For reading";
        return;
    }
    QString dgml = file->readAll();
    QScopedPointer<PtngDGMLConv> converter(new PtngDGMLConv());
    bool ok = false;
    QString dotOutput = converter->toDot(dgml,ok);

    // Create output file and write the result
    QString ofName = outputFileStem;
    if( ofName.isEmpty() ){
        ofName = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()) % "_";
        ofName += "network_model";
        ofName += ".dot";
    }
    else{
        if( !ofName.endsWith(".dot") ){
            ofName += ".dot";
        }
    }
    QFile oFile(ofName);
    if( !oFile.open(QIODevice::WriteOnly) ){
        qWarning() << "[warning] Could not open file for writing";
        return;
    }
    QTextStream out(&oFile);
    out <<dotOutput;
    oFile.close();
    file->close();

    // Execute the command if one is supplied
    if( command.isEmpty() ){
        return;
    }
    QString procCmd = command;
    procCmd = procCmd.replace("%input_file%",ofName);
    QProcess proc;
    proc.startCommand(procCmd);
    proc.waitForFinished();
    qInfo() << "[info] Completed processing input";
    // if( outputFileStem.isEmpty() ){
    //     qInfo() << "[info] dot runcontrol:"<<dotOutput;
    // }
}
