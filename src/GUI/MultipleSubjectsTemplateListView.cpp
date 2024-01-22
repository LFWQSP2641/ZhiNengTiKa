#include "MultipleSubjectsTemplateListView.h"
#include "TemplateListView.h"

MultipleSubjectsTemplateListView::MultipleSubjectsTemplateListView(QWidget *parent)
    : QWidget{parent},
      mainLayout(new QVBoxLayout(this)),
      multipleSubjectsTabBar(new QTabBar(this)),
      templateListView(new TemplateListView(this))
{
    const QStringList subjects({QStringLiteral("用户历史"),
                                QStringLiteral("语文"), QStringLiteral("数学"),
                                QStringLiteral("英语"), QStringLiteral("物理"),
                                QStringLiteral("化学"), QStringLiteral("生物")});
    for(const auto &subjectName : subjects)
    {
        multipleSubjectsTabBar->addTab(subjectName);
    }
    multipleSubjectsTabBar->setDrawBase(false);
    multipleSubjectsTabBar->setElideMode(Qt::ElideRight);

    templateListView->setTemplateListModel(this->multipleSubjectsTemplateListModelList.at(0));

    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(multipleSubjectsTabBar);
    mainLayout->addWidget(templateListView);

    connect(multipleSubjectsTabBar, &QTabBar::currentChanged, [this](int index)
    {
        if(index > -1)
        {
            templateListView->setTemplateListModel(this->multipleSubjectsTemplateListModelList.at(index));
        }
    });
    connect(templateListView, &TemplateListView::templateNameClicked, this, &MultipleSubjectsTemplateListView::templateNameClicked);
}

void MultipleSubjectsTemplateListView::addNewTemplate(const TemplateSummary &templateSummary)
{
    this->multipleSubjectsTemplateListModelList.addNewTemplate(templateSummary);
    this->multipleSubjectsTabBar->setCurrentIndex(MultipleSubjectsTemplateListModelList::Subjects::UserHistory);
    this->templateListView->setCurrentIndex(
        this->multipleSubjectsTemplateListModelList
        .at(MultipleSubjectsTemplateListModelList::Subjects::UserHistory)
        ->index(this->multipleSubjectsTemplateListModelList
                .at(MultipleSubjectsTemplateListModelList::Subjects::
                    UserHistory)
                ->rowCount() - 1));
}

MultipleSubjectsTemplateListModelList &MultipleSubjectsTemplateListView::getMultipleSubjectsTemplateListModelList()
{
    return multipleSubjectsTemplateListModelList;
}

const TemplateSummary &MultipleSubjectsTemplateListView::getCurrentTemplateSummary()
{
    return this->templateListView->getCurrentTemplateSummary();
}
