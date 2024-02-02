#include "MultipleSubjectsTemplateListModelList.h"
#include "../StaticClass/Global.h"

MultipleSubjectsTemplateListModelList::MultipleSubjectsTemplateListModelList(QObject *parent)
    : QObject{parent}
{
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
        importTemplateList(QDir(dirPath).filePath(fileName), true);
    }
}

MultipleSubjectsTemplateListModelList::~MultipleSubjectsTemplateListModelList()
{
    writeUserHistoryToFile();
}

void MultipleSubjectsTemplateListModelList::addNewTemplate(const TemplateSummary &templateSummary)
{
    auto templateListModelUserHistory(templateListModelList[Subjects::UserHistory]);
    templateListModelUserHistory->addNewTemplate(templateSummary);
}

void MultipleSubjectsTemplateListModelList::addNewTemplate(const QString &templateName, const QString &templateCode)
{
    this->addNewTemplate(TemplateSummary(templateName, templateCode));
}

void MultipleSubjectsTemplateListModelList::importTemplateList(const QString &filePath, bool split)
{
    QFile file { filePath };
    if(file.exists())
    {
        QList<TemplateSummary> tempTemplateList;
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


                tempTemplateList.append(TemplateSummary(
                    tempTemplateNameData.at(2), tempTemplateCode,
                    tempTemplateNameData.at(0), tempTemplateNameData.at(1)));
            }
            else
            {
                tempTemplateList.append(TemplateSummary(tempTemplateName, tempTemplateCode));
            }
        }
        file.close();
        templateListModelList.append(new TemplateListModel(tempTemplateList, this));
    }
    else
    {
        templateListModelList.append(new TemplateListModel(this));
    }
}

void MultipleSubjectsTemplateListModelList::writeUserHistoryToFile() const
{
    QFile file(Global::dataPath().append(QStringLiteral("/templateList_UserHistory")));
    file.open(QFile::WriteOnly);
    file.write(templateListModelList.at(Subjects::UserHistory)->exportData());
    file.close();
}
