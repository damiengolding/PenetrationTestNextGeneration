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

#include "main.hpp"

// QString logFile = "D:\\Development\\Sources\\Repos\\Qt\\TestLogs\\ztest.txt";
QString logFile = "log.txt";
QString configFile="";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#ifndef QT_DEBUG
    qInfo() << "This application only runs in debug mode (#ifdef QT_DEBUG)";
    return(0);
#endif
    QCommandLineParser p;
    initArgumentParser(a,p);

    // Setup logger
    Logger::fileName = logFile;
    Logger::install();
    if( configFile.isEmpty() || !QFile::exists(configFile) ){
        qCritical() << "A valid XML configuration file must be supplied. Exiting.";
        p.showHelp();
    }
    // Setup libparser library config
    PtngConfig::testConfiguration = configFile;
    // qInfo() << "Current directory:"<<QDir::currentPath();
    // Run tests
    if( testTypes.contains("all",Qt::CaseInsensitive) ){
        testInput();
        testConversion();
        return(0);
    }
    else if( testTypes.contains("input",Qt::CaseInsensitive) ){
        testInput();
        return(0);
    }
    else if( testTypes.contains("conv",Qt::CaseInsensitive) ){
        testConversion();
        return(0);
    }
    // return(a.exec());
    return(0);
}

void initArgumentParser(QCoreApplication &app, QCommandLineParser &parser){
    app.setApplicationName("ztest");
    app.setApplicationVersion("0.0.1");
    app.setOrganizationName("Golding's Gym");
    app.setOrganizationDomain("https://github.com/damiengolding");
    // Convenience options
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription("ztest Test harness for libparser");
    // Init options from here, defined separately in initArgumentOptions
    initArgumentOptions(app,parser);
    // Process the supplied arguments into the parser
    parser.process(app);
    // Process the arguments supplied to the application so we can use them
    processArgumentOptions(app, parser);
}

void initArgumentOptions(QCoreApplication &app, QCommandLineParser &parser){
    parser.addPositionalArgument("list-types", "List supported types");
    parser.addOption({{"t","types"},"Tests to run. A comma delimited list: any of input,builder,conv,misc or just all, default: all","types"});
    parser.addOption({{"l","log"},"File to log to. Default is ./log.txt","log file"});
    parser.addOption({{"c","config"},"[required] Configuration XML file location","config"});
}

void processArgumentOptions(QCoreApplication &app, QCommandLineParser &parser){
    if( parser.positionalArguments().contains("list-types") ){
        listTypes();
        return; // Treat positional arguments separately, don't process with options
    }
    if( parser.isSet("types")){
        testTypes = parser.value("types").split(",");
    }
    else{
        testTypes << "all";
    }

    if( parser.isSet("log") ){
        logFile = parser.value("log");
    }

    if( parser.isSet("config") ){
        configFile = parser.value("config");
    }

}

void listTypes(){
    qInfo() << "Currently supported test type values (every option implicitly tests PtngEnums and PtngSpecifications)  :";
    qInfo() << "input - tests PtngIdent and PtngInputParser";
    qInfo() << "builder - tests PtngHostBuilder and PtngDGMLBuilder";
    qInfo() << "conv - tests PtngDGMLConv";
    qInfo() << "misc- tests PtngIP4Address";
    qInfo() << "all - speaks for itself, really";
}

