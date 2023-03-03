#include "SearchWidget.h"

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget{parent},
      mainLayout(new QVBoxLayout(this)),
      searchLineEdit(new QLineEdit(this)),
      stateShowLabel(new QLabel(this)),
      searchStartButton(new QPushButton(QStringLiteral("搜索"), this)),
      searchStopButton(new QPushButton(QStringLiteral("停止"), this)),
      searchResultListWidget(new QListWidget(this)),
      OKButton(new QPushButton(QStringLiteral("确定"), this))
{
    searchLineEdit->setPlaceholderText(QStringLiteral("题卡名称"));
    this->setSearchingState(false);
    stateShowLabel->setFixedHeight(this->fontMetrics().height());
    stateShowLabel->setVisible(false);
    OKButton->setEnabled(false);

    QScroller::grabGesture(searchResultListWidget->viewport(), QScroller::TouchGesture);
    searchResultListWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    searchResultListWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    searchResultListWidget->setAutoScroll(false);

    auto tempHBoxLayout{new QHBoxLayout};
    tempHBoxLayout->addWidget(searchLineEdit);
    tempHBoxLayout->addWidget(searchStartButton);
    tempHBoxLayout->addWidget(searchStopButton);
    this->mainLayout->addLayout(tempHBoxLayout);
    this->mainLayout->addWidget(stateShowLabel);
    this->mainLayout->addWidget(searchResultListWidget);
    this->mainLayout->addWidget(OKButton);

    connect(this->searchStartButton, &QPushButton::clicked, this, &SearchWidget::searchStartButtonPushed);
    connect(this->searchStopButton, &QPushButton::clicked, this, &SearchWidget::searchStopButtonPushed);
    connect(this->OKButton, &QPushButton::clicked, this, &SearchWidget::OKButtonPushed);
    connect(this->searchResultListWidget, &QListWidget::itemClicked, [this]
    {
        this->OKButton->setEnabled(true);
    });
    connect(&this->templateSearcher, &TemplateSearcher::searchResult, this, &SearchWidget::addToSearchResultListWidget);
    connect(&this->templateSearcher, &TemplateSearcher::searchFinished, this, &SearchWidget::templateSearcherSearchFinished);
}

void SearchWidget::setSearchingState(bool searching)
{
    this->searchStartButton->setVisible(!searching);
    this->searchStopButton->setVisible(searching);
}

void SearchWidget::closeEvent(QCloseEvent *event)
{
    if(this->templateSearcher.isRunning())
    {
        this->templateSearcher.stop();
    }
    this->templateSearcher.wait();
    QWidget::closeEvent(event);
    event->accept();
}

void SearchWidget::searchStartButtonPushed()
{
    this->searchResultListWidget->clear();
    this->templateCodeList.clear();
    this->stateShowLabel->setText(QStringLiteral("搜索中..."));
    this->stateShowLabel->setVisible(true);
    this->setSearchingState(true);
    templateSearcher.setSearchedTemplateName(searchLineEdit->text().trimmed());
    this->templateSearcher.start();
}

void SearchWidget::searchStopButtonPushed()
{
    this->searchStopButton->setEnabled(false);
    this->stateShowLabel->setText(QStringLiteral("搜索停止中..."));
    this->templateSearcher.stop();
}

void SearchWidget::templateSearcherSearchStoped()
{
    this->searchStopButton->setEnabled(true);
    this->stateShowLabel->setText(QStringLiteral("搜索已停止"));
    this->setSearchingState(false);
}

void SearchWidget::templateSearcherSearchFinished(bool success)
{
    if(success)
    {
        this->stateShowLabel->setText(QStringLiteral("搜索完成"));
        this->setSearchingState(false);
    }
    else
    {
        this->templateSearcherSearchStoped();
    }
}

void SearchWidget::OKButtonPushed()
{
    const auto currentSearchResultListWidgetItem{this->searchResultListWidget->currentItem()};
    if(currentSearchResultListWidgetItem != nullptr)
    {
        emit searchFinished(
            this->templateCodeList.at(this->searchResultListWidget->row(
                                          currentSearchResultListWidgetItem)));
    }
}

void SearchWidget::addToSearchResultListWidget(const QString &templateName, const QString &templateCode)
{
//    auto item{new QListWidgetItem(templateName,this->searchResultListWidget)};
//    hash.insert(item)
    this->searchResultListWidget->addItem(templateName);
    templateCodeList.append(templateCode);
}
