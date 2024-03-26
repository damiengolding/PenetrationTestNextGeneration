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

#include "inc/GlobalIncludes.hpp"
#include "inc/CommandLineHandler.hpp"

using namespace ptng;

void initArgumentParser(QCoreApplication &app, QCommandLineParser &parser);
void initArgumentOptions(QCoreApplication &app, QCommandLineParser &parser);
void processArgumentOptions(QCoreApplication &app, QCommandLineParser &parser);

QString inputFile="";
QString outputStem="";

QList<QCommandLineOption> commandLineOptions;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser p;
    initArgumentParser(a,p);
    //    return(a.exec());
    return(0);
}

void initArgumentParser(QCoreApplication &app, QCommandLineParser &parser){
    app.setApplicationName("ntxt");
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
    // TODO CODE: Add options
    /*
        Various ways to create options:
            Simplest:
                parser.addOption({{"o","output"},"Write to file","file"});
            Create a list:
                QList<QCommandLineOption> commandLineOptions;
                QCommandLineOption op1("short name","description","long name");
                parser.addOption(op1);
                commandLineOptions.append(op1);
        Positional arguments
            parser.addPositionalArgument("hostname", "the target host server to connect to");
            parser.addPositionalArgument("port","the target port to connect to");
    */
    parser.addOption({{"f","file"},"[required] The source file to use. Specify an nmap XML file for open TCP and UDP ports, or a Nessus native XML file for vulnerabilities. Both will output in .tsv and .csv output","file"});
    parser.addOption({{"o","output"},"[required] An output file name stem.","stem"});
}

void processArgumentOptions(QCoreApplication &app, QCommandLineParser &parser){
    if( !parser.isSet("output") || !parser.isSet("file") ){
        parser.showHelp(1);
    }
    inputFile = parser.value("file");
    outputStem = parser.value("output");
    PtngEnums::SupportedInputTypes type  =PtngIdent::checkFile(inputFile);
    if( type == PtngEnums::NMAP || type == PtngEnums::NESSUS ){
        qInfo() << "Processing" << inputFile << "which is of type:" << type;
        if( type == PtngEnums::NMAP  ){
            processNmap(inputFile, outputStem);
        }
        else{
            processNessus(inputFile,outputStem);
        }
    }
    else{
        qCritical() << "Input file"<<inputFile<<"is of incorrect type:"<<type;
        parser.showHelp(2);
    }


}

