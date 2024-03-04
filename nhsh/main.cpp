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
#include <QtCore/qglobal.h>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomElement>
#include <QDomText>
#include <QDomAttr>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QMap>
#include <QList>
#include <QDebug>
#include <QScopedPointer>
#include <QDateTime>

#include "PtngIdent.hpp"
#include "PtngSpecifications.hpp"
using namespace ptng;

void initArgumentParser(QCoreApplication &app, QCommandLineParser &parser);
void initArgumentOptions(QCoreApplication &app, QCommandLineParser &parser);
void processArgumentOptions(QCoreApplication &app, QCommandLineParser &parser);
void processNessusFile();

QList<PtngIssue> issueList;
QList<QCommandLineOption> commandLineOptions;
QString nessusFile="";
QString outputFile="";
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
    app.setApplicationName("nhsh");
    app.setApplicationVersion("0.0.1");
    app.setOrganizationName("Golding's Gym");
    app.setOrganizationDomain("");
    // Convenience options
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription("Converts a nessus xml file to a textual representation of highest severity\n"
                                     "vulnerabilities per host\n"
                                     "\n"
                                     "Output is used in conjunction with ntop.exe");
    // Init options from here, defined separately in initArgumentOptions
    initArgumentOptions(app,parser);
    // Process the supplied arguments into the parser
    parser.process(app);
    // Process the arguments supplied to the application so we can use them
    processArgumentOptions(app, parser);
}

void initArgumentOptions(QCoreApplication &app, QCommandLineParser &parser){
    parser.addOption({{"f","file"},"Nessus file to process","file"});
    parser.addOption({{"o","output"},"Output file name. If missing, the nessus file name prefixed with date/time stamp will be used","file"});
}

void processArgumentOptions(QCoreApplication &app, QCommandLineParser &parser){
    PtngEnums::SupportedInputTypes type;
    if( parser.isSet("file") ){
        nessusFile = parser.value("file");
        PtngIdent::checkFile(nessusFile);
        if( type != PtngEnums::NESSUS ){
            qInfo() << "[info] File" << nessusFile << "is not a nessus xml report file.";
            qInfo() << "[info] Supplied file is of type:" << type;
            parser.showHelp();
            ret = 1;
            return;
        }
    }
    else{
        qWarning() << "[fatal] No nessus file specified";
        parser.showHelp();
        ret = 1;
        return;
    }
    if( parser.isSet("output") ){
        outputFile = parser.value("output");
    }
    else{
        QString outputFileName = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
        outputFileName.append("_");
        QFileInfo info(nessusFile);
        outputFileName.append(info.baseName());
        outputFileName.append(".hsh");
        outputFile = outputFileName;
        // qInfo() << "[info] Candidate output file name:"<<outputFile;
    }
    processNessusFile();
}

void processNessusFile(){
    qInfo() << "[info] Starting to process nessus file:"<<nessusFile;
    QStringList ipAddresses;
    QScopedPointer<QDomDocument> doc(new QDomDocument("input"));
    QScopedPointer<QFile> f(new QFile(nessusFile));
    f->open(QIODevice::ReadOnly);
    doc->setContent(f.data());
    QDomElement rootElem = doc->documentElement();
    QDomNodeList hosts = rootElem.elementsByTagName("ReportHost");
    // qInfo() << "[info] Number of ReportHosts:"<<hosts.count();
    QString ipAddress;
    for( int i = 0;i<hosts.count();++i ){
        QDomNode host = hosts.item(i);
        QDomElement elem = host.toElement();
        if( elem.isNull()){
            continue;
        }
        ipAddress = elem.attribute("name");
        ipAddresses.append(ipAddress);
        qInfo() << "[info] IP address:"<<ipAddress;
        QDomNodeList items = elem.elementsByTagName("ReportItem");
        // qInfo() << "[info] Number of ReportItems:"<<items.count();

        for( int i = 0;i<items.count();++i){
            QDomNode item = items.at(i);
            QDomElement elem = item.toElement();
            if( elem.isNull() ){
                continue;
            }
            PtngIssue issue;
            issue.ipAddress = ipAddress;
            issue.portNumber = elem.attribute("port").toInt();
            issue.serviceName = elem.attribute("svc_name");
            QString prot =  elem.attribute("protocol");
            if( prot.toLower() == "tcp"  ){
                issue.protocol = PtngEnums::TCP;
            }
            else if( prot.toLower() == "udp" ){
                issue.protocol = PtngEnums::UDP;
            }
            int sev  = elem.attribute("severity").toInt();
            switch(sev){
            case 0:{
                issue.severity = PtngEnums::NONE;
                break;
            }
            case 1:{
                issue.severity = PtngEnums::LOW;
                break;
            }
            case 2:{
                issue.severity = PtngEnums::MEDUIM;
                break;
            }
            case 3:{
                issue.severity = PtngEnums::HIGH;
                break;
            }
            case 4:{
                issue.severity = PtngEnums::CRITICAL;
                break;
            }
            default:{
                issue.severity = PtngEnums::NUM_ISSUE_SEVERITIES;
                break;
            }
            }

            issue.pluginId = elem.attribute("pluginID");
            issue.pluginName = elem.attribute("pluginName");
            issue.pluginFamily = elem.attribute("pluginFamily");
            issue.pluginType = elem.firstChildElement("plugin_type").text();
            issue.display = elem.firstChildElement("plugin_name").text();
            issue.description = elem.firstChildElement("description").text();
            issue.synopsis = elem.firstChildElement("synopsis").text();
            issue.solution = elem.firstChildElement("solution").text();
            issue.seeAlso = elem.firstChildElement("xref").text();
            issue.riskFactor = elem.firstChildElement("risk_factor").text();
            // qInfo() << "[info] Risk factor:"<<elem.firstChildElement("risk_factor").text();
            // = elem.attribute("");
            //  = elem.firstChildElement("").text();
            issueList.append(issue);
        }
    }
    qInfo() << "[info] IP Addresses:"<<ipAddresses.count();
    int critical=0;
    int high=0;
    int medium=0;
    int low=0;
    int none=0;
    // QString ipAddress="";
    QString outputString="[NESSUS_HSH]\n";

    for( auto address : ipAddresses ){
        // QString address = ipAddresses.at(i);
        // qInfo() << "[info] address from ipAddresses list:"<<address;
        QString entry=address;
        // qInfo() << "[info] entry:"<<entry;
        for( auto issue : issueList){
            // PtngIssue issue = issueList.at(i);
            if( address == issue.ipAddress ){
                QString risk_factor = issue.riskFactor.toLower();
                if( risk_factor == "critical" )
                    critical++;
                else if( risk_factor == "high" )
                    high++;
                else if( risk_factor == "medium" )
                    medium++;
                else if( risk_factor == "low" )
                    low++;
                else if( risk_factor == "none" )
                    none++;
            }
        }
        outputString.append(address + "=");
        QString sevStr;
        if(none>0){
            sevStr="NONE\n";
        }
        if(low>0){
            sevStr="LOW\n";
        }
        if(medium>0){
            sevStr="MEDIUM\n";
        }
        if(high>0){
            sevStr="HIGH\n";
        }
        if(critical>0){
            sevStr="CRITICAL\n";
        }
        outputString.append(sevStr);
        critical=0;
        high=0;
        medium=0;
        low=0;
        none=0;
    }
    QFile writeFile(outputFile);
    if( !writeFile.open(QIODevice::WriteOnly)){
        qWarning() << "[warning] Could not open file"<<outputFile<<"for writing.";
    }
    else{
        QTextStream out(&writeFile);
        out << outputString;
    }
}

