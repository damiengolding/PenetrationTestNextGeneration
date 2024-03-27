QT = core gui xml testlib network

CONFIG += c++20 cmdline warn_off
DEFINES+= QT_MESSAGELOGCONTEXT

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        tests.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../PtngOutput/libparser/release/ -llibparser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../PtngOutput/libparser/debug/ -llibparser

INCLUDEPATH += $$PWD/../../PtngOutput/libparser/inc
DEPENDPATH += $$PWD/../../PtngOutput/libparser/inc

HEADERS += \
    main.hpp \
    tests.hpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../WaifsAndStraysOutput/release/ -lWaifsAndStrays
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../WaifsAndStraysOutput/debug/ -lWaifsAndStrays

INCLUDEPATH += $$PWD/../../WaifsAndStraysOutput/inc
DEPENDPATH += $$PWD/../../WaifsAndStraysOutput/inc
