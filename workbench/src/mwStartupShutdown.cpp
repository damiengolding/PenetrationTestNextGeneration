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

#include "../inc/MainWindow.hpp"
#include "ui_MainWindow.h"

void MainWindow::restoreMainWindowState(){
    QSettings s;
    // State
    restoreGeometry(s.value("geometry").toByteArray());
    restoreState(s.value("windowState").toByteArray());
    // MRU
    mostRecentlyUsed = s.value("mru").toString().split(";",Qt::SkipEmptyParts);
    // Menu
    connect( ui->actionAboutQt, SIGNAL(triggered()),this, SLOT(showAboutQt()));
    connect( ui->actionAboutWorkbench, SIGNAL(triggered()),this,SLOT(showAbout()));
    connect( ui->actionNewProject, SIGNAL(triggered()),this, SLOT(newProject()));
}

void MainWindow::closeEvent(QCloseEvent *event){
    QSettings s;
    // State
    s.setValue("geometry",saveGeometry());
    s.setValue("windowState",saveState());
    // MRU
    s.setValue("mru",mostRecentlyUsed.join(";"));

    QMainWindow::closeEvent(event);
}
