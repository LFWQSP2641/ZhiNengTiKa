#include "TemplateListModel.h"

TemplateListModel::TemplateListModel(const QList<QString> &templateNameList, const QList<QString> &templateCodeList, QObject *parent)
    : QAbstractListModel{parent}
{
    for(auto it1{templateNameList.begin()}, it2{templateCodeList.begin()}; (it1 < templateNameList.end() && it2 < templateCodeList.end()); ++it1, ++it2)
    {
        templateList.append(TemplateSummary(*it1, *it2));
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
        return templateList.at(index.row()).getTemplateName();
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

void TemplateListModel::addNewTemplate(const TemplateSummary &templateInfo)
{
    beginInsertRows(QModelIndex(), this->templateList.size(), 1);
    this->templateList.append(templateInfo);
    endInsertRows();
}

void TemplateListModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, this->templateList.size());
    this->templateList.clear();
    endRemoveRows();
}

QString TemplateListModel::getTemplateCode(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return QString();
    }
    if(index.row() >= templateList.size())
    {
        return QString();
    }
    return templateList.at(index.row()).getTemplateCode();
}

QString TemplateListModel::getTemplateCode(int index) const
{
    return templateList.at(index).getTemplateCode();
}

bool TemplateListModel::hasTemplateName(const QString &templateName) const
{
    const auto result{std::any_of(
                          this->templateList.begin(), this->templateList.end(),
                          [&templateName](const TemplateSummary & templateInfo)
    {
        return templateInfo.getTemplateName() == templateName;
    })};
    return result;
}

bool TemplateListModel::hasTemplateCode(const QString &templateCode) const
{
    const auto result{std::any_of(
                          this->templateList.begin(), this->templateList.end(),
                          [&templateCode](const TemplateSummary & templateInfo)
    {
        return templateInfo.getTemplateCode() == templateCode;
    })};
    return result;
}
