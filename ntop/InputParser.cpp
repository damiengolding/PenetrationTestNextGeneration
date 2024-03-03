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
    QScopedPointer<QDomDocument> doc(new QDomDocument("mydocument"));
    QScopedPointer<QFile> file(new QFile(networkMapSource));

    if( !file->open(QIODevice::ReadOnly)){
        qWarning() << "[warning] Failed opening file"<<networkMapSource;
        return(builderList);
    }

    if( !doc->setContent(file.data()) ){
        qWarning() << "[warning] Failed parsing"<<networkMapSource;
        file->close();
        return(builderList);
    }
    QDomNodeList nodes = doc->elementsByTagName("host");
    qInfo() << "[info] Number of host nodes:"<<nodes.length();
    for( int i = 0; i != nodes.length(); ++i ){
        QDomNode node = nodes.at(i);
        PtngHostBuilder *hb = new PtngHostBuilder();
        hb->addNmapScanXmlNode(node);
        addPorts(hb,node);
        builderList.append(hb);
    }

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
    QMultiMap<QString,QString> nv;
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
                builder->setSeverity( PtngEnums::INFO );
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

void addPorts(PtngHostBuilder* builder, const QDomNode &node){
    // qInfo() << "[info] addPorts";
    PtngHost *host = builder->getHost();
    QDomElement elem = node.toElement();
    if( elem.isNull() ){
        return;
    }
    QDomNodeList portList = elem.elementsByTagName("ports");
    if( portList.length() == 0 ){
        // qInfo() << "[info] No open ports found on"<<host->getIpAddress();
        return;
    }
    else{
        // qInfo() << "[info] Found"<< portList.length() <<"ports open on"<<host->getIpAddress();
        QDomElement portListElem = portList.at(0).toElement();
        if( portListElem.isNull() ){
            return;
        }

        QDomNodeList ports = portListElem.elementsByTagName("port");
        // qInfo() << "[info] Found"<< ports.length() <<"ports open on"<<host->getIpAddress();

        for( int i = 0;i< ports.length();++i){
            QDomNode node = ports.at(i);
            QDomElement e = node.toElement();
            PtngPort port;
            if( e.isNull() ){
                continue;
            }

            port.portNumber = e.attribute("portid").toInt();
            QString prot = e.attribute("protocol");
            if( prot.toLower() == "tcp" ){
                port.protocol = PtngEnums::TCP;
            }
            else if( prot.toLower() == "udp" ){
                port.protocol = PtngEnums::UDP;
            }
            QDomElement state = e.firstChildElement("state");
            if( !state.isNull()){
                port.state = state.attribute("state");
                port.reason = state.attribute("reason");
            }
            QDomElement service = e.firstChildElement("service");
            if( !service.isNull()){
                port.serviceName = service.attribute("name");
                port.serviceProduct = service.attribute("product");
                port.serviceVersion = service.attribute("version");
                port.idMethod =  service.attribute("method");
                QDomNodeList cpe = service.elementsByTagName("cpe");
                if( cpe.length() > 0 ){
                    for(int i =0;i<cpe.length();++i){
                        QDomElement entry = cpe.at(i).toElement();
                        if( !entry.isNull()){
                            port.serviceCPE.append( entry.text() );
                        }
                    }
                }
                QDomNodeList scripts = e.elementsByTagName("script");
                for( int i =0;i<scripts.length();++i ){
                    QDomElement e = scripts.at(i).toElement();
                    if( e.isNull()){
                        continue;
                    }
                    QString id = e.attribute("id");
                    QString output = e.attribute("output");
                    port.portScripts.insert(id,output);
                }
            }
            builder->addPortSpec(port);
        }
    }
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
    PtngAddressParser parser;
    QMultiMap<QString,QString> addresses = parser.parseAddresses(axfrFile);
    qInfo() << "[info] InputParser - number of addresses from AXFR:"<<addresses.count();

    return(bl);
}
