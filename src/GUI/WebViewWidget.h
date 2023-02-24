#pragma once

#include "../Logic/AnalysisWebRawData.h"

class WebViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebViewWidget(const AnalysisWebRawData &analysisWebRawData = AnalysisWebRawData(), QWidget *parent = nullptr);
    void setAnalysisWebRawData(const AnalysisWebRawData &analysisWebRawData);

protected:
    QGridLayout *mainLayout;
    WebView *webView;
    QListWidget *pagesSwitch;

#ifdef Q_OS_ANDROID
    QPushButton *fullScreenButton;
#else
    QPushButton *saveButton;
#endif // Q_OS_ANDROID

    qsizetype currentPageIndex = -1;
    QHash<QString, qsizetype> pageHash;

    bool analysisWebRawDataStateChanged = false;

    AnalysisWebRawData analysisWebRawData;
    void showEvent(QShowEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void analysis();
    virtual QString getAnalyzedHtml(const qsizetype index = -1) = 0;
    void saveToFile(const QString &pathName);

protected slots:
    void switchPage(QListWidgetItem *item);

signals:

};

