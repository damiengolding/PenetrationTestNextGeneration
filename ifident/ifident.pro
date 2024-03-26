QT += core xml gui testlib

CONFIG += c++20 cmdline warn_off

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# libparser references
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../PtngOutput/libparser/release/ -llibparser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../PtngOutput/libparser/debug/ -llibparser

INCLUDEPATH += $$PWD/../libparser/inc
DEPENDPATH += $$PWD/../libparser/inc
