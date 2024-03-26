/*
   Copyright (C) Damien Golding

   This is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this software; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA
*/

#include "pentestutils.h"

using namespace pentestutils;

ResultsChart::ResultsChart( QWidget *p ) : QCustomPlot(p){

}

QCustomPlot* ResultsChart::nessusPlugins(const QDomNode &n){
    QCustomPlot *p = new QCustomPlot();
    QCPBars *plg = new QCPBars(p->xAxis, p->yAxis);
    p->addPlottable( plg );
    QDomElement te = n.toElement();
    int sc,rem,db,lin,mob,dos,srv,win,web,p2p,nd,pol;
    sc = te.attribute( "plg_scada" ).toInt();
    rem = te.attribute( "plg_remote" ).toInt();
    db = te.attribute( "plg_db" ).toInt();
    lin = te.attribute( "plg_linux" ).toInt();
    mob = te.attribute( "plg_mobile" ).toInt();
    dos = te.attribute( "plg_dos" ).toInt();
    srv = te.attribute( "plg_services" ).toInt();
    win = te.attribute( "plg_windows" ).toInt();
    web = te.attribute( "plg_web" ).toInt();
    p2p = te.attribute( "plg_p2p" ).toInt();
    nd = te.attribute( "plg_netdevs" ).toInt();
    pol = te.attribute( "plg_policy" ).toInt();

    //Bar objects
    QVector<double> x,y;
    x <<1<<2<<3<<4<<5<<6<<7<<8<<9<<10<<11<<12;
    y << win << lin << nd << rem << web << db\
      << pol << srv << sc << mob << p2p << dos;

    QVector<QString> labels;
    labels <<"Win: \r\n"+QString::number(win)<<"Lin: \r\n"+QString::number(lin)<<"Net devs: \r\n"+QString::number(nd)<<"Rem: \r\n"+QString::number(rem)\
            <<"Web: \r\n"+QString::number(web)<<"Db: \r\n"+QString::number(db)<<"Pol: \r\n"+QString::number(pol)<<"Srv: \r\n"+QString::number(srv)\
            <<"SCADA: \r\n"+QString::number(sc)<<"Mob: \r\n"+QString::number(mob)<<"P2P: \r\n"+QString::number(p2p)<<"DoS: \r\n"+QString::number(dos);

    plg->setData( x,y );
    plg->setName( "Management categories" );
    p->xAxis->setAutoTicks(false);
    p->xAxis->setAutoTickLabels(false);
    p->xAxis->setTickVector(x);
    p->xAxis->setTickVectorLabels(labels);
    //p->xAxis->setTickLabelRotation(60);
    p->xAxis->setSubTickCount(0);
    p->xAxis->setTickLength(0, 4);
    p->xAxis->grid()->setVisible(true);
    p->xAxis->setRange(0, 13 );
    p->yAxis->setRange(0, max( y ) );
    p->xAxis->setPadding( 50 );
    QString caption = te.hasAttribute( "addr" ) ? te.attribute( "addr" ) + " (" + te.attribute( "name" ) + ")"  : "Report";
    p->xAxis->setLabel( QString( "Management categories: " ) + caption );
    p->setMinimumHeight( 500 );
    p->replot();
    return( p );
}

QCustomPlot* ResultsChart::nessusSeverities(const QDomNode &n){
    QCustomPlot *p = new QCustomPlot();
    QCPBars *plg = new QCPBars(p->xAxis, p->yAxis);
    p->addPlottable( plg );
    QDomElement te = n.toElement();
    int c,h,m,l,i;
    c = te.attribute( "sev_critical" ).toInt();
    h = te.attribute( "sev_high" ).toInt();
    m = te.attribute( "sev_medium" ).toInt();
    l = te.attribute( "sev_low" ).toInt();
    i = te.attribute( "sev_info" ).toInt();

    //Bar objects
    QVector<double> x,y;
    x <<1<<2<<3<<4<<5;
    y << c << h << m << l << i;

    QVector<QString> labels;
    labels <<"Critical: "+QString::number(c)<<"High: "+QString::number(h)<<"Medium: "+QString::number(m)<<"Low: "+QString::number(l)<<"Info: "+QString::number(i);

    plg->setData( x,y );
    plg->setName( "Vulnerability severities" );
    p->xAxis->setAutoTicks(false);
    p->xAxis->setAutoTickLabels(false);
    p->xAxis->setTickVector(x);
    p->xAxis->setTickVectorLabels(labels);
    //p->xAxis->setTickLabelRotation(60);
    p->xAxis->setSubTickCount(0);
    p->xAxis->setTickLength(0, 4);
    p->xAxis->grid()->setVisible(true);
    p->xAxis->setRange(0,6);
    p->yAxis->setRange(0, max( y ) );
    QString caption = te.hasAttribute( "addr" ) ? te.attribute( "addr" ) + " (" + te.attribute( "name" ) + ")"  : "Report";
    p->xAxis->setLabel( QString( "Severity levels: " ) + caption );
    p->setMinimumHeight( 500 );
    p->replot();
    return( p );
}

QCustomPlot* ResultsChart::burpSeverities(const QDomNode &n){
    QCustomPlot *p = new QCustomPlot();
    QCPBars *plg = new QCPBars(p->xAxis, p->yAxis);
    p->addPlottable( plg );
    QDomElement te = n.toElement();
    int h,m,l,i;
    h = te.attribute( "high" ).toInt();
    m = te.attribute( "medium" ).toInt();
    l = te.attribute( "low" ).toInt();
    i = te.attribute( "info" ).toInt();

    //Bar objects
    QVector<double> x,y;
    x <<1<<2<<3<<4;
    y << h << m << l << i;

    QVector<QString> labels;
    labels <<"High: "+QString::number(h)<<"Medium: "+QString::number(m)<<"Low: "+QString::number(l)<<"Info: "+QString::number(i);

    plg->setData( x,y );
    plg->setName( "Vulnerability severities" );
    p->xAxis->setAutoTicks(false);
    p->xAxis->setAutoTickLabels(false);
    p->xAxis->setTickVector(x);
    p->xAxis->setTickVectorLabels(labels);
    //p->xAxis->setTickLabelRotation(60);
    p->xAxis->setSubTickCount(0);
    p->xAxis->setTickLength(0, 4);
    p->xAxis->grid()->setVisible(true);
    p->xAxis->setRange(0,5);
    p->yAxis->setRange(0, max( y ) );
    //QString caption = te.hasAttribute( "addr" ) ? te.attribute( "addr" ) + " (" + te.attribute( "name" ) + ")"  : "Report";
    //p->xAxis->setLabel( QString( "Severity levels: " ) + caption );
    p->xAxis->setLabel( "Severity levels" );
    p->setMinimumHeight( 500 );
    p->replot();
    return( p );
}

double ResultsChart::max( QVector<double> &v ){
    if( v.empty() ){
        return( 0.0 );
    }
    else if( v.count() == 1 ){
        return( v.at(1) );
    }
    double ret = v.at(0);
    for( int i=1;i<v.size();++i){
        double d = v.at( i );
        if( d > ret ){
            ret = d;
        }
    }
    return( ret );
}
