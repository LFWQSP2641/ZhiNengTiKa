include(./ZhiNengTiKaCore.pri)

TEMPLATE = lib

QT += widgets gui quick quickcontrols2 webview sensors

PRECOMPILED_HEADER = src/pchQML.h

HEADERS += \
    src/QMLIntermediary/AccelerometerSingleton.h \
    src/QMLIntermediary/AnimeImageProvider.h \
    src/QMLIntermediary/MultipleSubjectsTemplateListModelListSingleton.h \
    src/QMLIntermediary/QMLUtils.h \
    src/QMLIntermediary/SettingOperator.h \
    src/QMLIntermediary/ZAccelerationToOpacityConverter.h
SOURCES += \
    src/QMLIntermediary/AccelerometerSingleton.cpp \
    src/QMLIntermediary/AnimeImageProvider.cpp \
    src/QMLIntermediary/MultipleSubjectsTemplateListModelListSingleton.cpp \
    src/QMLIntermediary/QMLUtils.cpp \
    src/QMLIntermediary/SettingOperator.cpp \
    src/QMLIntermediary/ZAccelerationToOpacityConverter.cpp \
    src/mainQML.cpp \
    src/pchQML.h.cpp

RESOURCES += \
    Resource/qml.qrc
