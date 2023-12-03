TEMPLATE = app
CONFIG += c++17 precompile_header
CONFIG -= app_bundle

SOURCES += \
        src/main_loader.cpp

VERSION = 2.6.4

QT += widgets gui quick quickcontrols2 webview sensors

QT += core network multimedia core5compat

android{

QT += core-private

ANDROID_VERSION_NAME = $$VERSION

include($$PWD/3rd/android_openssl/openssl.pri)

ANDROID_PACKAGE_SOURCE_DIR = \
    $$PWD/android

}

PRECOMPILED_HEADER = src/pchQML.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include($$PWD/3rd/Qt-AES/QAESEncryption.pri)

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
    $$PWD/android/src/com/LFWQSP2641/zhinengtika/Util.java \
    $$PWD/android/res/xml/provider_paths.xml
}
