#include "MultipleSubjectsTemplateListModelList.h"
#include "../StaticClass/Setting.h"
#include "../StaticClass/Global.h"
#include "../QMLIntermediary/TemplateRawDataQML.h"

MultipleSubjectsTemplateListModelList::MultipleSubjectsTemplateListModelList(QObject *parent)
    : QObject{parent}
{
    auto importTemplateList{[this](QString filePath, bool split = true)
    {
        QFile file { filePath };
        if(file.exists())
        {
            QList<TemplateSummary> tempTemplateList;
            file.open(QFile::ReadOnly);
            while (!file.atEnd())
            {
                QString tempTemplateName{file.readLine()};
                tempTemplateName.resize(tempTemplateName.size() - 1);
                tempTemplateName.squeeze();
                QString tempTemplateCode{file.readLine()};
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
            templateListModelList.append(new TemplateListModel(tempTemplateList));
        }
        else
        {
            templateListModelList.append(new TemplateListModel());
        }
    }};

    const QStringList fileListNames({QStringLiteral("templateList_chinese"),
                                     QStringLiteral("templateList_mathematics"),
                                     QStringLiteral("templateList_english"),
                                     QStringLiteral("templateList_physics"),
                                     QStringLiteral("templateList_chemistry"),
                                     QStringLiteral("templateList_biography")});

    const QString dirPath { QStringLiteral(":/templateList/") };
    const QDir allDir { QString(dirPath).append(QStringLiteral("all")) };
    const QDir latestDir{ QString(dirPath).append(QStringLiteral("latest")) };

    if(Setting::listLatestTemplatePreferentially && latestDir.exists())
    {
        for(const auto &fileName : fileListNames)
        {
            importTemplateList(QDir(latestDir).filePath(fileName));
        }
    }
    else
    {
        for(const auto &fileName : fileListNames)
        {
            importTemplateList(QDir(allDir).filePath(fileName));
        }
    }
    importTemplateList(Global::tempPath().append(QStringLiteral("/")).append(QStringLiteral("templateList_undefined")), false);
}

MultipleSubjectsTemplateListModelList::~MultipleSubjectsTemplateListModelList()
{
    //BUG 引发的异常: 0xC0000005: 读取位置 0xFFFFFFFFFFFFFFFF 时发生访问冲突.
//    for(auto &i : templateListModelList)
//    {
//        i->deleteLater();
//    }
}

void MultipleSubjectsTemplateListModelList::addNewTemplate(const TemplateSummary &templateSummary)
{
    templateListModelList[Subjects::Undefined]->addNewTemplate(templateSummary);
    this->addTemplateList(templateSummary);
}

void MultipleSubjectsTemplateListModelList::addNewTemplate(TemplateRawDataQML *templateRawDataQML)
{
    const auto templateSummary{templateRawDataQML->getTemplateRawData()};
    this->addNewTemplate(templateSummary);
}

void MultipleSubjectsTemplateListModelList::addNewTemplate(const QString &templateName, const QString &templateCode)
{
    this->addNewTemplate(TemplateSummary(templateName, templateCode));
}

void MultipleSubjectsTemplateListModelList::addTemplateList(const TemplateSummary &templateSummary)
{
    this->addTemplateList(templateSummary.getTemplateName(), templateSummary.getTemplateCode());
}

void MultipleSubjectsTemplateListModelList::addTemplateList(const QString &templateName, const QString &templateCode)
{
    const QString data(QString(templateName).append(QStringLiteral("\n")).append(templateCode).append(QStringLiteral("\n")));
    QFile f(Global::tempPath().append(QStringLiteral("/")).append(QStringLiteral("templateList_undefined")));
    f.open(QFile::ReadWrite | QFile::Append);
    f.write(data.toUtf8());
    f.close();
}
