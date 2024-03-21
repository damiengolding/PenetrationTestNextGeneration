QT       += core gui xml charts concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20 warn_off

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/MainWindow.cpp \
    src/PtngProject.cpp \
    src/mwEventsProject.cpp \
    src/mwMenuEventsAdmin.cpp \
    src/mwStartupShutdown.cpp \
    ui/NewProjectDialog.cpp \
    ui/PreferencesDialog.cpp \
    ui/ScanFolderDialog.cpp

HEADERS += \
    inc/MainWindow.hpp \
    inc/PtngProject.hpp \
    ui/NewProjectDialog.hpp \
    ui/PreferencesDialog.hpp \
    ui/ScanFolderDialog.hpp

FORMS += \
    ui/MainWindow.ui \
    ui/NewProjectDialog.ui \
    ui/PreferencesDialog.ui \
    ui/ScanFolderDialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../PTNG_Output/libparser/release/ -llibparser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../PTNG_Output/libparser/debug/ -llibparser

INCLUDEPATH += $$PWD/../../PTNG_Output/libparser/inc
DEPENDPATH += $$PWD/../../PTNG_Output/libparser/inc

RESOURCES += \
    res/workbench.qrc
