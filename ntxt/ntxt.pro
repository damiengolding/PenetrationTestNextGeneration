QT = core xml gui

CONFIG += c++20 cmdline warn_off

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        CommandLineHandler.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    inc/CommandLineHandler.hpp \
    inc/GlobalIncludes.hpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../PTNG_Output/libparser/release/ -llibparser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../PTNG_Output/libparser/debug/ -llibparser

INCLUDEPATH += $$PWD/../../PTNG_Output/libparser/inc
DEPENDPATH += $$PWD/../../PTNG_Output/libparser/inc
