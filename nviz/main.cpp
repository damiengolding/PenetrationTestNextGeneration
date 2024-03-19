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

void initArgumentParser(QCoreApplication &app, QCommandLineParser &parser);
void initArguments(QCoreApplication &app, QCommandLineParser &parser);
void processArguments(QCoreApplication &app, QCommandLineParser &parser);

QList<QCommandLineOption> commandLineOptions;

QString sourceFile="";
QString conversionType="";
QString outputFileStem="";
QString command="";
QString subnetFilters="";

int ret = 0;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser p;
    initArgumentParser(a,p);
    //    return(a.exec());
    return(ret);
}

void initArgumentParser(QCoreApplication &app, QCommandLineParser &parser){
    app.setApplicationName("nviz");
    app.setApplicationVersion("0.0.1");
    app.setOrganizationName("Golding's Gym");
    app.setOrganizationDomain("");

    // Convenience options
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription("nviz (network topology) converts a DGML file generated by ndgml into an output type. Currently supported options depend on the type of conversion but are dot, tsv, csv, png");

    // Init options from here, defined separately in initArgumenviztions
    initArguments(app, parser);
    // Process the supplied arguments into the parser
    parser.process(app);
    // Process the arguments supplied to the application so we can use them
    processArguments(app, parser);
}

void initArguments(QCoreApplication &app, QCommandLineParser &parser){
    // Options
    parser.addOption({{"f","file"},"[required] The source file to use.","file"});
    parser.addOption({{"c","conversion"},"[required] The required conversion type. Currently supported switches are: dot, hvln, vplg","conversion"});
    parser.addOption({{"o","output"},"[required] An output file name stem.","stem"});
    //dot.exe <*****.dot> -Tpng -o <imagename.png>
    parser.addOption({{"e","exec"},"[optional] A command to call on the output file (name supplied with the -o/--output option). For instance, to obtain a dot network model enter: dot.exe %input_file% -Tpng -o <imagename.png> ","command"});

}

void processArguments(QCoreApplication &app, QCommandLineParser &parser){
    PtngEnums::SupportedInputTypes type;
     // Options
    // dgml source file - mandatory
    if(!parser.isSet("file")){
        qWarning() << "[fatal] No dgml source file specified";
        parser.showHelp();
        ret = 1;
        return;
    }
    else{
        sourceFile = parser.value("file");
        if( !QFile::exists(sourceFile) ){
            qWarning() << "[warning] Specified dgml source file"<<sourceFile<<"does not exist.";
            parser.showHelp();
            ret = 2;
            return;
        }
        else{
            type = PtngIdent::checkFile(sourceFile);
            if( type  == PtngEnums::DGML || type == PtngEnums::NESSUS){
                qInfo() << "[info] Processing" << sourceFile << "which is of type:" << type;
            }
            else{
                qInfo() << "[info] File" << sourceFile << "is not supported by nviz.";
                qInfo() << "[info] Supplied file is of type:" << type;
            }
        }
    }

    // Converson type
    if( parser.isSet("conversion")){
        QString t = parser.value("conversion").toLower();
        if( t == "dot" ||  t == "hvln" || t == "vplg"  ){
            qInfo() << "[info] The supplied conversion type is set to:" <<t;
            conversionType = t;
        }
        else{
            qWarning() << "[warning] The supplied conversion type is not recognised.";
            parser.showHelp();
            ret = 1;
            return;
        }
    }

    // Output file stem
    if( parser.isSet("output")  ){
        outputFileStem= parser.value("output");
        qInfo() << "[info] Optional argument output set to:" <<outputFileStem;
    }
    else{
        qInfo() << "[info] Optional argument output not set. Moving on.";
    }
    // Command to execute on the output file
    if( parser.isSet("exec") ){
        command = parser.value("exec");
        qInfo() << "[info] Optional argument exec set to:" <<command;
    }
    else{
        qInfo() << "[info] Optional argument exec not set. Moving on.";
    }

    processFile(sourceFile,conversionType,outputFileStem,command);
}
