TEMPLATE = subdirs

SUBDIRS += \
    libparser \
    ifident \
    ndgml \
    ntxt \
    nviz \
    workbench \
    ztest

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../PtngOutput/libparser/release/ -llibparser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../PtngOutput/libparser/debug/ -llibparser

INCLUDEPATH += $$PWD/../PtngOutput/libparser/inc
DEPENDPATH += $$PWD/../PtngOutput/libparser/inc
