#include "SearchWidget.h"
#include "TemplateListView.h"

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget{parent},
      mainLayout(new QVBoxLayout(this)),
      searchLineEdit(new QLineEdit(this)),
      stateShowLabel(new QLabel(this)),
      searchStartButton(new QPushButton(QStringLiteral("搜索"), this)),
      searchStopButton(new QPushButton(QStringLiteral("停止"), this)),
      OKButton(new QPushButton(QStringLiteral("确定"), this)),
      searchResultTemplateListView(new TemplateListView(this))
{
    searchLineEdit->setPlaceholderText(QStringLiteral("题卡名称"));
    this->setSearchingState(false);
    stateShowLabel->setFixedHeight(this->fontMetrics().height());
    stateShowLabel->setVisible(false);
    OKButton->setEnabled(false);
    searchResultTemplateListView->setTemplateListModel(&this->searchResultTemplateListModel);

    auto tempHBoxLayout{new QHBoxLayout};
    tempHBoxLayout->addWidget(searchLineEdit);
    tempHBoxLayout->addWidget(searchStartButton);
    tempHBoxLayout->addWidget(searchStopButton);
    this->mainLayout->addLayout(tempHBoxLayout);
    this->mainLayout->addWidget(stateShowLabel);
    this->mainLayout->addWidget(searchResultTemplateListView);
    this->mainLayout->addWidget(OKButton);

    connect(this->searchStartButton, &QPushButton::clicked, this, &SearchWidget::searchStartButtonPush);
    connect(this->searchStopButton, &QPushButton::clicked, this, &SearchWidget::searchStopButtonPush);
    connect(this->OKButton, &QPushButton::clicked, this, &SearchWidget::OKButtonPush);
    connect(this->searchResultTemplateListView, &TemplateListView::templateNameClicked, [this]
    {
        this->OKButton->setEnabled(true);
    });
    connect(&this->templateSearcher, &TemplateSearcher::searchResult, this, &SearchWidget::addToSearchResultListWidget);
    connect(&this->templateSearcher, &TemplateSearcher::searchFinished, this, &SearchWidget::templateSearcherSearchFinish);
}

void SearchWidget::setSearchingState(bool searching)
{
    this->OKButton->setEnabled(false);
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

void SearchWidget::searchStartButtonPush()
{
    this->searchResultTemplateListView->clear();
    this->stateShowLabel->setText(QStringLiteral("搜索中..."));
    this->stateShowLabel->setVisible(true);
    this->setSearchingState(true);
    templateSearcher.setSearchedTemplateName(searchLineEdit->text().trimmed());
    this->templateSearcher.start();
}

void SearchWidget::searchStopButtonPush()
{
    this->searchStopButton->setEnabled(false);
    this->stateShowLabel->setText(QStringLiteral("搜索停止中..."));
    this->templateSearcher.stop();
}

void SearchWidget::templateSearcherSearchStop()
{
    this->searchStopButton->setEnabled(true);
    this->stateShowLabel->setText(QStringLiteral("搜索已停止"));
    this->setSearchingState(false);
}

void SearchWidget::templateSearcherSearchFinish(bool success)
{
    if(success)
    {
        this->stateShowLabel->setText(QStringLiteral("搜索完成"));
        this->setSearchingState(false);
    }
    else
    {
        this->templateSearcherSearchStop();
    }
}

void SearchWidget::OKButtonPush()
{
    emit searchFinished(this->searchResultTemplateListView->getCurrentTemplateSummary());
}

void SearchWidget::addToSearchResultListWidget(TemplateSummary *templateSummary)
{
    this->searchResultTemplateListView->addNewTemplate(templateSummary);
}
