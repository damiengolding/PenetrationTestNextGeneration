TEMPLATE = subdirs

SUBDIRS += \
    libparser \
    ifident \
    libtest \
    ndgml \
    ntxt \
    nviz \
    workbench

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../PTNG_Output/libparser/release/ -llibparser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../PTNG_Output/libparser/debug/ -llibparser

INCLUDEPATH += $$PWD/../PTNG_Output/libparser/inc
DEPENDPATH += $$PWD/../PTNG_Output/libparser/inc
