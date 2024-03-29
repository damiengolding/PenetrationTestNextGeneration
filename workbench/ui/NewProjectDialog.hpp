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

#include <QDialog>
#include <QFileDialog>
#include <QListWidgetItem>

#include "../inc/MainWindow.hpp"
#include "../inc/Project.hpp"
using namespace ptng;

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(Project *project READ getProject WRITE setProject NOTIFY projectChanged FINAL)

public:
    explicit NewProjectDialog(QWidget *parent = nullptr);
    ~NewProjectDialog();

    Project *getProject() const;
    void setProject(Project *newProject);

public slots:
    void done(int r);

signals:
    void projectChanged();

protected slots:
    void setProjectFile();
    void setProjectName(const QString &projectName);
    void addWatchDirectory();
    void removeWatchDirectory();
    void enableWatchRemoveButton();

private:
    Ui::NewProjectDialog *ui;
    Project* project;
    QStringList watchDirectories; // To keep an eye on duplicates
    QString projectName;
    QString projectFile;
    QString projectId;
    QString projectDescription;
};

