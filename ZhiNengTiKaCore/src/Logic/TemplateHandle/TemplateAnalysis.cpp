#include "TemplateAnalysis.h"

#include "src/Logic/TemplateHandle/TemplateModel/TemplateDataTreeModel.h"

TemplateAnalysis::TemplateAnalysis()
    : templateDataTreeModel(QSharedPointer<TemplateDataTreeModel>(new TemplateDataTreeModel, &QObject::deleteLater))
{
}

QString TemplateAnalysis::getAnswerAndAnalysisHtml(const qsizetype index) const
{
    if (index == -1)
    {
        QString data;
        for (const auto &i : templateDataList)
            data.append(i->getAnswerAndAnalysisHtml());
        return data;
    }
    else
    {
        return templateDataList.at(index)->getAnswerAndAnalysisHtml();
    }
}

QString TemplateAnalysis::getAnswerHtml(const qsizetype index) const
{
    QString data;
    data.append(QStringLiteral("<p>"));
    if (index == -1)
    {
        for (const auto &i : templateDataList)
            data.append(i->getAnswerHtml());
        return data;
    }
    else
    {
        return templateDataList.at(index)->getAnswerHtml();
    }
    data.append(QStringLiteral("</p>"));

    return data;
}

QString TemplateAnalysis::getQuestionHtml(const qsizetype index) const
{
    if (index == -1)
    {
        QString data;
        for (const auto &i : templateDataList)
            data.append(i->getQuestionHtml());
        return data;
    }
    else
    {
        return templateDataList.at(index)->getQuestionHtml();
    }
}

TemplateAnswerData TemplateAnalysis::getCountAndAnswer(const qsizetype index) const
{
    if (index == -1)
    {
        TemplateAnswerData answerData;
        for (const auto &i : templateDataList)
            answerData.addChild(i->getCountAndAnswer());
        return answerData;
    }
    else
    {
        return templateDataList.at(index)->getCountAndAnswer();
    }
}

TemplateDataTreeModel *TemplateAnalysis::getTemplateDataTreeModel() const
{
    return templateDataTreeModel.get();
}

bool TemplateAnalysis::getLocal() const
{
    return local;
}

bool TemplateAnalysis::getNetwork() const
{
    return network;
}

bool TemplateAnalysis::getValid() const
{
    return valid;
}

void TemplateAnalysis::analyze(const QByteArray &rawData)
{
    QJsonObject object(QJsonDocument::fromJson(rawData).object().value(QStringLiteral("data")).toObject());
    if (this->templateName.isEmpty())
    {
        this->templateName = object.value(QStringLiteral("templateName")).toString();
    }
    QJsonArray array{ object.value(QStringLiteral("questions")).toArray().at(0).toObject().value(QStringLiteral("questionsAnswers")).toArray() };
    for (auto i{ 0 }; i < array.size(); ++i)
    {
        auto jsonObject{ array.at(i) };
        QJsonArray childQuestionArray{ jsonObject.toObject().value(QStringLiteral("childQuestionList")).toArray() };
        QJsonObject question{ jsonObject.toObject().value(QStringLiteral("question")).toObject() };
        const auto count{ QString::number(jsonObject.toObject().value(QStringLiteral("ordered")).toInt()) };

        auto tempdateData{ createTemplateData(question, count) };
        QList<QSharedPointer<TemplateData>> childQuestionList;

        QString questionsCountsStr{ QString::number(i + 1) };
        if (!childQuestionArray.isEmpty())
        {
            for (auto j{ 0 }; j < childQuestionArray.size(); ++j)
            {
                QJsonObject childQuestion{ childQuestionArray.at(j).toObject().value(QStringLiteral("question")).toObject() };
                childQuestionList.append(createTemplateData(childQuestion, QString::number(childQuestionArray.at(j).toObject().value(QStringLiteral("ordered")).toInt()), QString::number(j + 1)));
            }
            questionsCountsStr.append(QStringLiteral("(%0~%1)").arg(QString::number(childQuestionArray.at(0).toObject().value(QStringLiteral("ordered")).toInt()), QString::number(childQuestionArray.at(childQuestionArray.size() - 1).toObject().value(QStringLiteral("ordered")).toInt())));
        }
        else if (i + 1 != count.toInt())
        {
            questionsCountsStr.append(QStringLiteral("%0").arg(count));
        }
        questionsCountsStrList.append(questionsCountsStr);
        tempdateData->addChildren(std::move(childQuestionList));
        templateDataList.append(tempdateData);
    }
    templateDataTreeModel->rootItem->addChildren(templateDataList);
    this->valid = true;
}

QSharedPointer<TemplateData> TemplateAnalysis::createTemplateData(const QJsonObject &object, const QString &globalQuestionNumber, const QString &questionNumber)
{
    QStringList optionsArray;
    bool choiceQuestion{ !object.value(QStringLiteral("optionA")).toString().isEmpty() };
    if (choiceQuestion)
    {
        for (auto i{ 0 }; i < 7; ++i)
        {
            auto option{ QString(QByteArray::fromHex(QString::number(41 + i).toUtf8())) };
            auto questionOption{ object.value(QStringLiteral("option").append(option)).toString() };
            if (questionOption.isEmpty())
            {
                break;
            }
            else
            {
                optionsArray.append(option.append(QStringLiteral(".")).append(questionOption));
            }
        }
    }

    QSharedPointer<TemplateData> data(new TemplateData);
    data->answerContent = object.value(QStringLiteral("answer")).toString();
    data->answerExplanation = object.value(QStringLiteral("answerExplanation")).toString();
    data->questionContent = object.value(QStringLiteral("content")).toString();
    data->questionId = object.value(QStringLiteral("id")).toString();
    data->globalQuestionNumber = globalQuestionNumber;
    data->questionNumber = questionNumber;
    data->options = std::move(optionsArray);

    return data;
}

QList<QSharedPointer<TemplateData>> TemplateAnalysis::getTemplateDataList() const
{
    return templateDataList;
}
