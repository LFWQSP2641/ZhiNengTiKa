#include "WebViewWidget.h"
#include "../StaticClass/Global.h"

WebViewWidget::WebViewWidget(const TemplateAnalysis &templateAnalysis, QWidget *parent)
    : QWidget{parent}, templateAnalysis(templateAnalysis)
{
    mainLayout = new QGridLayout(this);
    webView = new WebView(this);
    pagesSwitch = new QListWidget(this);
    openBrowserButton = new QPushButton("使用浏览器打开", this);
    connect(openBrowserButton, &QPushButton::clicked, [this]
    {
        const QString path{Global::tempPath().append(QStringLiteral("/")).append(QStringLiteral("temp.html"))};
        saveToFile(path);
        QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
    });

    pagesSwitch->setFixedHeight(pagesSwitch->fontMetrics().height() * 3 / 2);
    pagesSwitch->setFlow(QListView::LeftToRight);
    pagesSwitch->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pagesSwitch->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pagesSwitch->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    pagesSwitch->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    QScroller::grabGesture(pagesSwitch->viewport(), QScroller::TouchGesture);

#ifndef LIMITED
    mainLayout->addWidget(openBrowserButton, 0, 1, 1, 1);
#endif // LIMITED
    mainLayout->addWidget(pagesSwitch, 1, 0, 1, 2);
    mainLayout->addWidget(webView, 2, 0, 1, 2);

    connect(pagesSwitch, &QListWidget::itemPressed, this, &WebViewWidget::switchPage);

    this->templateAnalysisStateChanged = true;
}

const TemplateAnalysis &WebViewWidget::getTemplateAnalysis() const
{
    return templateAnalysis;
}

void WebViewWidget::setTemplateAnalysis(const TemplateAnalysis &newTemplateAnalysis)
{
    if (templateAnalysis == newTemplateAnalysis)
        return;
    this->templateAnalysisStateChanged = true;
    templateAnalysis = newTemplateAnalysis;
    emit templateAnalysisChanged();
}

void WebViewWidget::showEvent(QShowEvent *event)
{
    if(templateAnalysisStateChanged)
    {
        analysis();
        templateAnalysisStateChanged = false;
    }
    event->accept();
}

void WebViewWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Copy))
    {
        const QString path{Global::tempPath().append(QStringLiteral("/")).append(templateAnalysis.getTemplateName()).append(QStringLiteral(".html"))};
        saveToFile(path);

        QList<QUrl> copyfile;
        QUrl url = QUrl::fromLocalFile(path);
        copyfile.append(url);
        QMimeData *data = new QMimeData;
        data->setUrls(copyfile);

        QClipboard *clip = QApplication::clipboard();
        clip->setMimeData(data);
    }
    event->accept();
}

void WebViewWidget::analysis()
{
    if(!this->templateAnalysis.getValid())
    {
        return;
    }
    pageHash.clear();
    currentPageIndex = -1;
    pagesSwitch->clear();
    pagesSwitch->horizontalScrollBar()->setSliderPosition(0);
    pagesSwitch->verticalScrollBar()->setSliderPosition(0);
    const auto allItem{new QListWidgetItem(QStringLiteral("ALL"), pagesSwitch)};
    allItem->setSelected(true);
    const auto pageStrList{templateAnalysis.getQuestionsCountsStrList()};
    pagesSwitch->addItems(pageStrList);
    pageHash.insert(QStringLiteral("ALL"), -1);
    for(auto i{0}; i < pageStrList.size(); ++i)
    {
        pageHash.insert(pageStrList.at(i), i);
    }
    webView->setHtml(getAnalyzedHtml());
}

void WebViewWidget::saveToFile(const QString &pathName)
{
    if(!this->templateAnalysis.getValid() || pathName.isEmpty())
    {
        return;
    }
    const auto fileData
    {
        QStringLiteral(
            "<!DOCTYPE html><html><head><meta charset=\"utf-8\">"
            "<title>%0</title>"
            "</head><body>%1"
            "<footer>"
            "<p>本文件由 ZhiNengTiKa 自动生成</p>"
            "<p>软件仓库: <a href=\"%2\">%2</a>.</p>"
            "</footer></body></html>")
        .arg(templateAnalysis.getTemplateName(),
             getAnalyzedHtml(currentPageIndex),
             QStringLiteral("https://github.com/LFWQSP2641/ZhiNengTiKa"))};
    QFile file(pathName);
    file.open(QFile::WriteOnly);
    file.write(fileData.toUtf8());
    file.close();
}

void WebViewWidget::switchPage(QListWidgetItem *item)
{
    currentPageIndex = pageHash.value(item->text());
    webView->setHtml(getAnalyzedHtml(currentPageIndex));
}
