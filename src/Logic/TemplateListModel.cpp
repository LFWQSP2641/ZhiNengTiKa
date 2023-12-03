#include "TemplateListModel.h"

TemplateListModel::TemplateListModel(const QList<QString> &templateNameList, const QList<QString> &templateCodeList, QObject *parent)
    : QAbstractListModel{parent}
{
    for(auto it1{templateNameList.begin()}, it2{templateCodeList.begin()}; (it1 < templateNameList.end() && it2 < templateCodeList.end()); ++it1, ++it2)
    {
        auto templateSummary(new TemplateSummary(*it1, *it2));
        templateSummary->setParent(this);
        templateList.append(templateSummary);
    }
}

int TemplateListModel::rowCount([[maybe_unused]]const QModelIndex &parent) const
{
    return templateList.count();
}

QVariant TemplateListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    if(index.row() >= templateList.size())
    {
        return QVariant();
    }
    if(role == Qt::DisplayRole)
    {
        return templateList.at(index.row())->getTemplateName();
    }
    else
    {
        return QVariant();
    }
}

QVariant TemplateListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if(orientation == Qt::Horizontal)
    {
        return QStringLiteral("C %0").arg(section);
    }
    else
    {
        return QStringLiteral("R %0").arg(section);
    }
}

TemplateSummary *TemplateListModel::getTemplateSummary(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return (new TemplateSummary(this));
    }
    return getTemplateSummary(index.row());
}

TemplateSummary *TemplateListModel::getTemplateSummary(int index)
{
    if(index >= templateList.size())
    {
        return (new TemplateSummary(this));
    }
    return templateList.at(index);
}

void TemplateListModel::addNewTemplate(TemplateSummary *templateSummary)
{
    beginInsertRows(QModelIndex(), 0, 0);
    this->templateList.append(templateSummary);
    endInsertRows();
}

void TemplateListModel::clear()
{
    if(this->templateList.size() == 0)
    {
        return;
    }
    beginRemoveRows(QModelIndex(), 0, this->templateList.size() - 1);
    this->templateList.clear();
    endRemoveRows();
}

QString TemplateListModel::getTemplateCode(const QModelIndex &index)
{
    return getTemplateSummary(index)->getTemplateCode();
}

QString TemplateListModel::getTemplateCode(int index) const
{
    if(index >= templateList.size())
    {
        return QString();
    }
    return templateList.at(index)->getTemplateCode();
}

bool TemplateListModel::hasTemplateName(const QString &templateName) const
{
    const auto result{std::any_of(
                          this->templateList.begin(), this->templateList.end(),
                          [&templateName](TemplateSummary * templateInfo)
    {
        return templateInfo->getTemplateName() == templateName;
    })};
    return result;
}

bool TemplateListModel::hasTemplateCode(const QString &templateCode) const
{
    const auto result{std::any_of(
                          this->templateList.begin(), this->templateList.end(),
                          [&templateCode](TemplateSummary * templateInfo)
    {
        return templateInfo->getTemplateCode() == templateCode;
    })};
    return result;
}
