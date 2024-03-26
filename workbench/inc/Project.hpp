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
#include <QtCore/qglobal.h>
#include <QFile>
#include <QTextStream>
#include <QMap>.
#include <QList>
#include <QDebug>
#include <QDateTime>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QDir>

/*!
   \brief The ProjectArtefact class
 */
struct ProjectArtefact : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString sourceTool READ getSourceTool WRITE setSourceTool NOTIFY sourceToolChanged FINAL)
    Q_PROPERTY(QString sourceFile READ getSourceFile WRITE setSourceFile NOTIFY sourceFileChanged FINAL)
    Q_PROPERTY(QString outputFile READ getOutputFile WRITE setOutputFile NOTIFY outputFileChanged FINAL)
    Q_PROPERTY(QString Id READ getId WRITE setId NOTIFY IdChanged FINAL)

public:
    explicit ProjectArtefact(QObject *parent = nullptr);
    QString getSourceTool() const;
    void setSourceTool(const QString &newSourceTool);
    QString getSourceFile() const;
    void setSourceFile(const QString &newSourceFile);
    QString getOutputFile() const;
    void setOutputFile(const QString &newOutputFile);
    QString getId() const;
    void setId(const QString &newId);


signals:
    void sourceToolChanged();
    void sourceFileChanged();
    void outputFileChanged();
    void IdChanged();

private:
    QString sourceTool;
    QString sourceFile;
    QString outputFile;
    QString Id;
};

/*!
   \brief The Project class
 */
class Project : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString projectName READ getProjectName WRITE setProjectName NOTIFY projectNameChanged FINAL)
    Q_PROPERTY(QString projectFile READ getProjectFile WRITE setProjectFile NOTIFY projectFileChanged FINAL)
    Q_PROPERTY(QString projectId READ getProjectId WRITE setProjectId NOTIFY projectIdChanged FINAL)
    Q_PROPERTY(QSqlDatabase database READ getDatabase WRITE setDatabase NOTIFY databaseChanged FINAL)
    Q_PROPERTY(QString projectDescription READ getProjectDescription WRITE setProjectDescription NOTIFY projectDescriptionChanged FINAL)
    Q_PROPERTY(QStringList watchDirectories READ getWatchDirectories WRITE setWatchDirectories NOTIFY watchDirectoriesChanged FINAL)

public:
    explicit Project(const QString &inputFileName = "", QObject *parent = nullptr);
    ~Project();

    bool load();
    bool create();
    QSqlTableModel* getProjectTable();
    QSqlTableModel* getArtefactsTable();

    QString getProjectName() const;
    void setProjectName(const QString &newProjectName);

    QString getProjectFile() const;
    void setProjectFile(const QString &newProjectFile);

    QString getProjectId() const;
    void setProjectId(const QString &newProjectId);

    QSqlDatabase getDatabase() const;
    void setDatabase(const QSqlDatabase &newDatabase);

    QStringList getWatchDirectories() const;
    void setWatchDirectories(const QStringList &newWatchDirectories);

    QString getProjectDescription() const;
    void setProjectDescription(const QString &newProjectDescription);

private: // Functions
    bool initDatabase();

private: // Members
    QString projectDisplayName;
    QString projectDescription;
    QString projectFile;
    QString projectId;
    QStringList watchDirectories;
    QSqlDatabase database;
    QString projectTableSql = "CREATE TABLE 'project' ('display_name' TEXT NOT NULL,'project_id' TEXT,'created_date' TEXT,'project_title' TEXT NOT NULL,'working_directory' TEXT NOT NULL,'watch_directories' TEXT)";
    QString artefactsTableSql = "CREATE TABLE 'artefacts' ('display_name' TEXT NOT NULL UNIQUE,'artefact_id' INTEGER NOT NULL UNIQUE,'source_tool' TEXT NOT NULL,'source_file' TEXT NOT NULL,'output_file' TEXT,PRIMARY KEY('artefact_id' AUTOINCREMENT))";

signals:
    void projectNameChanged();
    void projectFileChanged();
    void projectIdChanged();
    void databaseChanged();
    void projectTitleChanged();
    void watchDirectoriesChanged();
    void projectDescriptionChanged();
};


