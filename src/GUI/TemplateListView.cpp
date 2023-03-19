#include "TemplateListView.h"
#include "../Logic/TemplateListModel.h"

TemplateListView::TemplateListView(QWidget *parent)
    : QListView{parent}
{
    connect(this, &QListView::clicked, this, &TemplateListView::findTemplateCode);
}

void TemplateListView::setTemplateListModel(TemplateListModel *model)
{
    this->templateListModel = model;
    this->setModel(model);
}

QString TemplateListView::findTemplateCode(const QModelIndex &templateNameIndex)
{
    const auto templateCode{this->templateListModel->getTemplateCode(templateNameIndex)};
    emit templateNameClicked(templateCode);
    return templateCode;
}
