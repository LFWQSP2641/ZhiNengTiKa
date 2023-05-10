include(./ZhiNengTiKaCore.pri)

QT       += widgets gui quick quickcontrols2 webview

PRECOMPILED_HEADER = src/pchQML.h

HEADERS += \
    src/QMLIntermediary/QRCodeScannerQML.h \
    src/QMLIntermediary/SettingOperator.h \
    src/QMLIntermediary/TemplateAnalysisQML.h \
    src/QMLIntermediary/TemplateRawDataQML.h \
    src/QMLIntermediary/TemplateSummaryQML.h
SOURCES += \
    src/QMLIntermediary/QRCodeScannerQML.cpp \
    src/QMLIntermediary/SettingOperator.cpp \
    src/QMLIntermediary/TemplateAnalysisQML.cpp \
    src/QMLIntermediary/TemplateRawDataQML.cpp \
    src/QMLIntermediary/TemplateSummaryQML.cpp \
    src/mainQML.cpp \
    src/pchQML.h.cpp

RESOURCES += \
    Resource/qml.qrc
