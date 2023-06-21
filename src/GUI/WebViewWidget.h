#ifndef WEBVIEWWIDGET_H
#define WEBVIEWWIDGET_H

#include "../Logic/TemplateAnalysis.h"
#include "../Logic/ImageProvider.h"

class WebView: public QTextBrowser
{
    Q_OBJECT
public:
    using QTextBrowser::QTextBrowser;
    void setHtml(const QString &html)
    {
        this->html = imageProvider.loadHtml(html);
        this->QTextBrowser::setHtml(this->html);
    }
    QString getHtml() const
    {
        return html;
    }
protected:
    QString html;
    ImageProvider imageProvider;
};

class WebViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WebViewWidget(QSharedPointer<TemplateAnalysis> templateAnalysis = QSharedPointer<TemplateAnalysis>(new TemplateAnalysis), QWidget *parent = nullptr);
    virtual QString getAnalyzedHtml(const qsizetype index = -1) = 0;

protected:
    QGridLayout *mainLayout;
    WebView *webView;
    QListWidget *pagesSwitch;

    QPushButton *saveButton;

    qsizetype currentPageIndex = -1;
    QHash<QString, qsizetype> pageHash;

    bool templateAnalysisStateChanged = false;

    QSharedPointer<TemplateAnalysis> templateAnalysisPointer;
    void showEvent(QShowEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void setTemplateAnalysis(QSharedPointer<TemplateAnalysis> newTemplateAnalysis);
    void saveToFile(const QString &pathName);

protected slots:
    void analysis();
    void switchPage(QListWidgetItem *item);

signals:

};

#endif // WEBVIEWWIDGET_H
