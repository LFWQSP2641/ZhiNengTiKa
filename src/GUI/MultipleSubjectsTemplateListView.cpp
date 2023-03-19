#include "MultipleSubjectsTemplateListView.h"
#include "TemplateListView.h"

MultipleSubjectsTemplateListView::MultipleSubjectsTemplateListView(QWidget *parent)
    : QWidget{parent},
      mainLayout(new QVBoxLayout(this)),
      multipleSubjectsTabBar(new QTabBar(this)),
      templateListView(new TemplateListView(this))
{
    const QStringList subjects({QStringLiteral("语文"), QStringLiteral("数学"),
                                QStringLiteral("英语"), QStringLiteral("物理"),
                                QStringLiteral("化学"), QStringLiteral("生物"),
                                QStringLiteral("undefined")});
    for(const auto &subjectName : subjects)
    {
        multipleSubjectsTabBar->addTab(subjectName);
    }
    multipleSubjectsTabBar->setDrawBase(false);
    multipleSubjectsTabBar->setElideMode(Qt::ElideRight);

    templateListView->setTemplateListModel(this->multipleSubjectsTemplateListModelList[0]);

    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(multipleSubjectsTabBar);
    mainLayout->addWidget(templateListView);

    connect(multipleSubjectsTabBar, &QTabBar::currentChanged, [this](int index)
    {
        if(index > -1)
        {
            templateListView->setTemplateListModel(this->multipleSubjectsTemplateListModelList[index]);
        }
    });
    connect(templateListView, &TemplateListView::templateNameClicked, this, &MultipleSubjectsTemplateListView::templateNameClicked);
}

void MultipleSubjectsTemplateListView::addNewTemplate(QPair<QString, QString> templateInfo)
{
    this->multipleSubjectsTemplateListModelList.addNewTemplate(templateInfo);
    this->multipleSubjectsTabBar->setCurrentIndex(MultipleSubjectsTemplateListModelList::Subjects::Undefined);
    this->templateListView->setCurrentIndex(
        this->multipleSubjectsTemplateListModelList
        .at(MultipleSubjectsTemplateListModelList::Subjects::Undefined)
        ->index(this->multipleSubjectsTemplateListModelList
                .at(MultipleSubjectsTemplateListModelList::Subjects::
                    Undefined)
                ->rowCount() - 1));
}

MultipleSubjectsTemplateListModelList &MultipleSubjectsTemplateListView::getMultipleSubjectsTemplateListModelList()
{
    return multipleSubjectsTemplateListModelList;
}
