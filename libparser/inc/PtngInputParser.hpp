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
#include "libparser_local.hpp"
#include <QObject>

#include "PtngEnums.hpp"
#include "PtngIdent.hpp"
#include "PtngHostBuilder.hpp"

namespace ptng {
/*!
   \brief The PtngInputParser class

   Parses IP addresses, and if available DNS names from a variety of input types:

   \see PtngIdent, PtngEnums
 */
class LIBPARSER_EXPORT PtngInputParser : public QObject
{
    Q_OBJECT
public:
    explicit PtngInputParser(QObject *parent = nullptr);

    // AXFR
    /*!
       \brief parseZoneTransfer
       \param inputFile
       \return QMap<QString,QString>
     */
    static QMap<QString,QString> parseZoneTransfer(const QString &inputFile);
    // Nmap
    /*!
       \brief parseNmap
       \param inputFile
       \return
     */
    static QList<PtngHostBuilder*> parseNmap(const QString &inputFile);
    // Nessus
    /*!
       \brief parseNmap
       \param inputFile
       \return QList<PtngHostBuilder*>
     */
    static QList<PtngHostBuilder*> parseNessus(const QString &inputFile);
    /*!
       \brief parseNesusIssues
       \param inputFile
       \return QList<PtngIssue*>
     */
    static QList<PtngIssue> parseNesusIssues(const QString &inputFile);
    /*!
       \brief parseNesusSeverities
       \param inputFile
       \return QMap<QString,QString>
     */
    static QMap<QString,QString> parseNesusSeverities(const QString &inputFile);

private:
    static QMap<QString,QString> parseAxfrDnsRecon(const QString &inputFile);
    static QMap<QString,QString> parseAxfrNmap(const QString &inputFile);
    static QMap<QString,QString> parseAxfrNslookupWin(const QString &inputFile);
    static QMap<QString,QString> parseAxfrNslookupLin(const QString &inputFile);
    static QMap<QString,QString> parseAxfrArpscan(const QString &inputFile);
    static QMap<QString,QString> parseAxfrNbtscan(const QString &inputFile);
    static QMap<QString,QString> parseAxfrHostScan(const QString &inputFile);
    static void addPorts(PtngHostBuilder* builder, const QDomNode &node);

#ifdef QT_DEBUG
private slots: // for QTest module
    void nmapTest_data();
    void nmapTest();
    void nessusTest_data();
    void nessusTest();
    void nessusIssuesTest_data();
    void nessusIssuesTest();
    void nessusSeveritiesTest_data();
    void nessusSeveritiesTest();
#endif
signals:

};

} // namespace ptng

