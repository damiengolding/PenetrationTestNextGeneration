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

/*
   Start up
  */

void MainWindow::restoreMainWindowState(){
    QSettings s;
    // State
    restoreGeometry(s.value("geometry").toByteArray());
    restoreState(s.value("windowState").toByteArray());
    // MRU
    mostRecentlyUsed = s.value("mru").toString().split(";",Qt::SkipEmptyParts);
    // Menu
    connect(ui->actionAboutQt,
            SIGNAL(triggered()),
            this,
            SLOT(showAboutQt())
            );
    connect(
                ui->actionAboutWorkbench,
                SIGNAL(triggered()),
                this,
                SLOT(showAbout())
                );
    connect(
                ui->actionNewProject,
                SIGNAL(triggered()),
                this,
                SLOT(newProject())
                );
    connect(ui->actionPreferences,
            SIGNAL( triggered()),
            this,
            SLOT( showPreferences())
            );
    connect(ui->actionShowExplorer,
            SIGNAL( triggered(bool) ),
            this,
            SLOT( showExplorerDock(bool) )
            );
    connect(ui->actionShowOutput,
            SIGNAL( triggered(bool) ),
            this,
            SLOT( showOutputDock(bool) )
            );
    connect(ui->actionAddFile,
            SIGNAL( triggered() ),
            this,
            SLOT( addFile() )
            );
    connect(ui->actionScanFolder,
            SIGNAL( triggered() ),
            this,
            SLOT( addFolder() )
            );
}

void MainWindow::initAdminDirectories(){
    QDir defaultDir(defaultDirectory);
    if( !defaultDir.exists() ){
        defaultDir.mkpath(defaultDirectory);
    }
    QDir scriptDir(scriptDirectory);
    if( !scriptDir.exists() ){
        scriptDir.mkpath(scriptDirectory);
    }
    QDir pluginDir(pluginDirectory);
    if( !pluginDir.exists() ){
        pluginDir.mkpath(pluginDirectory);
    }
    QDir configDir(configDirectory);
    if( !configDir.exists() ){
        configDir.mkpath(configDirectory);
    }
}

void MainWindow::initDefaults(){
    // Init any pointers

    // Default directories
    QSettings s;
    if( !s.contains("defaultDirectory")){
        s.setValue("defaultDirectory",QDir::homePath() + "/AppData/Local/Golding's Gym/PTNG Workbench" );
    }
    defaultDirectory = s.value("defaultDirectory").toString();
    if( !s.contains("defaultProjectDirectory") ){
        s.setValue("defaultProjectDirectory",QDir::homePath() +  "/AppData/Local/Golding's Gym/PTNG Workbench" );
    }
    defaultProjectDirectory = s.value("defaultProjectDirectory").toString();
    if( !s.contains("pluginDirectory") ){
        s.setValue("pluginDirectory",defaultDirectory + "/plugins");
    }
    pluginDirectory = s.value("pluginDirectory").toString();
    if( !s.contains("scriptDirectory")){
        s.setValue("scriptDirectory", defaultDirectory + "/scripts");
    }
    scriptDirectory  = s.value("scriptDirectory").toString();
    if( !s.contains("configDirectory")){
        s.setValue("configDirectory", defaultDirectory + "/config");
    }
    configDirectory  = s.value("configDirectory").toString();

    // Look and feel
    QFont displayFont(s.value("displayFont","Open Sans").toString(), s.value("fontSize",12).toInt() );
    this->setFont(displayFont);

    // Toolbar
    mainToolBar = this->addToolBar("Tools");
    mainToolBar->setObjectName("Maintoolbar");
    mainToolBar->addAction(ui->actionNewProject);
    mainToolBar->addAction( ui->actionAddFile );
    mainToolBar->addAction( ui->actionScanFolder );
    mainToolBar->addSeparator();
    mainToolBar->addAction( ui->actionPreferences );
    mainToolBar->addAction(ui->actionShowExplorer);
    mainToolBar->addAction(ui->actionShowOutput);

    // Main menu
    exportMenu = ui->menuProject->addMenu("Export");
    exportNamicsoft = new QAction("Namicsoft");
    exportXml = new QAction("XML");
    exportCsv = new QAction("CSV");

    exportMenu->addAction(exportNamicsoft);
    exportMenu->addAction(exportXml);
    exportMenu->addAction(exportCsv);

    // Default project DB sql
    QString projectTable = "CREATE TABLE 'project' ('display_name' TEXT NOT NULL,'project_id' TEXT,'created_date' TEXT,'project_title' TEXT NOT NULL,'working_directory' TEXT NOT NULL,'watch_directories' TEXT)";
    QString artefactsTable = "CREATE TABLE 'artefacts' ('display_name' TEXT NOT NULL,'artefact_id' INTEGER NOT NULL UNIQUE,'source_tool' TEXT NOT NULL,'source_file' TEXT NOT NULL,'output_file' TEXT,PRIMARY KEY('artefact_id' AUTOINCREMENT))";
    QString newProjectFile = configDirectory % QDir::separator() % "new_project.txt";
    if( !QFile::exists(newProjectFile) ){
        QFile npSql(newProjectFile);
        if( !npSql.open(QIODevice::WriteOnly) ){
            qWarning() << "[warning] Could not open"<<newProjectFile<<"for writing";
            return;
        }
        QTextStream outFile(&npSql);
        outFile << projectTable << "\r\n" << artefactsTable;
        npSql.close();
    }
}

void MainWindow::initStateMachine(){
    // Create states
    QState *stateIdling = new QState();
    // stateMachine.addState(stateIdling);
    stateMachine.setInitialState(stateIdling);
    states.append(stateIdling);

    QState *stateLoaded = new QState();
    stateMachine.addState(stateLoaded);
    states.append(stateLoaded);

    QState *stateCreated = new QState();
    stateMachine.addState(stateCreated);
    states.append(stateCreated);

    QState *stateDirty = new QState();
    stateMachine.addState(stateDirty);
    states.append(stateDirty);

    QState *stateClean = new QState();
    stateMachine.addState(stateClean);
    states.append(stateClean);

    QState *stateExitingDirty = new QState();
    stateMachine.addState(stateExitingDirty);
    states.append(stateExitingDirty);

    QState *stateExitingClean = new QState();
    stateMachine.addState(stateExitingClean);
    states.append(stateExitingClean);

    QFinalState *stateExit = new QFinalState();
    stateMachine.addState(stateExit);

}

void MainWindow::initConnections(){
    // Menu
    connect(ui->actionAboutQt,
            &QAction::triggered,
            this,
            &MainWindow::showAboutQt
            ,Qt::UniqueConnection
            );
    connect(
                ui->actionAboutWorkbench,
                &QAction::triggered,
                this,
                &MainWindow::showAbout
                ,Qt::UniqueConnection
                );
    connect(
                ui->actionNewProject,
                &QAction::triggered,
                this,
                &MainWindow::newProject
                ,Qt::UniqueConnection
                );
    connect(ui->actionPreferences,
            &QAction::triggered,
            this,
            &MainWindow::showPreferences
            ,Qt::UniqueConnection
            );
    connect(ui->actionShowExplorer,
            &QAction::triggered,
            this,
            &MainWindow::showExplorerDock
            ,Qt::UniqueConnection
            );
    connect(ui->actionShowOutput,
            &QAction::triggered,
            this,
            &MainWindow::showOutputDock
            ,Qt::UniqueConnection
            );
    connect(ui->actionAddFile,
            &QAction::triggered,
            this,
            &MainWindow::addFile
            ,Qt::UniqueConnection
            );
    connect(ui->actionScanFolder,
            &QAction::triggered,
            this,
            &MainWindow::addFolder
            ,Qt::UniqueConnection
            );
}

/*
   Shut down
  */

void MainWindow::closeEvent(QCloseEvent *event){
    QSettings s;
    // State
    s.setValue("geometry",saveGeometry());
    s.setValue("windowState",saveState());
    // MRU
    s.setValue("mru",mostRecentlyUsed.join(";"));

    QMainWindow::closeEvent(event);
}

