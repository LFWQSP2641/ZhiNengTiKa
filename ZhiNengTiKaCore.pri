VERSION = 2.4.0

QT       += core network multimedia

android{
QT       += core-private

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

RESOURCES += \
    $$PWD/Resource/Template.qrc

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

win32:LIBS += -L$$PWD/lib/zxing-cpp/LibZXingCpp/windows-msvc2019-64bit -lZXingCpp
android:ANDROID_EXTRA_LIBS += $$PWD/lib/zxing-cpp/LibZXingCpp/android/libZXingCpp_arm64-v8a.so
#android:ANDROID_EXTRA_LIBS += $$PWD/lib/zxing-cpp/LibZXingCpp/android/libZXingCpp_armeabi-v7a.so
#android:ANDROID_EXTRA_LIBS += $$PWD/lib/zxing-cpp/LibZXingCpp/android/libZXingCpp_x86.so
#android:ANDROID_EXTRA_LIBS += $$PWD/lib/zxing-cpp/LibZXingCpp/android/libZXingCpp_x86_64.so

android:LIBS += -L$$PWD/lib/zxing-cpp/LibZXingCpp/android/ -lZXingCpp_arm64-v8a
#android:LIBS += -L$$PWD/lib/zxing-cpp/LibZXingCpp/android/ -lZXingCpp_armeabi-v7a
#android:LIBS += -L$$PWD/lib/zxing-cpp/LibZXingCpp/android/ -lZXingCpp_x86
#android:LIBS += -L$$PWD/lib/zxing-cpp/LibZXingCpp/android/ -lZXingCpp_x86_64

INCLUDEPATH += $$PWD/lib/zxing-cpp
DEPENDPATH += $$PWD/lib/zxing-cpp

HEADERS += \
    $$PWD/src/Logic/AnswerDetailData.hpp \
    $$PWD/src/Logic/ImageProvider.h \
    $$PWD/src/Logic/MultipleSubjectsTemplateListModelList.h \
    $$PWD/src/Logic/NetworkAccessManagerBlockable.h \
    $$PWD/src/Logic/QRCodeReader.h \
    $$PWD/src/Logic/QRCodeScanner.h \
    $$PWD/src/Logic/TemplateAnalysis.h \
    $$PWD/src/Logic/TemplateListModel.h \
    $$PWD/src/Logic/TemplateRawData.h \
    $$PWD/src/Logic/TemplateSearcher.h \
    $$PWD/src/Logic/TemplateSummary.h \
    $$PWD/src/Logic/UpdateChecker.h \
    $$PWD/src/Logic/UserData.h \
    $$PWD/src/Logic/UserDataList.hpp \
    $$PWD/src/Singleton/Network.h \
    $$PWD/src/Singleton/Settings.h \
    $$PWD/src/StaticClass/CallAndroidNativeComponent.h \
    $$PWD/src/StaticClass/Global.h \
    $$PWD/src/StaticClass/XinjiaoyuEncryptioner.h \
    $$PWD/src/StaticClass/XinjiaoyuNetwork.h

SOURCES += \
    $$PWD/src/Logic/ImageProvider.cpp \
    $$PWD/src/Logic/QRCodeReader.cpp \
    $$PWD/src/Logic/QRCodeScanner.cpp \
    $$PWD/src/Logic/MultipleSubjectsTemplateListModelList.cpp \
    $$PWD/src/Logic/NetworkAccessManagerBlockable.cpp \
    $$PWD/src/Logic/TemplateAnalysis.cpp \
    $$PWD/src/Logic/TemplateListModel.cpp \
    $$PWD/src/Logic/TemplateRawData.cpp \
    $$PWD/src/Logic/TemplateSearcher.cpp \
    $$PWD/src/Logic/TemplateSummary.cpp \
    $$PWD/src/Logic/UpdateChecker.cpp \
    $$PWD/src/Logic/UserData.cpp \
    $$PWD/src/Singleton/Network.cpp \
    $$PWD/src/Singleton/Settings.cpp \
    $$PWD/src/StaticClass/CallAndroidNativeComponent.cpp \
    $$PWD/src/StaticClass/Global.cpp \
    $$PWD/src/StaticClass/XinjiaoyuEncryptioner.cpp \
    $$PWD/src/StaticClass/XinjiaoyuNetwork.cpp

RESOURCES += $$PWD/Resource/Resource.qrc

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
