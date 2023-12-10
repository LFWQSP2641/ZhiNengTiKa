VERSION = 3.0.0

QT += core network multimedia core5compat

android{
QT += core-private

include($$PWD/3rd/android_openssl/openssl.pri)

}
win32{

RC_ICONS = Resource/img/xinjiaoyuico.ico

QMAKE_TARGET_PRODUCT = "ZhiNengTiKa"

QMAKE_TARGET_COPYRIGHT = "Copyright © 2022 - 2023 LFWQSP2641.All Rights Reserved."

RC_LANG = 0x0004

}

#DEFINES += LIMITED

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DEFINES += DATABASE_DOMAIN=\\\"https://gitee.com/LFWQSP2641/zhinengtika_database/raw/master/\\\"

DEFINES *= QT_USE_QSTRINGBUILDER

CONFIG += c++17 precompile_header

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/3rd
include($$PWD/3rd/Qt-AES/QAESEncryption.pri)

INCLUDEPATH += $$PWD/lib/zxing-cpp
DEPENDPATH += $$PWD/lib/zxing-cpp

win32:LIBS += -L$$PWD/lib/zxing-cpp/LibZXingCpp/windows-msvc2019-64bit -lZXingCpp
android:LIBS += -L$$PWD/lib/zxing-cpp/LibZXingCpp/android/ -lZXingCpp_arm64-v8a
#android:LIBS += -L$$PWD/lib/zxing-cpp/LibZXingCpp/android/ -lZXingCpp_armeabi-v7a
#android:LIBS += -L$$PWD/lib/zxing-cpp/LibZXingCpp/android/ -lZXingCpp_x86
#android:LIBS += -L$$PWD/lib/zxing-cpp/LibZXingCpp/android/ -lZXingCpp_x86_64

INCLUDEPATH += $$PWD/lib/unzip/quazip/quazip
INCLUDEPATH += $$PWD/lib/unzip/zlib
win32 {
INCLUDEPATH += $$PWD/lib/unzip/windows-msvc2019-64bit
LIBS += -L$$PWD/lib/unzip/windows-msvc2019-64bit -lquazip1-qt6
}
android {
INCLUDEPATH += $$PWD/lib/unzip/android/arm64_v8a
LIBS += -L$$PWD/lib/unzip/android/arm64_v8a -lquazip1-qt6
}

HEADERS += \
    $$PWD/src/Logic/AccountManager.h \
    $$PWD/src/Logic/AnnouncementManager.h \
    $$PWD/src/Logic/AnnouncementModel.h \
    $$PWD/src/Logic/AnswerDetailData.hpp \
    $$PWD/src/Logic/FileTreeItem.h \
    $$PWD/src/Logic/FileTreeModel.h \
    $$PWD/src/Logic/ImageProvider.h \
    $$PWD/src/Logic/MultipleSubjectsTemplateListModelList.h \
    $$PWD/src/Logic/NetworkAccessManagerBlockable.h \
    $$PWD/src/Logic/QRCodeReader.h \
    $$PWD/src/Logic/QRCodeScanner.h \
    $$PWD/src/Logic/ResourceFileFetcher.h \
    $$PWD/src/Logic/ResourceFileModel.h \
    $$PWD/src/Logic/TemplateAnalysis.h \
    $$PWD/src/Logic/TemplateFetcher.h \
    $$PWD/src/Logic/TemplateListModel.h \
    $$PWD/src/Logic/TemplateSearcher.h \
    $$PWD/src/Logic/TemplateSummary.h \
    $$PWD/src/Logic/UserData.h \
    $$PWD/src/Singleton/Network.h \
    $$PWD/src/Singleton/Settings.h \
    $$PWD/src/StaticClass/CallAndroidNativeComponent.h \
    $$PWD/src/StaticClass/Global.h \
    $$PWD/src/StaticClass/XinjiaoyuEncryptioner.h \
    $$PWD/src/StaticClass/XinjiaoyuNetwork.h

SOURCES += \
    $$PWD/src/Logic/AccountManager.cpp \
    $$PWD/src/Logic/AnnouncementManager.cpp \
    $$PWD/src/Logic/AnnouncementModel.cpp \
    $$PWD/src/Logic/FileTreeItem.cpp \
    $$PWD/src/Logic/FileTreeModel.cpp \
    $$PWD/src/Logic/ImageProvider.cpp \
    $$PWD/src/Logic/QRCodeReader.cpp \
    $$PWD/src/Logic/QRCodeScanner.cpp \
    $$PWD/src/Logic/MultipleSubjectsTemplateListModelList.cpp \
    $$PWD/src/Logic/NetworkAccessManagerBlockable.cpp \
    $$PWD/src/Logic/ResourceFileFetcher.cpp \
    $$PWD/src/Logic/ResourceFileModel.cpp \
    $$PWD/src/Logic/TemplateAnalysis.cpp \
    $$PWD/src/Logic/TemplateFetcher.cpp \
    $$PWD/src/Logic/TemplateListModel.cpp \
    $$PWD/src/Logic/TemplateSearcher.cpp \
    $$PWD/src/Logic/TemplateSummary.cpp \
    $$PWD/src/Logic/UserData.cpp \
    $$PWD/src/Singleton/Network.cpp \
    $$PWD/src/Singleton/Settings.cpp \
    $$PWD/src/StaticClass/CallAndroidNativeComponent.cpp \
    $$PWD/src/StaticClass/Global.cpp \
    $$PWD/src/StaticClass/XinjiaoyuEncryptioner.cpp \
    $$PWD/src/StaticClass/XinjiaoyuNetwork.cpp

RESOURCES += \
    $$PWD/Resource/Resource.qrc \
    $$PWD/Resource/Template.qrc
