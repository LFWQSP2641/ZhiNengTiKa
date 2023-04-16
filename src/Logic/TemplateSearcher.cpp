#include "TemplateSearcher.h"

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
#ifdef Q_OS_ANDROID
    const QString dirPath { QStringLiteral("assets:/templateList/") };
#else
    const QString dirPath { QStringLiteral(":/templateList/") };
#endif

    QDir allDir {dirPath + QStringLiteral("all")};

    allDir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
    allDir.setSorting(QDir::Time | QDir::DirsFirst);
    QFileInfoList list = allDir.entryInfoList();

    for(int i = 0; i < list.size(); i++)
    {
        if(list.at(i).isFile())
        {
            QFile file(list.at(i).absoluteFilePath());
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
    }
    emit this->searchFinished(true);
}
