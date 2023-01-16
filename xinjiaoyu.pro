win32:VERSION = 1.4.0.28
else:VERSION = 1.4.0


android{

QT       += core widgets gui network core-private

ANDROID_VERSION_NAME = $$VERSION

RESOURCES += Resource/Resource_Android.qrc

include(D:/Softwares/scr/AndroidSDK/android_openssl/openssl.pri)

ANDROID_PACKAGE_SOURCE_DIR = \
    $$PWD/android

}
win32{

QT       += core widgets gui network

RC_ICONS = Resource/img/xinjiaoyuico.ico

QMAKE_TARGET_PRODUCT = "xinjiaoyu"

QMAKE_TARGET_COPYRIGHT = "Copyright © 2022 - 2023 LFWQSP2641.All Rights Reserved."

RC_LANG = 0x0004

RESOURCES += Resource/Resource.qrc

}

CONFIG += webview

#DEFINES += LIMITED

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DEFINES *= QT_USE_QSTRINGBUILDER

CONFIG += c++17 precompile_header

PRECOMPILED_HEADER = src/pch.h

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(D:/Softwares/scr/Qt-AES/QAESEncryption.pri)
include(D:/Softwares/scr/WebView/WebView.pri)

HEADERS += \
    src/GUI/AnswerAndAnalysisWidget.h \
    src/GUI/ClickableLabel.hpp \
    src/GUI/MainWidget.h \
    src/GUI/PixmapLabel.h \
    src/GUI/PlusSignLabel.h \
    src/GUI/QuestionWidget.h \
    src/GUI/SearchWidget.h \
    src/GUI/SettingWidget.h \
    src/GUI/ShowFullScreenWidget.h \
    src/GUI/UploadChildWidget.h \
    src/GUI/UploadWidget.h \
    src/GUI/WebViewWidget.h \
    src/Logic/AnalysisWebRawData.h \
    src/Logic/AnswerDetailData.hpp \
    src/Logic/AutoUpdate.h \
    src/Logic/UserData.hpp \
    src/Logic/UserDataList.hpp \
    src/StaticClass/CallAndroidNativeComponent.h \
    src/StaticClass/Global.h \
    src/StaticClass/Login.h \
    src/StaticClass/Network.h \
    src/StaticClass/QRCodeScanner.h \
    src/StaticClass/Setting.h \
    src/StaticClass/XinjiaoyuEncryptioner.h \
    src/StaticClass/XinjiaoyuNetwork.h

SOURCES += \
    src/GUI/AnswerAndAnalysisWidget.cpp \
    src/GUI/MainWidget.cpp \
    src/GUI/PixmapLabel.cpp \
    src/GUI/PlusSignLabel.cpp \
    src/GUI/QuestionWidget.cpp \
    src/GUI/SearchWidget.cpp \
    src/GUI/SettingWidget.cpp \
    src/GUI/ShowFullScreenWidget.cpp \
    src/GUI/UploadChildWidget.cpp \
    src/GUI/UploadWidget.cpp \
    src/GUI/WebViewWidget.cpp \
    src/Logic/AnalysisWebRawData.cpp \
    src/Logic/AutoUpdate.cpp \
    src/StaticClass/CallAndroidNativeComponent.cpp \
    src/StaticClass/Global.cpp \
    src/StaticClass/Login.cpp \
    src/StaticClass/Network.cpp \
    src/StaticClass/QRCodeScanner.cpp \
    src/StaticClass/Setting.cpp \
    src/StaticClass/XinjiaoyuEncryptioner.cpp \
    src/StaticClass/XinjiaoyuNetwork.cpp \
    src/main.cpp \
    src/pch.h.cpp

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
    android/src/com/LFWQSP2641/xinjiaoyu/IntentActivity.java
}

DISTFILES += \
    android/res/xml/provider_paths.xml \
    android/src/com/LFWQSP2641/xinjiaoyu/CallNativeComponent.java \
    android/src/com/LFWQSP2641/xinjiaoyu/Util.java
