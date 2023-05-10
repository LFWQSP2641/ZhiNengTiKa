VERSION = 2.0.8

QT       += core network

android{
QT       += core-private

ANDROID_VERSION_NAME = $$VERSION

include(3rd/android_openssl/openssl.pri)

ANDROID_PACKAGE_SOURCE_DIR = \
    $$PWD/android

}
win32{
QT       += webenginewidgets

RC_ICONS = Resource/img/xinjiaoyuico.ico

QMAKE_TARGET_PRODUCT = "ZhiNengTiKa"

QMAKE_TARGET_COPYRIGHT = "Copyright © 2022 - 2023 LFWQSP2641.All Rights Reserved."

RC_LANG = 0x0004

RESOURCES += \
    Resource/Template.qrc

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

include(3rd/Qt-AES/QAESEncryption.pri)

HEADERS += \
    src/Logic/UpdateChecker.h \
    src/Logic/AnswerDetailData.hpp \
    src/Logic/MultipleSubjectsTemplateListModelList.h \
    src/Logic/NetworkAccessManagerBlockable.h \
    src/Logic/TemplateAnalysis.h \
    src/Logic/TemplateListModel.h \
    src/Logic/TemplateRawData.h \
    src/Logic/TemplateSearcher.h \
    src/Logic/TemplateSummary.h \
    src/Logic/UserData.h \
    src/Logic/UserDataList.hpp \
    src/Singleton/Network.h \
    src/StaticClass/CallAndroidNativeComponent.h \
    src/StaticClass/Global.h \
    src/StaticClass/QRCodeScanner.h \
    src/StaticClass/Setting.h \
    src/StaticClass/XinjiaoyuEncryptioner.h \
    src/StaticClass/XinjiaoyuNetwork.h

SOURCES += \
    src/Logic/UpdateChecker.cpp \
    src/Logic/MultipleSubjectsTemplateListModelList.cpp \
    src/Logic/NetworkAccessManagerBlockable.cpp \
    src/Logic/TemplateAnalysis.cpp \
    src/Logic/TemplateListModel.cpp \
    src/Logic/TemplateRawData.cpp \
    src/Logic/TemplateSearcher.cpp \
    src/Logic/TemplateSummary.cpp \
    src/Logic/UserData.cpp \
    src/Singleton/Network.cpp \
    src/StaticClass/CallAndroidNativeComponent.cpp \
    src/StaticClass/Global.cpp \
    src/StaticClass/QRCodeScanner.cpp \
    src/StaticClass/Setting.cpp \
    src/StaticClass/XinjiaoyuEncryptioner.cpp \
    src/StaticClass/XinjiaoyuNetwork.cpp

RESOURCES += Resource/Resource.qrc

android{
DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-xhdpi/icon.png \
    android/res/drawable-xxhdpi/icon.png \
    android/res/drawable-xxxhdpi/icon.png \
    android/res/values/libs.xml \
    android/src/com/LFWQSP2641/zhinengtika/CallNativeComponent.java \
    android/src/com/LFWQSP2641/zhinengtika/IntentActivity.java \
    android/src/com/LFWQSP2641/zhinengtika/Util.java \
    android/res/xml/provider_paths.xml
}
