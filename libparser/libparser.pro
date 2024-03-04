QT += gui xml

TEMPLATE = lib
DEFINES += LIBPARSER_LIBRARY

CONFIG += c++20 warn_off

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    PtngDGMLBuilder.cpp \
    PtngHostBuilder.cpp \
    PtngIP4Address.cpp \
    PtngIdent.cpp \
    PtngInputParser.cpp \
    PtngStyleSheet.cpp

HEADERS += \
    inc/PtngDGMLBuilder.hpp \
    inc/PtngEnums.hpp \
    inc/PtngHostBuilder.hpp \
    inc/PtngIdent.hpp \
    inc/PtngIP4Address.hpp \
    inc/PtngInputParser.hpp \
    inc/PtngSpecifications.hpp \
    inc/PtngStyleSheet.hpp \
    inc/ConsoleSupport.hpp \
    inc/libparser_global.hpp \

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
