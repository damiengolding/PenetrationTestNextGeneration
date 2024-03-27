QT += gui xml core testlib

TEMPLATE = lib
DEFINES += LIBPARSER_LIBRARY

CONFIG += c++20 warn_off

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    PtngConfig.cpp \
    PtngDGMLBuilder.cpp \
    PtngDGMLBuilderNessus.cpp \
    PtngDGMLBuilderNmap.cpp \
    PtngDGMLBuilderSimple.cpp \
    PtngDGMLBuilderTest.cpp \
    PtngDGMLConvDot.cpp \
    PtngHostBuilder.cpp \
    PtngIP4Address.cpp \
    PtngIdent.cpp \
    PtngInputParserAxfr.cpp \
    PtngInputParserNessus.cpp \
    PtngInputParserNmap.cpp \
    PtngInputParserTest.cpp

HEADERS += \
    inc/PtngConfig.hpp \
    inc/PtngDGMLBuilder.hpp \
    inc/PtngDGMLConv.hpp \
    inc/PtngEnums.hpp \
    inc/PtngHostBuilder.hpp \
    inc/PtngIdent.hpp \
    inc/PtngIP4Address.hpp \
    inc/PtngInputParser.hpp \
    inc/PtngSpecifications.hpp \
    inc/libparser_global.hpp

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

# OpenMP
unix:CONFIG{
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS += -fopenmp
    LIBS += -lopenmp
}

# Boost
#    INCLUDEPATH += "C:\Boost_1_81_0"
# Add one path at a time, then add individual library, e.g.
#    LIBS += -L"C:\Boost_1_81_0lib64-msvc-14.3" -lboost_program_options-vc143-mt-gd-x64-1_81.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../WaifsAndStraysOutput/release/ -lWaifsAndStrays
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../WaifsAndStraysOutput/debug/ -lWaifsAndStrays

INCLUDEPATH += $$PWD/../../WaifsAndStraysOutput/inc
DEPENDPATH += $$PWD/../../WaifsAndStraysOutput/inc
