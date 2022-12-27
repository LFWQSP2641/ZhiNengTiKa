#pragma once

class AnalysisWebRawData;

class WebViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebViewWidget(AnalysisWebRawData &analysisWebRawData, QWidget *parent = nullptr);

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

    AnalysisWebRawData &analysisWebRawData;
    void showEvent(QShowEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void analysis();
    virtual QString getHtml(const qsizetype index = -1) = 0;
    void saveToFile(const QString &pathName);

protected slots:
    void switchPage(QListWidgetItem *item);

signals:

};

