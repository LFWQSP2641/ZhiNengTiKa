include(./ZhiNengTiKaCore.pri)

QT       += widgets gui

PRECOMPILED_HEADER = src/pch.h

HEADERS += \
    src/GUI/AnswerAndAnalysisWidget.h \
    src/GUI/ClickableLabel.hpp \
    src/GUI/MainWidget.h \
    src/GUI/MultipleSubjectsTemplateListView.h \
    src/GUI/NavigationBarTabWidget.h \
    src/GUI/PixmapLabel.h \
    src/GUI/PlusSignLabel.h \
    src/GUI/QuestionWidget.h \
    src/GUI/SearchWidget.h \
    src/GUI/SelectWidget.h \
    src/GUI/SettingWidget.h \
    src/GUI/TemplateDetailWidget.h \
    src/GUI/TemplateListView.h \
    src/GUI/UploadChildWidget.h \
    src/GUI/UploadWidget.h \
    src/GUI/WebViewWidget.h \

SOURCES += \
    src/GUI/AnswerAndAnalysisWidget.cpp \
    src/GUI/MainWidget.cpp \
    src/GUI/MultipleSubjectsTemplateListView.cpp \
    src/GUI/NavigationBarTabWidget.cpp \
    src/GUI/PixmapLabel.cpp \
    src/GUI/PlusSignLabel.cpp \
    src/GUI/QuestionWidget.cpp \
    src/GUI/SearchWidget.cpp \
    src/GUI/SelectWidget.cpp \
    src/GUI/SettingWidget.cpp \
    src/GUI/TemplateDetailWidget.cpp \
    src/GUI/TemplateListView.cpp \
    src/GUI/UploadChildWidget.cpp \
    src/GUI/UploadWidget.cpp \
    src/GUI/WebViewWidget.cpp \
    src/main.cpp \
    src/pch.h.cpp