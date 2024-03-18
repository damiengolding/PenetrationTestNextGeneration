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

bool fileIsSupported(const QString &inputFile){
    if( PtngIdent::checkFile(inputFile) == PtngEnums::AXFR_NMAP
            ||PtngIdent::checkFile(inputFile) == PtngEnums::NMAP
            || PtngIdent::checkFile(inputFile) == PtngEnums::AXFR_DNS_RECON
            || PtngIdent::checkFile(inputFile) == PtngEnums::AXFR_NS_WIN
            ||PtngIdent::checkFile(inputFile) == PtngEnums::AXFR_NS_LIN
            ||PtngIdent::checkFile(inputFile) == PtngEnums::ARPSCAN
            ||PtngIdent::checkFile(inputFile) == PtngEnums::NBTSCAN
            ||PtngIdent::checkFile(inputFile) == PtngEnums::AXFR_HOST
            ||PtngIdent::checkFile(inputFile) == PtngEnums::NESSUS
            ){
        return(true);
    }
    return(false);
}

void showTypes(){
    qInfo() << "[info] Supported file types:";
    qInfo() << "[info] nmap - standard scan";
    qInfo() << "[info] nmap - with the dns-zone-transfer.nse script";
    qInfo() << "[info] nessus";
    qInfo() << "[info] dns recon - use \'dnsrecon -d <domain> -n <nameserver> -t axfr -x <outputfile>.xml\'";
    qInfo() << "[info] nslookup (windows) - use \'ls -d discworld.io > nslookup_win_axfr.txt\'";
    qInfo() << "[info] nslookup (linux)";
    qInfo() << "[info] arp-scan - use\'arp-scan <address_range> > <outputfile>.txt\'";
    qInfo() << "[info] nbtscan - use \'nbtscan -s : <address_range> <outputfile>.txt\' (the \'s : '\ is required as it is the separator used on each entry)";
    qInfo() << "[info] host - use \'host -t axfr <domain> <nameserver> > host_axfr.txt\'";
}


