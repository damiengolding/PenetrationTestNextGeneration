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

#include "../ui/NewProjectDialog.hpp"
#include "../ui/ScanFolderDialog.hpp"

void MainWindow::newProject(){
    QScopedPointer<NewProjectDialog> npd(new NewProjectDialog(this));
    if( npd->exec() == QDialog::Accepted ){
        delete(currentProject);
        currentProject = npd->getProject();
        ui->listViewProject->setModel(currentProject->getProjectTable());
        ui->listViewArtefacts->setModel(currentProject->getArtefactsTable());
    }
    AppendOutput("Created new project at: " % currentProject->getProjectFile());
}

void MainWindow::openProject(){
    QSettings s;
    QString file = QFileDialog::getOpenFileName(this,
                                                "PTNG Workbench",
                                                s.value("defaultProjectDirectory").toString(),
                                                "SQlLite database files (*.db)"
                                                );
    if( file.isEmpty() )
        return;
    delete(currentProject);
    currentProject = new Project(file);
    currentProject->setProjectFile(file);
    currentProject->load();
    ui->listViewProject->setModel(currentProject->getProjectTable());
    ui->listViewArtefacts->setModel(currentProject->getArtefactsTable());
    AppendOutput("Opened project: " % currentProject->getProjectFile());
}

// TODO Need to figure out where to do file processing e.g. to DGML -> dot -> png etc
void MainWindow::addFile(){
    QSettings s;
    QString sourceFile = QFileDialog::getOpenFileName(this,"PTNG Workbench - source file",s.value("defaultDirectory").toString());
    if( sourceFile.isEmpty() ){
        return;
    }
    else if( PtngIdent::checkFile(sourceFile) == PtngEnums::NUM_SUPPORTED_INPUT_TYPES ){
        QMessageBox::warning(this,"PTNG Workbench","The file type is not supported.");
        return;
    }
    QString targetFile = QFileDialog::getSaveFileName(this,"PTNG Workbench - target file",s.value("defaultDirectory").toString());
    if( targetFile.isEmpty() ){
        return;
    }
    QFile::copy(sourceFile,targetFile);
}
// TODO Need to figure out where to do file processing e.g. to DGML -> dot -> png etc
void MainWindow::addFolder(){
    QSettings s;
    QScopedPointer<ScanFolderDialog> sfd( new ScanFolderDialog() );
    sfd->exec();
}
