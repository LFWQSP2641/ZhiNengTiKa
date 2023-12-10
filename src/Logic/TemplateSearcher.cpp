#include "TemplateSearcher.h"
#include "../StaticClass/Global.h"

TemplateSearcher::TemplateSearcher(QObject *parent)
    : QThread{parent}
{

}

void TemplateSearcher::setSearchedTemplateName(const QString &searcheredTemplateName)
{
    this->templateName = searcheredTemplateName;
}

void TemplateSearcher::stop()
{
    this->toStop = true;
}

void TemplateSearcher::run()
{
    this->toStop = false;
    const QString dirPath { QStringLiteral(":/templateList") };
    QDir allDir { dirPath };
    auto list = allDir.entryInfoList(QDir::Files);
    list.append(QFileInfo(Global::dataPath().append(QStringLiteral("/templateList_UserHistory"))));

    for(const auto &i : list)
    {
        QFile file(i.absoluteFilePath());
        file.open(QFile::ReadOnly);
        while(!file.atEnd())
        {
            if(this->toStop)
            {
                emit this->searchStoped();
                emit this->searchFinished(false);
                return;
            }
            QString tempTemplateName(file.readLine());
            tempTemplateName.resize(tempTemplateName.size() - 1);

            QString tempTemplateCode(file.readLine());
            tempTemplateCode.resize(tempTemplateCode.size() - 1);

            if(tempTemplateName.contains(this->templateName, Qt::CaseInsensitive))
            {
                emit this->searchResult(TemplateSummary(tempTemplateName, tempTemplateCode));
            }
        }
    }
    emit this->searchFinished(true);
}
