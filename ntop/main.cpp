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
#include "CommandLineHandler.hpp"

void initArgumentParser(QCoreApplication &app, QCommandLineParser &parser);
void initArgumentOptions(QCoreApplication &app, QCommandLineParser &parser);
void processArgumentOptions(QCoreApplication &app, QCommandLineParser &parser);

QList<QCommandLineOption> commandLineOptions;

QString networkMapSource="";
QString zoneTransferFile="";
QString hshFile="";
QString ipLocationAPIKey="";
QString subnetFilter="";
QString outputFileStem="";
QString fontFamily="";
int ret = 0;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser p;
    initArgumentParser(a,p);
    parseInputFiles(networkMapSource,zoneTransferFile,hshFile,ipLocationAPIKey,subnetFilter,outputFileStem,fontFamily);
    //    return(a.exec());
    return(ret);
}

void initArgumentParser(QCoreApplication &app, QCommandLineParser &parser){
    app.setApplicationName("ntop");
    app.setApplicationVersion("0.0.1");
    app.setOrganizationName("Golding's Gym");
    app.setOrganizationDomain("");

    // Convenience options
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription("Converts an nmap xml file to visual network representations.\n"
                                     "\tThe ***_model_***.png image shows hosts grouped by network\n"
                                     "\tThe ***_layout_***.png image shows a directed graph depicting the layout of the network\n"
                                     "\t\n"
                                     "\tThe two .dot file artefacts are the Dot (from GraphViz by AT&T) run control files used\n"
                                     "\tby dot.exe to generate the images. They can be modified and the files regenerated afterwards\n"
                                     "\tby calling dot.exe <*****.dot> -Tpng -o <imagename.png>\n");

    // Init options from here, defined separately in initArgumentOptions
    initArgumentOptions(app, parser);
    // Process the supplied arguments into the parser
    parser.process(app);
    // Process the arguments supplied to the application so we can use them
    processArgumentOptions(app, parser);
}

void initArgumentOptions(QCoreApplication &app, QCommandLineParser &parser){
    // Options
    parser.addOption({{"i","input"},"[required] The network file to use (to view the currently supported formats use ntop -t | --types option)","file"});
    parser.addOption({{"x","axfr"},"[optional] The Zone Transfer to use (to view the currently supported formats use ntop -t | --types option)","file"});
    parser.addOption({{"s","sev"},"[optional] The Highest Severity by Host (*.hsh) file to use. See nhsh.exe for details about how to generate this file type","file"});
    parser.addOption({{"a","api"},"[optional] An API key for geolocation (either IPInfoDB or IP2Location","key"});
    parser.addOption({{"n","net"},"[optional] A subnet filter in the form 192.168.1. (use the trailing \'.\' to avoid including 192.168.10. and 192.168.100.)","subnet"});
    parser.addOption({{"o","output"},"[optional] An output file name stem. This will be prefixed by a system date/time string. If it is omitted just the system date/time string will be used as the output file name(s)","stem"});
    parser.addOption({{"f","font"},"[optional] The font family to use in image generaton","font"});
    // Positional arguments
    parser.addPositionalArgument("types","Currently supported input file types");
}

void processArgumentOptions(QCoreApplication &app, QCommandLineParser &parser){
    QScopedPointer<PtngIdent> ident(new PtngIdent());
    PtngEnums::SupportedInputTypes type;
    // Positional arguments
    for(QString pos : parser.positionalArguments()){
        if( pos.toLower() == "types"){
            showTypes();
        }
    }
    if( parser.positionalArguments().count() > 0 ){
        ret = 0;
        return;
    }
    // Options
    // network source file - mandatory
    if(!parser.isSet("input")){
        qWarning() << "[fatal] No network map source file specified";
        parser.showHelp();
        ret = 1;
        return;
    }
    else{
        networkMapSource = parser.value("input");
        if( !QFile::exists(networkMapSource) ){
            std::cout << "[fatal] Specified network map source file does not exist." << std::endl;
            parser.showHelp();
            ret = 2;
            return;
        }
        else{
            type = ident->checkFile(networkMapSource);
            if( type  != PtngEnums::NMAP){
                qInfo() << "[info] File" << networkMapSource << "is not supported by ntop. Use ntop types for a list.";
                qInfo() << "[info] Supplied file is of type: " << type;
            }
            else{
                qInfo() << "[info] Processing" << networkMapSource << "which is of type:" << type;
            }
        }
    }

    // Other (optional) options
    if( parser.isSet("axfr")  ){
        type = ident->checkFile(parser.value("axfr"));
        if( type == PtngEnums::AXFR_DNS_RECON
                || type == PtngEnums::AXFR_NS_WIN
                || type == PtngEnums::AXFR_NS_LIN
                || type == PtngEnums::NBTSCAN
                || type == PtngEnums::ARPSCAN
                || type == PtngEnums::AXFR_HOST
                ){
            zoneTransferFile = parser.value("axfr");
            qInfo() << "[info] Optional argument axfr set to:" << zoneTransferFile << "which is of type" << type;
        }
        else{
            qInfo() << "[info] Supplied file" << parser.value("axfr") << "is not supported:" << type;
        }
    }
    else{
        qInfo() << "[info] Optional argument axfr not set. Moving on.";
    }
    if( parser.isSet("sev")  ){
        type = ident->checkFile(parser.value("sev"));
        if( type == PtngEnums::NESSUS_HSH){
            hshFile = parser.value("sev");
            qInfo() << "[info] Optional argument sev set to:" << hshFile << "which is of type" << type;
        }
        else{
            qInfo() << "[info] Supplied file" << parser.value("sev") << "is not supported:" << type;
        }
    }
    else{
        qInfo() << "[info] Optional argument sev not set. Moving on.";
    }
    if( parser.isSet("api")  ){
        ipLocationAPIKey= parser.value("api");
        qInfo() << "[info] Optional argument api set to:" <<ipLocationAPIKey;
    }
    else{
        qInfo() << "[info] Optional argument api not set. Moving on.";
    }
    if( parser.isSet("net")  ){
        subnetFilter= parser.value("net");
        qInfo() << "[info] Optional argument net set to:" <<subnetFilter;
    }
    else{
        qInfo() << "[info] Optional argument net not set. Moving on.";
    }
    if( parser.isSet("output")  ){
        outputFileStem= parser.value("output");
        qInfo() << "[info] Optional argument output set to:" <<outputFileStem;
    }
    else{
        qInfo() << "[info] Optional argument output not set. Moving on.";
    }
    if( parser.isSet("font")  ){
        fontFamily= parser.value("font");
        qInfo() << "[info] Optional argument font set to:" <<fontFamily;
    }
    else{
        qInfo() << "[info] Optional argument font not set. Moving on.";
    }
}

