#include "WebViewWidget.h"
#include "../Logic/AnalysisWebRawData.h"
#include "../StaticClass/Global.h"
#include "../StaticClass/Setting.h"


WebViewWidget::WebViewWidget(AnalysisWebRawData &analysisWebRawData, QWidget *parent)
    : QWidget{parent}, analysisWebRawData(analysisWebRawData)
{
    mainLayout = new QGridLayout(this);
    webView = new WebView(this);
    pagesSwitch = new QListWidget(this);
#ifdef Q_OS_ANDROID
    fullScreenButton = new QPushButton("全屏", this);
#else
    saveButton = new QPushButton("另存为", this);
    connect(saveButton, &QPushButton::clicked, [this]
    {
        saveToFile(QFileDialog::getSaveFileName(this, QStringLiteral("Save File"), QStringLiteral(".") + QDir::separator() + this->analysisWebRawData.getTemplateName(), QStringLiteral("Hyper Text Markup Language (*.html *.htm)")));
    });
#endif // Q_OS_ANDROID

    QFont smallFont;
    const auto defaultFontPixelSize{QWidget::font().pixelSize()};
    if(Setting::smallFontPointSize != 0)
    {
        smallFont.setPointSize(Setting::smallFontPointSize);
    }
    else
    {
        smallFont.setPixelSize(defaultFontPixelSize / 2);
    }
    pagesSwitch->setFont(smallFont);

    pagesSwitch->setFixedHeight(pagesSwitch->fontMetrics().height() * 3 / 2);
    pagesSwitch->setFlow(QListView::LeftToRight);
    pagesSwitch->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pagesSwitch->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pagesSwitch->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    pagesSwitch->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    QScroller::grabGesture(pagesSwitch->viewport(), QScroller::TouchGesture);

    this->setEnabled(false);

#ifdef Q_OS_ANDROID
    mainLayout->addWidget(fullScreenButton, 0, 1, 1, 1);
#else
#ifndef LIMITED
    mainLayout->addWidget(saveButton, 0, 1, 1, 1);
#endif // LIMITED
#endif // Q_OS_ANDROID
    mainLayout->addWidget(pagesSwitch, 1, 0, 1, 2);
    mainLayout->addWidget(webView, 2, 0, 1, 2);

    connect(&analysisWebRawData, &AnalysisWebRawData::analysisFinished, [this]
    {
        if(this->isVisible())
        {
            analysis();
            analysisWebRawDataStateChanged = false;
        }
        else
        {
            analysisWebRawDataStateChanged = true;
        }
    });
    connect(pagesSwitch, &QListWidget::itemPressed, this, &WebViewWidget::switchPage);
}

void WebViewWidget::showEvent(QShowEvent *event)
{
    if(analysisWebRawDataStateChanged)
    {
        analysis();
        analysisWebRawDataStateChanged = false;
    }
    event->accept();
}

void WebViewWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Copy))
    {
        const auto path{Global::tempPath() + QDir::separator() + analysisWebRawData.getTemplateName() + QStringLiteral(".html")};
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
    pageHash.clear();
    currentPageIndex = -1;
    pagesSwitch->clear();
    pagesSwitch->horizontalScrollBar()->setSliderPosition(0);
    pagesSwitch->verticalScrollBar()->setSliderPosition(0);
    const auto allItem{new QListWidgetItem(QStringLiteral("ALL"), pagesSwitch)};
    allItem->setSelected(true);
    const auto pageStrList{analysisWebRawData.getQuestionsCountsStrList()};
    pagesSwitch->addItems(pageStrList);
    pageHash.insert(QStringLiteral("ALL"), -1);
    for(auto i{0}; i < pageStrList.size(); ++i)
    {
        pageHash.insert(pageStrList.at(i), i);
    }
    webView->setHtml(getHtml());
    this->setEnabled(true);
}

void WebViewWidget::saveToFile(const QString &pathName)
{
    if(analysisWebRawData.isEmpty() || pathName.isEmpty())
    {
        return;
    }
    const auto fileData{webView->getHtml().prepend(QStringLiteral("<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title").append(analysisWebRawData.getTemplateName()).append("</title></head><body>")).append("</body></html>").toUtf8()};
    QFile file(pathName);
    file.open(QFile::WriteOnly);
    file.write(fileData);
    file.close();
}

void WebViewWidget::switchPage(QListWidgetItem *item)
{
    currentPageIndex = pageHash.value(item->text());
    webView->setHtml(getHtml(currentPageIndex));
}
