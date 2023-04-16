#include "MultipleSubjectsTemplateListModelList.h"
#include "../StaticClass/Setting.h"
#include "../StaticClass/Global.h"

MultipleSubjectsTemplateListModelList::MultipleSubjectsTemplateListModelList(QObject *parent)
    : QObject{parent}
{
    auto importTemplateList{[this](QString filePath)
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

                QString tempTemplateCode{file.readLine()};
                tempTemplateCode.resize(tempTemplateCode.size() - 1);
                tempTemplateCode.squeeze();

                tempTemplateList.append(TemplateSummary(
                                            tempTemplateNameData.at(2), tempTemplateCode,
                                            tempTemplateNameData.at(0), tempTemplateNameData.at(1)));
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

#ifdef Q_OS_ANDROID
    const QString dirPath { QStringLiteral("assets:/templateList/") };
#else
    const QString dirPath { QStringLiteral(":/templateList/") };
#endif
    const QDir allDir {dirPath + QStringLiteral("all")};
    const QDir latestDir{dirPath + QStringLiteral("latest")};

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
    importTemplateList(Global::tempPath() + QDir::separator() + QStringLiteral("templateList_undefined"));
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
}

void MultipleSubjectsTemplateListModelList::addNewTemplate(const QString &templateName, const QString &templateCode)
{
    this->addNewTemplate(TemplateSummary(templateName, templateCode));
}
