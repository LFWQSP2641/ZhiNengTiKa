#include "TemplateListView.h"
#include "../Logic/TemplateListModel.h"

TemplateListView::TemplateListView(QWidget *parent)
    : QListView{parent}
{
    QScroller::grabGesture(this->viewport(), QScroller::TouchGesture);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
#ifdef Q_OS_ANDROID
    this->setAutoScroll(false);
#endif
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
