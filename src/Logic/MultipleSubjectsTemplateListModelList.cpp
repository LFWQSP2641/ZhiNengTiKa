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
            QList<QPair<QString, QString>> tempTemplateList;
            file.open(QFile::ReadOnly);
            while (!file.atEnd())
            {
                QString tempTemplateName{file.readLine()};
                tempTemplateName.resize(tempTemplateName.size() - 1);
                tempTemplateName.squeeze();

                QString tempTemplateCode{file.readLine()};
                tempTemplateCode.resize(tempTemplateCode.size() - 1);
                tempTemplateCode.squeeze();

                tempTemplateList.append(QPair<QString, QString>(tempTemplateName, tempTemplateCode));
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
    const QString dirPath { QStringLiteral(":/template/templateList/") };
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
    for(auto &i : templateListModelList)
    {
        i->deleteLater();
    }
}

void MultipleSubjectsTemplateListModelList::addNewTemplate(QPair<QString, QString> templateInfo)
{
    templateListModelList[Subjects::Undefined]->addNewTemplate(templateInfo);
}
