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

#include "inc\PtngInputParser.hpp"
namespace ptng {

QList<PtngHostBuilder*> PtngInputParser::parseNmap(const QString &inputFile){
    QList<PtngHostBuilder*> builderList;
    PtngEnums::SupportedInputTypes type = PtngIdent::checkFile(inputFile);
    if( type != PtngEnums::NMAP ){
        qWarning() << "[warning] Input file"<<inputFile<<"is of incorrect type"<<type;
        return(builderList);
    }
    QScopedPointer<QDomDocument> doc(new QDomDocument("mydocument"));
    QScopedPointer<QFile> file(new QFile(inputFile));
    if( !file->open(QIODevice::ReadOnly)){
        qWarning() << "[warning] Failed opening file"<<inputFile;
        return(builderList);
    }

    if( !doc->setContent(file.data()) ){
        qWarning() << "[warning] Failed parsing"<<inputFile;
        file->close();
        return(builderList);
    }
    QDomNodeList nodes = doc->elementsByTagName("host");
    // qInfo() << "[info] Number of host nodes:"<<nodes.length();
    for( int i = 0; i != nodes.length(); ++i ){
        // qInfo() << "[info] Node number:"<<i;
        QDomNode node = nodes.at(i);
        PtngHostBuilder *hb = new PtngHostBuilder();
        hb->addNmapScanXmlNode(node);
        addPorts(hb,node);
        builderList.append(hb);
    }
    return(builderList);
}

void PtngInputParser::addPorts(PtngHostBuilder* builder, const QDomNode &node){
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

} // namespace ptng
