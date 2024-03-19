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
#include "inc/CommandLineHandler.hpp"

void processNessusVulns(const QString &sourceFile,
                 const QString &conversionType,
                 const QString &outputFileStem,
                        const QString &command){
    qInfo() << "[info] Processing Nessus native XML to host vulnerabilities - not yet implemented";

    // QList<PtngHostBuilder*> builders = PtngInputParser::parseNessus(sourceFile);
    // qInfo() << "[info] Number of nessus host buidlers:"<<builders.count();
    // qInfo() << "[info] Got here 0";
    // auto set0 = new QBarSet("Critical");
    // auto set1 = new QBarSet("High");
    // auto set2 = new QBarSet("Medium");
    // auto set3 = new QBarSet("Low");
    // auto set4 = new QBarSet("None");
    // qInfo() << "[info] Got here 1";
    // *set0 << 47;
    // *set1 << 93;
    // *set2 << 152;
    // *set3 << 70;
    // *set4 << 222;
    // qInfo() << "[info] Got here 2";
    // QBarSeries *series = new QBarSeries();
    // series->append(set0);
    // series->append(set1);
    // series->append(set2);
    // series->append(set3);
    // series->append(set4);
    // qInfo() << "[info] Got here 3";
    // QChart *chart = new QChart;
    // qInfo() << "[info] Got here 3a";
    // chart->addSeries(series);
    // qInfo() << "[info] Got here 3b";
    // chart->setTitle("Fake IP Address");
    // qInfo() << "[info] Got here 3c";
    // chart->setAnimationOptions(QChart::SeriesAnimations);
    // qInfo() << "[info] Got here 4";
    // QChartView *view = new QChartView();
    // view->setVisible(false);
    // view->setChart(chart);
    // qInfo() << "[info] Got here 5";
    // QImage *image = new QImage();
    // qInfo() << "[info] Got here 6";
    // QPixmap pixmap = view->grab();
    // qInfo() << "[info] Got here 7";
    // pixmap.save("test.png");
}
