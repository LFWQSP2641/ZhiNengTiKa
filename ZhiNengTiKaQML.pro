include(./ZhiNengTiKaCore.pri)

QT       += widgets gui quick quickcontrols2 webview

PRECOMPILED_HEADER = src/pchQML.h

HEADERS += \
    src/QMLIntermediary/AnimeImageProvider.h \
    src/QMLIntermediary/MultipleSubjectsTemplateListModelListSingleton.h \
    src/QMLIntermediary/QMLUtils.h \
    src/QMLIntermediary/SettingOperator.h \
    src/QMLIntermediary/TemplateAnalysisQML.h \
    src/QMLIntermediary/TemplateRawDataQML.h \
    src/QMLIntermediary/TemplateSummaryQML.h \
    src/QMLIntermediary/ZAccelerationToOpacityConverter.h
SOURCES += \
    src/QMLIntermediary/AnimeImageProvider.cpp \
    src/QMLIntermediary/MultipleSubjectsTemplateListModelListSingleton.cpp \
    src/QMLIntermediary/QMLUtils.cpp \
    src/QMLIntermediary/SettingOperator.cpp \
    src/QMLIntermediary/TemplateAnalysisQML.cpp \
    src/QMLIntermediary/TemplateRawDataQML.cpp \
    src/QMLIntermediary/TemplateSummaryQML.cpp \
    src/QMLIntermediary/ZAccelerationToOpacityConverter.cpp \
    src/mainQML.cpp \
    src/pchQML.h.cpp

RESOURCES += \
    Resource/qml.qrc
