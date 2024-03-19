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
QString nessusFile="";
QString zoneFile="";
QString subnetFilters="";
QString criticalityFilters="";
bool showLabels=false;

// void processAxfrFile(const QString& inputFile,const QString& outputFile,const QString& zoneFile, const QString &nessusFile, const QString &subnets,bool labels);
// void processNmapFile(const QString& inputFile,const QString& outputFile,const QString& zoneFile, const QString &nessusFile, const QString &subnets,bool labels);
// void processNessusFile(const QString& inputFile,const QString& outputFile, const QString &subnets, const QString &criticalityFilters, bool labels);

void showTypes();
bool fileIsSupported(const QString &inputFile);

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
    parser.setApplicationDescription("Creates a basic xml output based on Microsoft DGML\n\n"
                                     "Caveat: the DGML output is viewable and editable in Microsoft Visual Studio, but most edits render the DGML unusable by PTNG, so if you wish to make changes for display and reporting make two output files and keep one separate for VS usage.\n\n"
                                     "N.B. This is VERY specific to pen testing; for instance, the core DGML created at instantiation includes categories for colouring critical, high, medium, low and info issues according to a nessus input file"
                                     );
    // Init options from here, defined separately in initArgumentOptions
    initArgumentOptions(app,parser);
    // Process the supplied arguments into the parser
    parser.process(app);
    // Process the arguments supplied to the application so we can use them
    processArgumentOptions(app, parser);
}

void initArgumentOptions(QCoreApplication &app, QCommandLineParser &parser){
    parser.addPositionalArgument("show-types","Currently supported input file types");
    parser.addOption({{"f","file"},"Input file. Use \'ndgml types\' to list supported types.","file"});
    parser.addOption({{"o","output"},"[optional] Write to file","file"});
    parser.addOption({{"l","labels"},"[optional] Add labels to links; this is necessary for conversion to dot runcontrol (default is '\on\')","on|off"});
    parser.addOption({{"i","issues"},"[optional] Output from a vulneablilty scanner contining a list of issues; used for highest severity for each host. Currently nessus only","file"});
    parser.addOption({{"z","zone"},"[optional] Output from a zone transfer file.  Use \'ndgml types\' to list supported types.","file"});
    parser.addOption({{"s","subnets"},"[optional] A comma delimited list of subnet filters in the form 192.168.1.,192.168.48. (use the trailing \'.\' to avoid including 192.168.10. and 192.168.100. etc)","subnets"});
    parser.addOption({{"c","crits"},"[optional] A comma delimited list of criticality filters in the form \"critical,high,medium,low,none,all\"","criticalities"});
}

void processArgumentOptions(QCoreApplication &app, QCommandLineParser &parser){
    /*
       Options processing
      */
    PtngEnums::SupportedInputTypes type;
    // Positional arguments
    for(QString pos : parser.positionalArguments()){
        if( pos.toLower() == "show-types"){
            showTypes();
        }
    }
    // Don't handle poitional arguments and options together, one or the other
    if( parser.positionalArguments().count() > 0 ){
        ret = 0;
        return;
    }
    qInfo() << "[info] Starting to process input files. This may take some time for large files.";
    if(!parser.isSet("file")){
        qWarning() << "[fatal] No source file specified";
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
            // type = PtngIdent::checkFile(inputFile);
            if( !fileIsSupported(inputFile)){
                qInfo() << "[info] Source file" << inputFile << "is not supported by ndgml. Use ndgml types for a list.";
                qInfo() << "[info] Supplied file is of type:" << type;
                ret = 4;
                return;
            }
            else{
                type = PtngIdent::checkFile(inputFile);
                qInfo() << "[info] Input file set to" << inputFile << "which is of type:" << type;
            }
        }
    }

    // Issues file
    if( parser.isSet("issues") ){
        nessusFile = parser.value("issues");
        type = PtngIdent::checkFile(nessusFile);
        if( type != PtngEnums::NESSUS ){
            qInfo() << "[info] Issues file" << nessusFile << "is not supported by ndgml. Use ndgml types for a list.";
            qInfo() << "[info] Supplied file is of type:" << type;
        }
        else{
            qInfo() << "[info] Issues file set to" << nessusFile << "which is of type:" << type;}
    }

    // Zone file
    if( parser.isSet("zone") ){
        zoneFile = parser.value("zone");
        type = PtngIdent::checkFile(zoneFile);
        if( type == PtngEnums::ARPSCAN
                || type == PtngEnums::NBTSCAN
                || type == PtngEnums::AXFR_DIG
                || type == PtngEnums::AXFR_DNS_RECON
                || type == PtngEnums::AXFR_HOST
                || type == PtngEnums::AXFR_NS_LIN
                || type == PtngEnums::AXFR_NS_WIN
                || type == PtngEnums::AXFR_NMAP
                ){
            qInfo() << "[info] Issues file set to" << nessusFile << "which is of type:" << type;
        }
        else{
            qInfo() << "[info] Issues file" << zoneFile << "is not supported by ndgml. Use ndgml types for a list.";
            qInfo() << "[info] Supplied file is of type:" << type;
        }
    }

    // Output file
    if( parser.isSet("output") ){
        outputFile = parser.value("output");
        qInfo() << "[info] Output file set to" << inputFile;
    }

    // Show labels
    if( parser.isSet("labels") ){
        if( parser.value("labels").toLower() == "on"){
            showLabels = true;
            qInfo() << "[info] Show labels set to: True";
        }
        else if(parser.value("labels").toLower() == "off"){
            showLabels = false;
            qInfo() << "[info] Show labels set to: False";
        }
    }
    else{
        showLabels = false;
        qInfo() << "[info] Show labels set to: False";
    }

    // Subnet filters
    if(  parser.isSet("subnets")){
        subnetFilters = parser.value("subnets");
    }

    // Criticality filters
    if(  parser.isSet("crits")){
        criticalityFilters = parser.value("crits");
    }

/*
   Processing
*/

    qInfo() << "[info] Starting to process file:"<<inputFile;
    type = PtngIdent::checkFile(inputFile);
    QString dgml;
    PtngDGMLBuilder builder;
    if( type == PtngEnums::NMAP ){
        // qInfo() << "[info] NMAP";
        QList<PtngHostBuilder*> hostBuilders = PtngInputParser::parseNmap(inputFile);
        builder.createFromNmap(hostBuilders,nessusFile,zoneFile,subnetFilters,showLabels);
        dgml = builder.toString(4);
    }
    else if( type == PtngEnums::NESSUS ){
        // qInfo() << "[info] NESSUS";
        QList<PtngHostBuilder*> hostBuilders = PtngInputParser::parseNessus(inputFile);
        builder.createFromNessus(hostBuilders,inputFile,criticalityFilters,subnetFilters);
        dgml = builder.toString(4);
    }
    else{
        QMap<QString,QString> hosts = PtngInputParser::parseZoneTransfer(inputFile);
        // qInfo() << "[info] Number of hosts (simple):"<<hosts.count();
        builder.createSimple(hosts, subnetFilters, showLabels);
        dgml = builder.toString(4);
    }

    if( !outputFile.isEmpty() ){
        QFile file(outputFile);
        if( !file.open(QIODevice::WriteOnly) ){
            qInfo() << "[info] Could not open"<<outputFile<<"for writing";
            return;
        }
        QTextStream outStream(&file);
        outStream << dgml;
        file.close();
        qInfo() << "[info] Completed processing file:"<<inputFile;
    }

    if( outputFile.isEmpty() ){
        qInfo() << "[info] DGML:"<<dgml;
    }

}

