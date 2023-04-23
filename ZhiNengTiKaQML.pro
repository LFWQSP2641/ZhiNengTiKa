include(./ZhiNengTiKaCore.pri)

QT       += widgets gui quick webview

PRECOMPILED_HEADER = src/pchQML.h

HEADERS += \
    src/QMLIntermediary/QRCodeScannerQML.h \
    src/QMLIntermediary/SettingOperator.h
SOURCES += \
    src/QMLIntermediary/QRCodeScannerQML.cpp \
    src/QMLIntermediary/SettingOperator.cpp \
    src/mainQML.cpp \
    src/pchQML.h.cpp

RESOURCES += \
    Resource/qml.qrc
