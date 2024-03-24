QT = core gui xml

CONFIG += c++20 cmdline warn_off

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    inc/GlobalIncludes.hpp \
    inc/CommandLineHandler.hpp

SOURCES += \
        CommandLineHandler.cpp \
        NessusXmlHostVulns.cpp \
        NessusXmlPluginFamilies.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../PtngOutput/libparser/release/ -llibparser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../PtngOutput/libparser/debug/ -llibparser

INCLUDEPATH += $$PWD/../../PtngOutput/libparser/inc
DEPENDPATH += $$PWD/../../PtngOutput/libparser/inc

