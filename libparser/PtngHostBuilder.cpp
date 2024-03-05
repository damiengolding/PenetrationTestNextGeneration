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
#include "inc\PtngHostBuilder.hpp"

namespace ptng {
/*
    PtngHost
*/

PtngHost::PtngHost(QObject *parent)
    : QObject{parent}
{}

/*
    PtngHostBuilder
*/

PtngHostBuilder::PtngHostBuilder(QObject *parent)
    : QObject{parent}
{
    host = new PtngHost();
}

PtngHostBuilder &PtngHostBuilder::createSimple(const QString &ipAddress, const QString &dnsName)
{
    host->ipAddress = ipAddress;
    host->dnsName = dnsName;
    return(*this);
}

PtngHostBuilder& PtngHostBuilder::setSeverity( PtngEnums::IssueSeverity sev )
{
    host->highestSeverity = sev;
    return(*this);
}

PtngHostBuilder& PtngHostBuilder::setIsAXFR(bool isAxfr ){
    host->inAxfr = isAxfr;
    return(*this);
}

PtngHostBuilder& PtngHostBuilder::addScript( const QString &id, const QString &output ){
    host->hostScripts.insert(id,output);
    return(*this);
}

PtngHostBuilder &PtngHostBuilder::setHighestSeverity(PtngEnums::IssueSeverity severity)
{
    host->highestSeverity = severity;
    return(*this);
}

PtngHostBuilder &PtngHostBuilder::setIsInZoneFile(bool axfr)
{
    host->gateway = axfr;
    return(*this);
}

PtngHostBuilder& PtngHostBuilder::addPortSpec(const PtngPort &portSpec ){
    host->portSpecs.append(portSpec);
    return(*this);
}

PtngHostBuilder &PtngHostBuilder::addNmapAXFRXmlNode(const QDomNode &node)
{

    return(*this);
}

PtngHostBuilder &PtngHostBuilder::addNmapScanXmlNode(const QDomNode &node)
{
    QDomElement e = node.toElement();
    if( e.isNull() ){
        qWarning() << "[warning] Cannot convert the supplied QDomNode to a QDomElement";
        return(*this);
    }
    QDomNodeList nodes = e.childNodes();

    for( int i = 0;i<nodes.count();++i){
        QDomNode node = nodes.item(i);
        QDomElement elem = node.toElement();
        if( elem.isNull()){
            continue;
        }

        if( elem.tagName() == "address" ){
            QString t = elem.attribute("addrtype");
            if( t.toLower() == "ipv4" ){
                host->ipAddress = elem.attribute("addr");
                host->addrType = elem.attribute("addrtype");
            }
            else if( t.toLower() == "mac" ){
                host->macAddress = elem.attribute("addr");
                host->macVendor = elem.attribute("vendor");
            }

        }
        else if( elem.tagName() == "status" ){
            host->hostState = elem.attribute("state");
            host->hostStateReason = elem.attribute("reason");
        }
        else if( elem.tagName() == "hostnames" ){
             QDomElement hn = elem.firstChildElement("hostname");
            host->dnsName = hn.attribute("name");
            // qInfo() << "[info] Host name 2:"<<host->dnsName;
            // qInfo() << "[info] Host name 3:"<<host->getDnsName();
        }
        else if( elem.tagName() == "os" ){
            // qInfo() << "[info] in the os node for:"<<host->getIpAddress();
            QDomNodeList osmatch = elem.elementsByTagName("osmatch");
            // qInfo() << "[info] osmatch:"<<osmatch.length();
            if( osmatch.length() > 0){
                for( int i = 0;i<osmatch.count();++i ){
                    QDomNode match = osmatch.item(i);
                    QDomElement e = match.toElement();
                    host->osName = e.attribute("name");
                    if( !e.isNull()){
                        QDomNodeList osclasses = e.elementsByTagName("osclass");
                        // qInfo() << "[info] osclasses:"<<osclasses.length();
                        if( osclasses.isEmpty() ){
                            continue;
                        }
                        for(int i = 0;i<osclasses.count();++i){
                            QDomNode osclass = osclasses.item(i);
                            QDomElement e = osclass.toElement();
                            if( !e.isNull()){
                                QString cpe = e.firstChildElement().text();
                                QDomNodeList cpes = e.elementsByTagName("cpe");
                                for( int i = 0; i< cpes.count();++i ){
                                    QDomElement elem = cpes.at(i).toElement();
                                    host->hostCPE.append(elem.text() + "#");
                                }
                            }
                        }
                    }
                }
            }
        }
        else if( elem.tagName() == "distance" ){
            host->distance = elem.attribute("value").toInt();
        }
        else if( elem.tagName() == "hostscript" ){
            QDomNodeList scripts = elem.elementsByTagName("script");
            for( int i = 0;i<scripts.count();++i ){
                QDomNode script = scripts.item(i);
                QDomElement elem = script.toElement();
                if( elem.isNull() ){
                    continue;
                }
                QString id = elem.attribute("id");
                QString output = elem.attribute("output");
                host->hostScripts.insert( id,  output);

                if( id == "ip-geolocation-ipinfodb"  ){
                    qInfo() << "[info] ip-geolocation-ipinfodb";
                    host->geoLocation = true;
                    QStringList results = output.split("\n",Qt::SkipEmptyParts);
                    qInfo() << "[info] ip-geolocation-ipinfodb split:"<<results.length();
                    for(auto result : results ){
                        QString o = result.trimmed();
                      if( o.toLower().startsWith("coordinates") ){
                          QStringList coords = o.split(":");
                          QString ll = coords.at(1).trimmed();
                          QStringList lll = ll.split(",");
                          host->latitude = lll.at(0);
                          host->longitude = lll.at(1);
                          qInfo() << "[info] Latitude:"<<host->latitude<< "Longitude:"<< host->longitude;
                      }
                      else if( o.toLower().startsWith("city") ){
                           QStringList cy = o.split(":");
                           host->city = cy.at(1).trimmed();
                           qInfo() << "[info] City:"<<host->city;
                      }
                    }
                }
                if( id == "ip-forwarding"  ){
                    qInfo() << "[info] ip-forwarding";
                    host->gateway = true;
                }
            }
        }
    }

    return(*this);
}

} // namespace ptng
