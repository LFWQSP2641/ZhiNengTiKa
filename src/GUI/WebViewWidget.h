#ifndef WEBVIEWWIDGET_H
#define WEBVIEWWIDGET_H

#include "../Logic/TemplateAnalysis.h"

class WebView: public QWebEngineView
{
    Q_OBJECT
public:
    using QWebEngineView::QWebEngineView;
    void setHtml(const QString &html)
    {
        this->html = html;
        this->QWebEngineView::setHtml(this->html);
    }
    QString getHtml() const
    {
        return this->html;
    }
protected:
    QString html;
};

class WebViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebViewWidget(TemplateAnalysis *templateAnalysis, QWidget *parent = nullptr);
    virtual QString getAnalyzedHtml(const qsizetype index = -1) = 0;

    TemplateAnalysis *getTemplateAnalysis() const;
    void setTemplateAnalysis(TemplateAnalysis *newTemplateAnalysis);

protected:
    QGridLayout *mainLayout;
    WebView *webView;
    QListWidget *pagesSwitch;

    QPushButton *openBrowserButton;

    qsizetype currentPageIndex = -1;
    QHash<QString, qsizetype> pageHash;

    bool templateAnalysisStateChanged = false;

    TemplateAnalysis *templateAnalysis;
    void showEvent(QShowEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void saveToFile(const QString &pathName);

protected slots:
    void analysis();
    void switchPage(QListWidgetItem *item);

signals:

    void templateAnalysisChanged();
private:
    Q_PROPERTY(TemplateAnalysis *templateAnalysis READ getTemplateAnalysis WRITE setTemplateAnalysis NOTIFY templateAnalysisChanged FINAL)
};

#endif // WEBVIEWWIDGET_H
