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

#include "GlobalIncludes.hpp"
#include "CommandLineHandler.hpp"
#include "PtngDGMLBuilder.hpp"
#include "PtngIdent.hpp"

// Miscellaneous - everything has got to live somewhere it can stay here for now
void showTypes(){
    qInfo() << "[info] Supported file types:";
    qInfo() << "[info] For IP addresses: nmap xml";
    qInfo() << "[info] For Zone Transfers: dnsrecon xml";
    qInfo() << "[info] For vulnerabilities: nessus xml";
}

// Parser functions
QList<PtngHostBuilder*> addSeverities(const QList<PtngHostBuilder*> &builders, const QString &hshFile);
QList<PtngHostBuilder*> addAxfrEntry(const QList<PtngHostBuilder*> &builders, const QString &axfrFile);
void addPorts(PtngHostBuilder* builder, const QDomNode &node);

QList<PtngHostBuilder*> parseInputFiles( QString networkMapSource,
                                         QString zoneTransferFile,
                                         QString hshFile,
                                         QString ipLocationAPIKey,
                                         QString subnetFilter,
                                         QString outputFileStem,
                                         QString fontFamily)
{
    qInfo() << "[info] Starting nmap parse on"<<networkMapSource;
     QList<PtngHostBuilder*> builderList;
    PtngInputParser ip;
    builderList = ip.parseNmap(networkMapSource);
    qInfo() << "[info] New builder list size:"<<builderList.count();

    if( hshFile != ""  ){
        builderList = addSeverities(builderList, hshFile);
    }
    if( zoneTransferFile != "" ){
        builderList = addAxfrEntry(builderList,zoneTransferFile);
    }
    qInfo() << "[info] Host builder list:"<<builderList.length();
    return(builderList);
}

QList<PtngHostBuilder*> addSeverities(const QList<PtngHostBuilder*> &builders, const QString &hshFile){
    // qInfo() << "[info] Adding severities";
    QList<PtngHostBuilder*> bl = builders;
    QScopedPointer<QFile> file(new QFile(hshFile));
    if( !file->open(QIODevice::ReadOnly) ){
        qWarning() << "[warning] Cannot open"<< hshFile << "for reading.";
        return(bl);
    }
    QStringList lines;
    QTextStream stream(file.data());
    while( !stream.atEnd() ){
        lines.append(stream.readLine());
    }
    QMap<QString,QString> nv;
    for( auto line : lines ){
        if( line.toLower() == "[nessus_hsh]"){
            continue;
        }
        QStringList entry = line.split("=", Qt::SkipEmptyParts);
        // qInfo() << "[info] Entries:"<<entry.length();
        if( entry.length() != 2 ){
            continue;
        }
        nv.insert(entry.at(0), entry.at(1));
    }
    // qInfo() << "[info] Pairs:"<<nv.count();
    for( auto builder : bl ){
        PtngHost *host = builder->getHost();
        if( nv.contains( host->getIpAddress() ) ){
            QString sev = nv.value( host->getIpAddress() );
            if( sev.toLower() == "critical" ){
                builder->setSeverity( PtngEnums::CRITICAL );
            }
            else if( sev.toLower() == "high" ){
                builder->setSeverity( PtngEnums::HIGH );
            }
            else if( sev.toLower() == "medium" ){
                builder->setSeverity( PtngEnums::MEDUIM );
            }
            else if( sev.toLower() == "low" ){
                builder->setSeverity( PtngEnums::LOW );
            }
            else if( sev.toLower() == "info" ){
                builder->setSeverity( PtngEnums::NONE );
            }
            else{
                builder->setSeverity( PtngEnums::NUM_ISSUE_SEVERITIES );
            }
        }
        // qInfo() << "[info] We have a winner:"<<builder->getHost()->getIpAddress() << "Severity:"<<builder->getHost()->getHighestSeverity();
    }
    file->close();
    return(bl);
}

QList<PtngHostBuilder*> addAxfrEntry(const QList<PtngHostBuilder*> &builders, const QString &axfrFile){
    qInfo() << "[info] Starting AXFR parse on"<<axfrFile;
    QList<PtngHostBuilder*> bl = builders;
    QScopedPointer<QDomDocument> doc(new QDomDocument("mydocument"));
    QScopedPointer<QFile> file(new QFile(axfrFile));
    PtngIdent ident;
    if( !file->open(QIODevice::ReadOnly)){
        qWarning() << "[warning] Unable to open AXFR file:"<<axfrFile;
        return(bl);
    }

    QMap<QString,QString> addresses = PtngInputParser::parseZoneTransfer(axfrFile);
    qInfo() << "[info] Number of addresses from AXFR:"<<addresses.count();

    return(bl);
}
