QT += gui xml

TEMPLATE = lib
DEFINES += LIBPARSER_LIBRARY

CONFIG += c++20 warn_off

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    PtngAddressParser.cpp \
    PtngHostBuilder.cpp \
    PtngIP4Address.cpp \
    PtngIdent.cpp \
    PtngStyleSheet.cpp

HEADERS += \
    ConsoleSupport.hpp \
    PtngAddressParser.hpp \
    PtngEnums.hpp \
    PtngHostBuilder.hpp \
    PtngIP4Address.hpp \
    PtngIdent.hpp \
    PtngSpecifications.hpp \
    PtngStyleSheet.hpp \
    libparser_global.hpp

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
