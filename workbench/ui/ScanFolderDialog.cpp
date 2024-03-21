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
#include "ScanFolderDialog.hpp"
#include "ui_ScanFolderDialog.h"

ScanFolderDialog::ScanFolderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScanFolderDialog)
{
    ui->setupUi(this);
    connect(ui->copyRadioButton,
            SIGNAL( toggled(bool) ),
            this,
            SLOT( toggleCopySources(bool) )
            );
    connect(ui->sdPushButton,
            SIGNAL( clicked() ),
            this,
            SLOT( selectSourceDirectory())
            );
    connect(ui->tdPushButton,
            SIGNAL( clicked() ),
            this,
            SLOT( selectTargetDirectory())
            );
}

ScanFolderDialog::~ScanFolderDialog()
{
    delete ui;
}

void ScanFolderDialog::toggleCopySources(bool checked)
{
    ui->dsRadioButton->setEnabled(checked);
    ui->tdPushButton->setEnabled(checked);
    ui->tdLabel->setEnabled(checked);
    ui->tdLineEdit->setEnabled(checked);
}

void ScanFolderDialog::selectSourceDirectory()
{
    QString directory = QFileDialog::getExistingDirectory(this,"PTNG Workbench");
    if( directory.isEmpty() ){
        return;
    }
    ui->sdLineEdit->setText(directory);
}

void ScanFolderDialog::selectTargetDirectory()
{
    QString directory = QFileDialog::getExistingDirectory(this,"PTNG Workbench");
    if( directory.isEmpty() ){
        return;
    }
    ui->tdLineEdit->setText(directory);
}

void ScanFolderDialog::done(int r)
{
    if( r == QDialog::Accepted ){
        if( !QFile::exists( ui->sdLineEdit->text() ) ){
            QMessageBox::information(this, "PTNG Workbench","An existing directory needs to be\nsupplied for the source directory.");
            return;
        }
        else if( ui->copyRadioButton->isChecked() && !QFile::exists( ui->tdLineEdit->text() ) ){
            QMessageBox::information(this, "PTNG Workbench","An existing directory needs to be\nsupplied for the target directory.");
            return;
        }
        processFiles( ui->sdLineEdit->text(),
                      ui->tdLineEdit->text(),
                      ui->caRadioButton->isChecked(),
                      ui->copyRadioButton->isChecked(),
                      ui->dsRadioButton->isChecked() );
        QDialog::done(r);
    }
    else{
        QDialog::done(r);
    }
}

// TODO need to put a QProgressDialog in here
// TODO Need to figure out where to do file processing e.g. to DGML -> dot -> png etc
void ScanFolderDialog::processFiles(const QString &sourceDirectory, const QString &targetDirectory, bool copySources, bool copyAll, bool keepStructure)
{
    qInfo() << "[info] Processing files - source dir:"<<sourceDirectory<<"Target dir:"<<targetDirectory;
    QDir src(sourceDirectory);
    QDir tgt(targetDirectory);
    QDirIterator copyIterator(sourceDirectory, QDir::NoDotAndDotDot | QDir::AllEntries, QDirIterator::Subdirectories);
    int sourceCount = 0;
    QDirIterator countIterator(sourceDirectory, QDir::NoDotAndDotDot | QDir::AllEntries, QDirIterator::Subdirectories);
    while( countIterator.hasNext()){
        ++sourceCount;
        countIterator.next();
    }
    while( copyIterator.hasNext()){
        QString source = copyIterator.next();
        QFileInfo sourceInfo(source);
        if( sourceInfo.isDir() ){
            QString tmpDir = targetDirectory % source.remove(sourceDirectory);
            tgt.mkdir(tmpDir);
            continue;
        }
        QString target = targetDirectory % source.remove(sourceDirectory);
        if( copySources && copyAll ){
            if( QFile::exists(target) ){
                QFile::remove(target);
            }
            if( !QFile::copy(sourceInfo.absoluteFilePath(),target) ){
                qInfo() << "[info] Failed to copy file:"<<sourceInfo.absoluteFilePath()<<"to"<<target;
            }
        }
        else{
            if( PtngIdent::checkFile(source) != PtngEnums::NUM_SUPPORTED_INPUT_TYPES ){
                QFile::copy(source,target);
            }
        }
    }
}
