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
#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

#include "GlobalIncludes.hpp"

using namespace ptng;

void initArgumentParser(QCoreApplication &app, QCommandLineParser &parser);
void initArgumentOptions(QCoreApplication &app, QCommandLineParser &parser);
void processArgumentOptions(QCoreApplication &app, QCommandLineParser &parser);

QList<QCommandLineOption> commandLineOptions;
int ret = 0;
QString inputFile="";
QString outputFile="";

void processFile(const QString& inputFile,const QString& outputFile, bool labels);
void showTypes();
bool fileIsSupported(const QString &inputFile);
bool showLabels=true;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser p;
    initArgumentParser(a,p);
    //    return(a.exec());
    return(ret);
}

void initArgumentParser(QCoreApplication &app, QCommandLineParser &parser){
    app.setApplicationName("ndgml");
    app.setApplicationVersion("0.0.1");
    app.setOrganizationName("Golding's Gym");
    app.setOrganizationDomain("");
    // Convenience options
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription("myapp description");
    // Init options from here, defined separately in initArgumentOptions
    initArgumentOptions(app,parser);
    // Process the supplied arguments into the parser
    parser.process(app);
    // Process the arguments supplied to the application so we can use them
    processArgumentOptions(app, parser);
}

void initArgumentOptions(QCoreApplication &app, QCommandLineParser &parser){
    parser.addPositionalArgument("types","Currently supported input file types");
    parser.addOption({{"f","file"},"Write to file. Use \'ndgml types\' to list supported types.","file"});
    parser.addOption({{"o","output"},"Write to file","file"});
    parser.addOption({{"l","labels"},"Add labels to links; this is necessary for conversion to dot runcontrol (default is '\on\')","on|off"});
}

void processArgumentOptions(QCoreApplication &app, QCommandLineParser &parser){
    PtngEnums::SupportedInputTypes type;
    // Positional arguments
    for(QString pos : parser.positionalArguments()){
        if( pos.toLower() == "types"){
            showTypes();
        }
    }
    // Don't handle poitional arguments and options together, one or the other
    if( parser.positionalArguments().count() > 0 ){
        ret = 0;
        return;
    }

    if(!parser.isSet("file")){
        qWarning() << "[fatal] No network map source file specified";
        parser.showHelp();
        ret = 1;
        return;
    }
    else{
        inputFile = parser.value("file");
        if( !QFile::exists(inputFile) ){
            qInfo() << "[fatal] Specified source file does not exist:"<<inputFile;
            parser.showHelp();
            ret = 2;
            return;
        }
        else{
            type = PtngIdent::checkFile(inputFile);
            if( !fileIsSupported(inputFile)){
                qInfo() << "[info] File" << inputFile << "is not supported by ndgml. Use ndgml types for a list.";
                qInfo() << "[info] Supplied file is of type:" << type;
            }
            // else{
            //     qInfo() << "[info] Processing" << inputFile << "which is of type:" << type;
            //     processFile(inputFile);
            // }
        }
    }

    // Output file
    if( parser.isSet("output") ){
        outputFile = parser.value("output");
    }

    // Show labels
    if( parser.isSet("labels") ){
        if( parser.value("labels").toLower() == "on"){
            showLabels = true;
        }
        else if(parser.value("labels").toLower() == "off"){
            showLabels = false;
        }
        else{
            showLabels = true;
        }
    }

    qInfo() << "[info] Processing" << inputFile << "which is of type:" << type;
    processFile(inputFile,outputFile,showLabels);
}

