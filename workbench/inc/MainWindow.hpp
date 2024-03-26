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
#include <QClipboard>
#include <QListWidgetItem>
#include <QToolBar>
#include <QDirIterator>
#include <QtStateMachine>
#include <QState>
#include <QFinalState>

// Ptng - from libparser
#include "PtngDGMLBuilder.hpp"
#include "PtngDGMLConv.hpp"
#include "PtngHostBuilder.hpp"
#include "PtngEnums.hpp"
#include "PtngIP4Address.hpp"
#include "PtngIdent.hpp"
#include "PtngSpecifications.hpp"
using namespace ptng;

// Ptng - from workbench
#include "inc/Project.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(WorkbenchState currentStateEnum READ getCurrentStateEnum WRITE setCurrentStateEnum NOTIFY currentStateEnumChanged FINAL)

public: // Property system
    enum WorkbenchState{
        StateIdling,
        StateLoaded,
        StateCreated,
        StateDirty,
        StateClean,
        StateExitingDirty,
        StateExitingClean,
        StateError
    };
    Q_ENUM(WorkbenchState)

    // Constructor, destructor, Ui::ui
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;

public: // Accessors and mutators
    WorkbenchState getCurrentState() const;
    void setCurrentState(WorkbenchState newCurrentState);
    WorkbenchState getCurrentStateEnum() const;
    void setCurrentStateEnum(WorkbenchState newCurrentStateEnum);

    // Internal objects
private:
    // Directories
    QString defaultProjectDirectory;
    QString defaultDirectory;
    QString pluginDirectory;
    QString scriptDirectory;
    QString configDirectory;
    QString logDirectory;

    // General objects/pointers
    QString windowTitle = "PTNG Workbench";
    QStringList mostRecentlyUsed;
    Project *currentProject;
    QToolBar *mainToolBar;
    QMenu* exportMenu;
    QAction *exportNamicsoft;
    QAction *exportXml;
    QAction *exportCsv;

    // State machine objects/pointers
    QState rootState;
    QStateMachine stateMachine;
    QList<QState*> states;
    WorkbenchState currentStateEnum = StateIdling;
    QState *currentState;

    // Lifecycle functions
private:
    void closeEvent(QCloseEvent *event);
    void restoreMainWindowState();
    void initDefaults();
    void initToolbar();
    void initAdminDirectories();
    void initStateMachine();
    void initConnections();

    // Menu and toolbar
public slots:
    void newProject();
    void openProject();
    void addFile();
    void addFolder();
    void showPreferences();
    void showAboutQt();
    void showAbout();
    void showExplorerDock(bool show);
    void showOutputDock(bool show);

    // Internal management
protected slots:
    void AppendOutput(const QString &line);
    void clearOutput();

signals:
    void currentStateEnumChanged();
};
