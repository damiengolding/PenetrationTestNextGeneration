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

#include <QObject>
#include "libparser_global.hpp"
#include "libparser_local.hpp"

namespace ptng {
class LIBPARSER_EXPORT PtngEnums : public QObject
{
    Q_OBJECT
    /*!
       \brief PtngEnums
       \param parent

       Contains various enums for PTNG, including supported file types, issue severity ratings and the Nessus plugin families.
     */
    PtngEnums(QObject *parent = nullptr){}

public:
    /*!
       \brief The SupportedInputTypes enum
     */
    enum SupportedInputTypes {
        ARPSCAN,
        AXFR_NMAP,
        AXFR_DIG,
        AXFR_DNS_RECON,
        AXFR_FIERCE,
        AXFR_HOST,
        AXFR_NS_LIN,
        AXFR_NS_WIN,
        DGML,
        HASHES,
        MASSCAN,
        METASPLOIT,
        NBTSCAN,
        NESSUS,
        NESSUS_HSH,
        NIKTO,
        NMAP,
        OPENVAS,
        OWASP_ZAP,
        SSL_SCAN,
        WAPITI,
        NUM_SUPPORTED_INPUT_TYPES
    };
    Q_ENUM(SupportedInputTypes)

    /*!
       \brief The NetworkNodeRoles enum
     */
    enum NetworkNodeRoles {
        ROOT,
        SUBNET,
        ACLASS,
        BCLASS,
        CCLASS,
        LEAF,
        NUM_NMAP_NODE_ROLES
    };
    Q_ENUM(NetworkNodeRoles)

    /*!
       \brief The ProcessorStates enum
     */
    enum ProcessorStates {
        START_STATE_CREATED,
        NUM_PROCESSOR_STATES
    };
    // Q_ENUM(ProcessorStates)

    /*!
       \brief The IssueSeverity enum
     */
    enum IssueSeverity {
        NONE,
        LOW,
        MEDIUM,
        HIGH,
        CRITICAL,
        NUM_ISSUE_SEVERITIES
    };
    Q_ENUM(IssueSeverity)

    enum Protocol{
        UDP,
        TCP,
        NUM_PROTOCOLS
    };
    Q_ENUM(Protocol)

    /*!
       \brief The NessusPluginFamilies enum
     */
    enum NessusPluginFamily {
        WINDOWS,
        LINUX,
        NETWORK_DEVICES,
        POLICY,
        DATABASE,
        WEB,
        SERVICES,
        MOBILE,
        P2P,
        SCADA,
        DOS,
        REMOTE_CONTROL,
        MISCELLANEOUS,
        NUM_NESSUS_PLUGIN_FAMILIES
    };
    Q_ENUM(NessusPluginFamily)
};

} // namespace ptng

