include($$PWD/../precompile_header/precompile_header.pri)
include($$PWD/../ZhiNengTiKaCommon.pri)

TEMPLATE = lib

QT += core network multimedia
android: QT += core-private

#DEFINES += LIMITED

DEFINES += ZHINENGTIKACORE_LIBRARY
DEFINES += ZHINENGTIKACORE

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/../3rd/zxing-cpp/core/src

HEADERS += \
    $$PWD/src/Logic/AccountManager.h \
    $$PWD/src/Logic/AnnouncementManager.h \
    $$PWD/src/Logic/AnnouncementModel.h \
    $$PWD/src/Logic/AnswerDetailData.hpp \
    $$PWD/src/Logic/ImageProvider.h \
    $$PWD/src/Logic/ImageProviderMultiThreading.h \
    $$PWD/src/Logic/NetworkAccessManagerBlockable.h \
    $$PWD/src/Logic/QRCodeReader.h \
    $$PWD/src/Logic/QRCodeScanner.h \
    $$PWD/src/Logic/ResourceFileFetcher.h \
    $$PWD/src/Logic/ResourceFileModel.h \
    $$PWD/src/Logic/TemplateHandle/TemplateAnalysis.h \
    $$PWD/src/Logic/TemplateHandle/TemplateData.h \
    $$PWD/src/Logic/TemplateHandle/TemplateFetcher.h \
    $$PWD/src/Logic/TemplateHandle/TemplateSummary.h \
    $$PWD/src/Logic/TemplateHandle/ListModel/MultipleSubjectsTemplateListModelList.h \
    $$PWD/src/Logic/TemplateHandle/ListModel/TemplateListModel.h \
    $$PWD/src/Logic/TemplateHandle/TemplateModel/TemplateDataTreeModel.h \
    $$PWD/src/Logic/TemplateHandle/UploadModel/TemplateAnswerData.h \
    $$PWD/src/Logic/TemplateHandle/UploadModel/TemplateAnswerTreeModel.h \
    $$PWD/src/Logic/TemplateSearcher.h \
    $$PWD/src/Logic/UserData.h \
    $$PWD/src/Logic/Version.h \
    $$PWD/src/Singleton/MultipleSubjectsTemplateListModelListSingleton.h \
    $$PWD/src/Singleton/Network.h \
    $$PWD/src/Singleton/Settings.h \
    $$PWD/src/StaticClass/CallAndroidNativeComponent.h \
    $$PWD/src/StaticClass/Global.h \
    $$PWD/src/StaticClass/XinjiaoyuEncryptioner.h \
    $$PWD/src/StaticClass/XinjiaoyuNetwork.h \
    $$PWD/src/ZhiNengTiKaCore_global.h \
    $$PWD/src/ZxingCpp/ZXingPosition.h \
    $$PWD/src/ZxingCpp/ZXingReader.h \
    $$PWD/src/ZxingCpp/ZXingResult.h

SOURCES += \
    $$PWD/src/Logic/AccountManager.cpp \
    $$PWD/src/Logic/AnnouncementManager.cpp \
    $$PWD/src/Logic/AnnouncementModel.cpp \
    $$PWD/src/Logic/ImageProvider.cpp \
    $$PWD/src/Logic/ImageProviderMultiThreading.cpp \
    $$PWD/src/Logic/QRCodeReader.cpp \
    $$PWD/src/Logic/QRCodeScanner.cpp \
    $$PWD/src/Logic/NetworkAccessManagerBlockable.cpp \
    $$PWD/src/Logic/ResourceFileFetcher.cpp \
    $$PWD/src/Logic/ResourceFileModel.cpp \
    $$PWD/src/Logic/TemplateHandle/TemplateAnalysis.cpp \
    $$PWD/src/Logic/TemplateHandle/TemplateData.cpp \
    $$PWD/src/Logic/TemplateHandle/TemplateFetcher.cpp \
    $$PWD/src/Logic/TemplateHandle/TemplateSummary.cpp \
    $$PWD/src/Logic/TemplateHandle/ListModel/MultipleSubjectsTemplateListModelList.cpp \
    $$PWD/src/Logic/TemplateHandle/ListModel/TemplateListModel.cpp \
    $$PWD/src/Logic/TemplateHandle/TemplateModel/TemplateDataTreeModel.cpp \
    $$PWD/src/Logic/TemplateHandle/UploadModel/TemplateAnswerData.cpp \
    $$PWD/src/Logic/TemplateHandle/UploadModel/TemplateAnswerTreeModel.cpp \
    $$PWD/src/Logic/TemplateSearcher.cpp \
    $$PWD/src/Logic/UserData.cpp \
    $$PWD/src/Logic/Version.cpp \
    $$PWD/src/Singleton/MultipleSubjectsTemplateListModelListSingleton.cpp \
    $$PWD/src/Singleton/Network.cpp \
    $$PWD/src/Singleton/Settings.cpp \
    $$PWD/src/StaticClass/CallAndroidNativeComponent.cpp \
    $$PWD/src/StaticClass/Global.cpp \
    $$PWD/src/StaticClass/XinjiaoyuEncryptioner.cpp \
    $$PWD/src/StaticClass/XinjiaoyuNetwork.cpp \
    $$PWD/src/ZxingCpp/ZXingPosition.cpp \
    $$PWD/src/ZxingCpp/ZXingReader.cpp \
    $$PWD/src/ZxingCpp/ZXingResult.cpp

RESOURCES += \
    $$PWD/Resource/Resource.qrc \
    $$PWD/Resource/Template.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../3rd_build/Qt-AES/release/ -lqt_aes
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../3rd_build/Qt-AES/debug/ -lqt_aes
else:unix: LIBS += -L$$OUT_PWD/../3rd_build/Qt-AES/ -lqt_aes_$${QT_ARCH}

INCLUDEPATH += $$PWD/../3rd_build/Qt-AES
DEPENDPATH += $$PWD/../3rd_build/Qt-AES

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rd_build/Qt-AES/release/libqt_aes.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rd_build/Qt-AES/debug/libqt_aes.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rd_build/Qt-AES/release/qt_aes.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rd_build/Qt-AES/debug/qt_aes.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../3rd_build/Qt-AES/libqt_aes_$${QT_ARCH}.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../3rd_build/zxing-cpp/release/ -lzxing
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../3rd_build/zxing-cpp/debug/ -lzxing
else:unix: LIBS += -L$$OUT_PWD/../3rd_build/zxing-cpp/ -lzxing_$${QT_ARCH}

INCLUDEPATH += $$PWD/../3rd_build/zxing-cpp
DEPENDPATH += $$PWD/../3rd_build/zxing-cpp

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rd_build/zxing-cpp/release/libzxing.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rd_build/zxing-cpp/debug/libzxing.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rd_build/zxing-cpp/release/zxing.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../3rd_build/zxing-cpp/debug/zxing.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../3rd_build/zxing-cpp/libzxing_$${QT_ARCH}.a
