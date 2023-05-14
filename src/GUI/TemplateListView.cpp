#include "TemplateListView.h"
#include "../Logic/TemplateListModel.h"

TemplateListView::TemplateListView(QWidget *parent)
    : QListView{parent}
{
    QScroller::grabGesture(this->viewport(), QScroller::TouchGesture);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    connect(this, &QListView::clicked, this, &TemplateListView::emitTemplateSummary);
}

TemplateSummary TemplateListView::getCurrentTemplateSummary()
{
    return getTemplateSummary(this->currentIndex());
}

TemplateSummary TemplateListView::getTemplateSummary(const QModelIndex &templateNameIndex) const
{
    const auto templateSummary{this->templateListModel->getTemplateSummary(templateNameIndex)};
    return templateSummary;
}

void TemplateListView::setTemplateListModel(TemplateListModel *model)
{
    this->templateListModel = model;
    this->setModel(model);
}


void TemplateListView::clear()
{
    if(this->templateListModel != nullptr)
    {
        this->templateListModel->clear();
    }
}

void TemplateListView::addNewTemplate(const TemplateSummary &templateSummary)
{
    if(this->templateListModel != nullptr)
    {
        this->templateListModel->addNewTemplate(templateSummary);
    }
}

void TemplateListView::emitTemplateSummary(const QModelIndex &templateNameIndex)
{
    emit templateNameClicked(getTemplateSummary(templateNameIndex));
}
