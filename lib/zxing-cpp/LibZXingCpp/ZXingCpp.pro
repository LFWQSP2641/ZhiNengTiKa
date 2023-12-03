QT += gui multimedia

TEMPLATE = lib
CONFIG += staticlib
DEFINES += ZXINGCPP_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../zxing-cpp/core/src
include($$PWD/../zxing-cpp/core/ZxingInclude.pri)

SOURCES += \
    ZXingPosition.cpp \
    ZXingReader.cpp \
    ZXingResult.cpp

HEADERS += \
    ZXingCpp_global.h \
    ZXingPosition.h \
    ZXingReader.h \
    ZXingResult.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
