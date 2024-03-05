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
#pragma once

#include "libparser_global.hpp"
#include "PtngSpecifications.hpp"

#include <QObject>
#include <QList>
#include <QMap>
namespace ptng {
/*!
   \brief The PtngHost class

   Encapsulates details for a machine on the network, includes routers and other network management devices

   \see PtngHostBuilder
 */
class LIBPARSER_EXPORT PtngHost : public QObject
{
    Q_OBJECT
    friend class PtngHostBuilder;
protected:
    explicit PtngHost(QObject *parent = nullptr);

public: // Accessors and mutators for Q_PROPERTY
    bool isGateway(){return(gateway);}
    bool hasGeoLocation(){ return(geoLocation); }
    bool isInAxfr(){ return(inAxfr);  }
    PtngEnums::IssueSeverity getHighestSeverity(){ return( highestSeverity); }
    QList<PtngPort> &getPortSpecs(){ return(portSpecs); }
    QMultiMap<QString,QString> &getHostScripts(){ return(hostScripts); }
    QString &getIpAddress(){ return( ipAddress ); }
    QString &getDnsName(){ return( dnsName ); }
    QString &getLongitude(){ return( longitude ); }
    QString &getLatitude(){ return( latitude ); }
    QString &getCity(){ return( city ); }
    QString &getAddrType(){ return( addrType); }
    QString &getMacAddress(){  return( macAddress ); }
    QString &getMacVendor(){ return(macVendor); }
    QString &getRecordType(){ return( recordType ); }
    QString &getHostName(){ return( hostName ); }
    QString &getOsName(){ return( osName ); }
    QString &getHostState(){ return( hostState); }
    QString &getHostStateReason(){ return( hostStateReason); }
    QStringList getHostCPE(){ return(hostCPE); }
    int getDistance(){ return( distance); }
public: // Q_PROPERTY declarations
    Q_PROPERTY(bool gateway READ isGateway)
    Q_PROPERTY(bool geoLocation READ hasGeoLocation)
    Q_PROPERTY(bool inAxfr READ isInAxfr)
    Q_PROPERTY(PtngEnums::IssueSeverity highestSeverity READ getHighestSeverity)
    Q_PROPERTY(QList<PtngPort> portSpecs READ getPortSpecs)
    Q_PROPERTY(QMultiMap<QString,QString> hostScripts READ getHostScripts)
    Q_PROPERTY(QString ipAddress READ getIpAddress)
    Q_PROPERTY(QString dnsName READ getDnsName)
    Q_PROPERTY(QString longitude READ getLongitude)
    Q_PROPERTY(QString latitude READ getLatitude)
    Q_PROPERTY(QString city READ getCity)
    Q_PROPERTY(QString addrType READ getAddrType)
    Q_PROPERTY(QString macAddress READ getMacAddress)
    Q_PROPERTY(QString macVendor READ getMacVendor)
    Q_PROPERTY(QString recordType READ getRecordType)
    Q_PROPERTY(QString osName READ getOsName)
    Q_PROPERTY(QString hostState READ getHostState)
    Q_PROPERTY(QString hostStateReason READ getHostStateReason)
    Q_PROPERTY(int distance READ getDistance)
    Q_PROPERTY(QStringList hostCPE READ getHostCPE)
protected: //Members
    bool gateway = false;
    bool geoLocation = false;
    bool inAxfr = false;
    PtngEnums::IssueSeverity highestSeverity = PtngEnums::NUM_ISSUE_SEVERITIES;
    QList<PtngPort> portSpecs;
    QMultiMap<QString,QString> hostScripts;
    QString ipAddress;
    QString dnsName;
    QString longitude = "";
    QString latitude = "";
    QString city = "";
    QString addrType = "";
    QString macAddress = "";
    QString macVendor = "";
    QString recordType = "";
    QString hostName = "";
    QString osName = "";
    QString hostState = "";
    QString hostStateReason = "";
    QStringList hostCPE;
    int distance = 0;
};

/*!
   \brief The PtngHostBuilder class

   Use this class to build and manage a PtngHost

   \see PtngHost
 */
class LIBPARSER_EXPORT PtngHostBuilder : public QObject
{
    Q_OBJECT
public:
    explicit PtngHostBuilder(QObject *parent = nullptr);

public: // Builder methods
    PtngHost* getHost(){ return(host); }
    /*!
       \brief createSimple
       \param ipAddress
       \param dnsName
       \return PtngHostBuilder&
     */
    PtngHostBuilder &createSimple(const QString &ipAddress,const QString &dnsName);
    /*!
       \brief addNmapScanXmlNode
       \param node
       \return PtngHostBuilder&
     */
    PtngHostBuilder& addNmapScanXmlNode(const QDomNode &node);
    /*!
       \brief addNmapAXFRXmlNode
       \param node
       \return PtngHostBuilder&
     */
    PtngHostBuilder& addNmapAXFRXmlNode(const QDomNode &node);
    /*!
       \brief setSeverity
       \param sev
       \return PtngHostBuilder&
     */
    PtngHostBuilder& setSeverity( PtngEnums::IssueSeverity sev );
    /*!
       \brief setIsAXFR
       \param isAxfr
       \return PtngHostBuilder&
     */
    PtngHostBuilder& setIsAXFR(bool isAxfr );
    /*!
       \brief addPortSpec
       \param portSpec
       \return PtngHostBuilder&
     */
    PtngHostBuilder& addPortSpec(const PtngPort &portSpec );
    /*!
       \brief addScript
       \param id
       \param output
       \return PtngHostBuilder&
     */
    PtngHostBuilder& addScript( const QString &id, const QString &output );

    PtngHostBuilder& setHighestSeverity( PtngEnums::IssueSeverity severity );
    PtngHostBuilder& setIsInZoneFile( bool axfr );

private:
    PtngHost *host;

signals:

};



} // namespace ptng

