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

#include <QtCore/qglobal.h>
#include <QDomDocument>
#include <QDomNode>
#include <QDomNodeList>
#include <QDomElement>
#include <QDomText>
#include <QDomAttr>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QList>
#include <QDebug>
#include <QDateTime>
#include <QProcess>
#include <QMainWindow>
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidgetItem>

// STD
#include <iostream>
#include <string>

// ptng - from libparser
#include "PtngEnums.hpp"
#include "PtngIdent.hpp"
#include "PtngHostBuilder.hpp"
#include "PtngSpecifications.hpp"
#include "PtngInputParser.hpp"
#include "PtngDGMLBuilder.hpp"
#include "PtngIP4Address.hpp"
#include "PtngDGMLConv.hpp"

// ptng - from workbench
#include "inc/PtngProject.hpp"

using namespace ptng;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString windowTitle = "PTNG Workbench";
    QStringList mostRecentlyUsed;
    PtngProject *currentProject;

    // Lifecycle
private:
    void closeEvent(QCloseEvent *event);
    void restoreMainWindowState();
    void initDefaults();

public slots:
    void newProject();
    void showPreferences();
    void showAboutQt();
    void showAbout();

};