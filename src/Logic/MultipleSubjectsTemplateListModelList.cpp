#include "MultipleSubjectsTemplateListModelList.h"
#include "../Singleton/Settings.h"
#include "../StaticClass/Global.h"

MultipleSubjectsTemplateListModelList::MultipleSubjectsTemplateListModelList(QObject *parent)
    : QObject{parent}
{
    auto importTemplateList{[this](QString filePath, bool split = true)
    {
        QFile file { filePath };
        if(file.exists())
        {
            QList<TemplateSummary *> tempTemplateList;
            file.open(QFile::ReadOnly);
            while (!file.atEnd())
            {
                QString tempTemplateName{file.readLine()};
                if(tempTemplateName.endsWith(QByteArrayLiteral("\n")))
                    tempTemplateName.resize(tempTemplateName.size() - 1);
                tempTemplateName.squeeze();
                QString tempTemplateCode{file.readLine()};
                if(tempTemplateCode.endsWith(QByteArrayLiteral("\n")))
                    tempTemplateCode.resize(tempTemplateCode.size() - 1);
                tempTemplateCode.squeeze();
                if(split)
                {
                    QStringList tempTemplateNameData(3);
                    qsizetype index{0};
                    for(const auto &i : qAsConst(tempTemplateName))
                    {
                        if((index < 2) && (i == QChar(32)))
                        {
                            ++index;
                        }
                        else
                        {
                            tempTemplateNameData[index].append(i);
                        }
                    }
                    for(auto &i : tempTemplateNameData)
                    {
                        i.squeeze();
                    }


                    tempTemplateList.append(new TemplateSummary(
                                                tempTemplateNameData.at(2), tempTemplateCode,
                                                tempTemplateNameData.at(0), tempTemplateNameData.at(1),
                                                this));
                }
                else
                {
                    tempTemplateList.append(new TemplateSummary(tempTemplateName, tempTemplateCode,
                                            QString(), QString(), this));
                }
            }
            file.close();
            templateListModelList.append(new TemplateListModel(tempTemplateList, this));
        }
        else
        {
            templateListModelList.append(new TemplateListModel(this));
        }
    }};

    const QStringList fileListNames({QStringLiteral("templateList_chinese"),
                                     QStringLiteral("templateList_mathematics"),
                                     QStringLiteral("templateList_english"),
                                     QStringLiteral("templateList_physics"),
                                     QStringLiteral("templateList_chemistry"),
                                     QStringLiteral("templateList_biography")});
    const QString dirPath { QStringLiteral(":/templateList") };
    importTemplateList(Global::dataPath().append(QStringLiteral("/templateList_UserHistory")), false);
    for(const auto &fileName : fileListNames)
    {
        importTemplateList(QDir(dirPath).filePath(fileName));
    }
}

MultipleSubjectsTemplateListModelList::~MultipleSubjectsTemplateListModelList()
{
}

void MultipleSubjectsTemplateListModelList::addNewTemplate(TemplateSummary *templateSummary)
{
    templateSummary->setParent(this);
    templateListModelList[Subjects::UserHistory]->addNewTemplate(templateSummary);
    this->addTemplateList(templateSummary);
}

void MultipleSubjectsTemplateListModelList::addNewTemplate(const QString &templateName, const QString &templateCode)
{
    this->addNewTemplate(new TemplateSummary(templateName, templateCode, QString(), QString(), this));
}

void MultipleSubjectsTemplateListModelList::addTemplateList(TemplateSummary *templateSummary)
{
    this->addTemplateList(templateSummary->getTemplateName(), templateSummary->getTemplateCode());
}

void MultipleSubjectsTemplateListModelList::addTemplateList(const QString &templateName, const QString &templateCode)
{
    const QString data(QString(templateName).append(QStringLiteral("\n")).append(templateCode).append(QStringLiteral("\n")));
    QFile file(Global::dataPath().append(QStringLiteral("/templateList_UserHistory")));
    file.open(QFile::ReadWrite | QFile::Append);
    file.write(data.toUtf8());
    file.close();
}
