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
#include <iostream>
#include <string>
using namespace std;

#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include "PtngEnums.hpp"
#include "PtngIdent.hpp"
using namespace ptng;

void initArgumentParser(QCoreApplication &app, QCommandLineParser &parser);
void initArgumentOptions(QCommandLineParser &parser);
void processArgumentOptions(QCommandLineParser &parser);
void processFile(const QString &file);

QList<QCommandLineOption> commandLineOptions;

int main(int argc, char *argv[])
{
    qInfo() << "[info] Starting ifident";
    QCoreApplication a(argc, argv);
    QCommandLineParser p;
    initArgumentParser(a,p);
    //    return(a.exec());
    return(0);
}

void initArgumentParser(QCoreApplication &app, QCommandLineParser &parser){
    app.setApplicationName("ifident");
    app.setApplicationVersion("0.0.1");
    app.setOrganizationName("Golding's Gym");

    // Convenience options
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription("ifident takes output files and\nattempts to identify the source tool from a number of\ncommon pen testing tools such as nmap, dig etc ");
    // Init options from here, defined separately in initArgumentOptions
    initArgumentOptions(parser);
    // Process the supplied arguments into the parser
    parser.process(app);
    // Process the arguments supplied to the application so we can use them
    processArgumentOptions(parser);
}

void initArgumentOptions(QCommandLineParser &parser){
    parser.addOption({{"f","file"},"File to identify <file>","file"});
}

void processArgumentOptions(QCommandLineParser &parser){
    if(parser.isSet("file")){
        QString inputFile = parser.value("f");
        // cout << "Input file: " << qPrintable(inputFile) << endl;
        qInfo() << "[info]"<< "Input file:" << qPrintable(inputFile);
        if( !QFile::exists(inputFile) ){
            qWarning() << "[warning]" << "File" << inputFile <<  "does not exist.";
            return;
        }
        else{
            qInfo() << "[info]"<< "Opening file" << qPrintable(inputFile) << "for processing.";
            processFile(inputFile);
        }
    }
    else{
        qWarning() << "[warning] No input file specified - use -h or --help for options";
        return;
    }
}

void processFile(const QString &file){
    QScopedPointer<PtngIdent> ident(new PtngIdent());
    PtngEnums::SupportedInputTypes ret = PtngEnums::NUM_SUPPORTED_INPUT_TYPES;
    ret = ident->checkFile(file);
    qInfo() << "[info]" << "File type guess:" << ret;
}


