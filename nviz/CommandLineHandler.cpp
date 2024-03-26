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

void processFile(const QString &sourceFile,
                 const QString &conversionType,
                 const QString &outputFileStem,
                 const QString &command){

    PtngEnums::SupportedInputTypes type = PtngIdent::checkFile(sourceFile);
    if( type == PtngEnums::NESSUS ){
        qInfo() << "Processing Nessus native XML";
        return(processNessus(sourceFile,conversionType,outputFileStem,command));
    }
    QScopedPointer<QFile> file( new QFile(sourceFile ) ) ;
    QScopedPointer<QDomDocument> doc(new QDomDocument());
    if( !file->open(QIODevice::ReadOnly)  ){
        qCritical() << "Could not open"<<sourceFile<<"For reading";
        return;
    }
    if( !doc->setContent(file.data()) ){
        qCritical() << "Failed parsing"<<sourceFile;
        file->close();
    }
    QDomElement root = doc->documentElement();
    if( !root.hasAttribute("Title") ){
        qCritical() << "The XML document root does not have a \"Title\" attribute. It does not appear to be a valid PTNG DGML file.";
        return;
    }
    QString title = root.attribute("Title");
    if( title.toLower() == "nmapdgml" ){
        qInfo() << "Processing NmapDGML input";
        processNmap( sourceFile,conversionType,outputFileStem,command );
    }
    else if( title.toLower() == "nessusdgml" ){
        qInfo() << "Processing NessusDGML input";
        processNessus( sourceFile,conversionType,outputFileStem,command );
    }
}

void processNmap(const QString &sourceFile,
                 const QString &conversionType,
                 const QString &outputFileStem,
                 const QString &command){
    QScopedPointer<QFile> file( new QFile(sourceFile ) ) ;
    qInfo() << "Starting to process input";
    if( !file->open(QIODevice::ReadOnly)  ){
        qCritical() << "Could not open"<<sourceFile<<"For reading";
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
        qCritical() << "Could not open file for writing";
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
    qInfo() << "Completed processing input";
    if( outputFileStem.isEmpty() ){
#ifdef QT_DEBUG
        qDebug() << "dot runcontrol:"<<dotOutput;
#endif
    }
}

void processNessus(const QString &sourceFile,
                   const QString &conversionType,
                   const QString &outputFileStem,
                   const QString &command){
    PtngEnums::SupportedInputTypes type = PtngIdent::checkFile(sourceFile);
    if( type == PtngEnums::NESSUS ){
        processNessusXml(sourceFile,conversionType,outputFileStem,command);
    }
    else if( conversionType.toLower() == "dot" ){
        qInfo() << "Nessus conversion to dot";
        return(processNmap(sourceFile,conversionType,outputFileStem,command));
    }
    else if( conversionType.toLower() == "hvln" || conversionType.toLower() == "vplg" ){
        qInfo() << "Nessus native XML conversion";
        return(processNessusXml(sourceFile,conversionType,outputFileStem,command));
    }
    else{
        qCritical() << "Conversion type"<<conversionType<<"is not recognised/supported.";
    }

    qInfo() << "Completed processing input";
}

void processNessusXml(const QString &sourceFile,
                      const QString &conversionType,
                      const QString &outputFileStem,
                      const QString &command){
    QString conv = conversionType.toLower();
    if( conv == "dot" ){
        qCritical() << "Nessus native XML conversion to dot is not supported. Use ndgml first to create a dgml source file.";
        return;
    }
    else if( conv == "hvln" ){
        processNessusVulns(sourceFile,conversionType,outputFileStem,command);
    }
    else if( conv == "vplg" ){
        processNessusPluginFamilies(sourceFile,conversionType,outputFileStem,command);
    }
    else{
        qCritical() << "Conversion type"<<conversionType<<"is not recognised/supported.";
    }
}
