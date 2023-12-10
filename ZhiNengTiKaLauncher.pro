VERSION = 3.0.0

TEMPLATE = app

# QML
QT += widgets gui quick quickcontrols2 webview sensors
# common
QT += core network multimedia core5compat
android{
QT += core-private
}

android{

ANDROID_VERSION_NAME = $$VERSION

include($$PWD/3rd/android_openssl/openssl.pri)

ANDROID_PACKAGE_SOURCE_DIR = \
    $$PWD/android

}
win32{

RC_ICONS = Resource/img/xinjiaoyuico.ico

QMAKE_TARGET_PRODUCT = "ZhiNengTiKa"

QMAKE_TARGET_COPYRIGHT = "Copyright © 2022 - 2023 LFWQSP2641.All Rights Reserved."

RC_LANG = 0x0004

}

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += DATABASE_DOMAIN=\\\"https://gitee.com/LFWQSP2641/zhinengtika_database/raw/master/\\\"
DEFINES *= QT_USE_QSTRINGBUILDER

CONFIG += c++17 precompile_header

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/3rd
include($$PWD/3rd/Qt-AES/QAESEncryption.pri)

QML_IMPORT_PATH = $$PWD/Resource/qml

PRECOMPILED_HEADER = src/pchQML.h

HEADERS += \
    $$PWD/src/Logic/NetworkAccessManagerBlockable.h \
    $$PWD/src/Logic/UpdateChecker.h \
    $$PWD/src/Singleton/Network.h \
    $$PWD/src/StaticClass/CallAndroidNativeComponent.h \
    $$PWD/src/StaticClass/Global.h \
    src/Logic/LibraryUpdateChecker.h

SOURCES += \
    $$PWD/src/Logic/NetworkAccessManagerBlockable.cpp \
    $$PWD/src/Logic/UpdateChecker.cpp \
    $$PWD/src/Singleton/Network.cpp \
    $$PWD/src/StaticClass/CallAndroidNativeComponent.cpp \
    $$PWD/src/StaticClass/Global.cpp \
    $$PWD/src/mainLauncher.cpp \
    src/Logic/LibraryUpdateChecker.cpp

android{
DISTFILES += \
    $$PWD/android/AndroidManifest.xml \
    $$PWD/android/build.gradle \
    $$PWD/android/gradle.properties \
    $$PWD/android/gradle/wrapper/gradle-wrapper.jar \
    $$PWD/android/gradle/wrapper/gradle-wrapper.properties \
    $$PWD/android/gradlew \
    $$PWD/android/gradlew.bat \
    $$PWD/android/res/drawable-hdpi/icon.png \
    $$PWD/android/res/drawable-ldpi/icon.png \
    $$PWD/android/res/drawable-mdpi/icon.png \
    $$PWD/android/res/drawable-xhdpi/icon.png \
    $$PWD/android/res/drawable-xxhdpi/icon.png \
    $$PWD/android/res/drawable-xxxhdpi/icon.png \
    $$PWD/android/res/values/libs.xml \
    $$PWD/android/src/com/LFWQSP2641/zhinengtika/CallNativeComponent.java \
    $$PWD/android/src/com/LFWQSP2641/zhinengtika/IntentActivity.java \
    $$PWD/android/src/com/LFWQSP2641/zhinengtika/ToastUtils.java \
    $$PWD/android/src/com/LFWQSP2641/zhinengtika/Util.java \
    $$PWD/android/res/xml/provider_paths.xml
}

DISTFILES += \
