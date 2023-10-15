#include "TemplateAnalysis.h"

QString TemplateAnalysis::getAnswerAndAnalysisHtml(const qsizetype index) const
{
    QString webStr;

    auto func{[&webStr](const QJsonObject & i)
    {
        webStr.append(QStringLiteral("<h1>第"));
        webStr.append(i.value("count").toString());
        webStr.append(QStringLiteral("题:</h1><p><b>答案:</b>"));
        webStr.append(i.value("answerContent").toString());
        webStr.append(QStringLiteral("</p>"));
        QString answerExplanation{ i.value("answerExplanation").toString() };
        if(!answerExplanation.isEmpty())
        {
            webStr.append(QStringLiteral("<p><b>解析:</b>").append(answerExplanation).append(QStringLiteral("</p>")));
        }
        webStr.append(QStringLiteral("<hr />"));
    }};

    callFunc(this->answerDataList, index, func);

    return webStr;
}

QString TemplateAnalysis::getAnswerHtml(const qsizetype index) const
{
    QString webStr;

    webStr.append(QStringLiteral("<p>"));
    auto func{[&webStr](const QJsonObject & i)
    {
        webStr.append(QStringLiteral("<b>"));
        webStr.append(i.value("count").toString());
        webStr.append(QStringLiteral(".</b>"));
        auto questionContent{ i.value("answerContent").toString() };
        questionContent.remove("<p>").remove("</p>");
        webStr.append(questionContent);
        webStr.append(QStringLiteral("&ensp;&ensp;&ensp;&ensp;"));
    }};
    webStr.append(QStringLiteral("</p>"));

    callFunc(this->answerDataList, index, func);

    return webStr;
}

QString TemplateAnalysis::getQuestionHtml(const qsizetype index) const
{
    QString webStr;

    auto func1{[&webStr](const QJsonObject & i)
    {
        webStr.append(QStringLiteral("<h4>"));
        webStr.append(i.value("count").toString());
        webStr.append(QStringLiteral(".&ensp;"));
        auto questionContent{ i.value("questionContent").toString() };
        if(questionContent.startsWith(QStringLiteral("<p>")))
        {
            questionContent.remove(0, 3);
        }
        if(questionContent.endsWith(QStringLiteral("</p>")))
        {
            questionContent.resize(questionContent.size() - 4);
        }
        webStr.append(questionContent);
        webStr.append(QStringLiteral("</h4><p>"));
        for(const auto &j : i.value("options").toArray())
        {
            webStr.append(j.toString().toUtf8().append(QByteArrayLiteral("<br />")));

        }
        webStr.append(QStringLiteral("</p><hr />"));
    }};

    auto func2{[&webStr](const QJsonObject & i)
    {
#ifdef Q_OS_WINDOWS
        webStr.append("<div style=\"border: 1px solid #000;\">");
#endif //Q_OS_WINDOWS
        webStr.append("<p>");
        webStr.append(i.value("questionContent").toString());
        webStr.append("</p>");
#ifdef Q_OS_WINDOWS
        webStr.append("</div>");
#endif //Q_OS_WINDOWS
    }};

    callFunc(this->answerDataList, index, func1, func2);

    return webStr;
}

QList<AnswerDetailData> TemplateAnalysis::getCountAndAnswer(const qsizetype index) const
{
    QList<AnswerDetailData> data;

    auto func{[&data](const QJsonObject & i)
    {
        if(!i.value("options").toArray().isEmpty())
        {
            const auto optionsCount{i.value("options").toArray().size()};
            QBitArray answer(optionsCount);
            auto answerStr{i.value("answerContent").toString()};
            for(auto j{0}; j < optionsCount; ++j)
            {
                auto option{ QString(QByteArray::fromHex(QString::number(41 + j).toUtf8())) };
                answer.setBit(j, answerStr.contains(option));
            }
            data.append(AnswerDetailData(i.value("count").toString(), i.value("questionId").toString().toUtf8(), true, answer));
        }
        else
        {
            data.append(AnswerDetailData(i.value("count").toString(), i.value("questionId").toString().toUtf8(), false));
        }
    }};

    callFunc(this->answerDataList, index, func);

    return data;
}

bool TemplateAnalysis::getInternal() const
{
    return internal;
}

bool TemplateAnalysis::getValid() const
{
    return valid;
}

bool TemplateAnalysis::getNetwork() const
{
    return network;
}

bool TemplateAnalysis::getExternal() const
{
    return external;
}

void TemplateAnalysis::analyze(const QByteArray &rawData)
{
    auto funCreateAnswerData{[](const QJsonObject & object, const QString & count)
    {
        const auto answerContent{ object.value(QStringLiteral("answer")).toString() };
        const auto answerExplanation{ object.value(QStringLiteral("answerExplanation")).toString() };
        const auto questionContent{ object.value(QStringLiteral("content")).toString() };
        const auto questionId{object.value(QStringLiteral("id")).toString()};
        QJsonArray optionsArray;
        bool choiceQuestion{!object.value(QStringLiteral("optionA")).toString().isEmpty()};
        if(choiceQuestion)
        {
            for(auto i{0}; i < 7; ++i)
            {
                auto option{ QString(QByteArray::fromHex(QString::number(41 + i).toUtf8())) };
                auto questionOption{object.value(QStringLiteral("option").append(option)).toString()};
                if(questionOption.isEmpty())
                {
                    break;
                }
                else
                {
                    optionsArray.append(option.append(QStringLiteral(".")).append(questionOption));
                }
            }
        }

        QJsonObject outputObject;
        outputObject.insert(QStringLiteral("answerContent"), answerContent);
        outputObject.insert(QStringLiteral("answerExplanation"), answerExplanation);
        outputObject.insert(QStringLiteral("count"), count);
        outputObject.insert(QStringLiteral("questionContent"), questionContent);
        outputObject.insert(QStringLiteral("questionId"), questionId);
        outputObject.insert(QStringLiteral("options"), optionsArray);
        return outputObject;
    }};

    QJsonObject object(QJsonDocument::fromJson(rawData).object().value(QStringLiteral("data")).toObject());
    if(this->templateName.isEmpty())
    {
        this->templateName = object.value(QStringLiteral("templateName")).toString();
    }
    QJsonArray array{ object.value(QStringLiteral("questions")).toArray().at(0).toObject().value(QStringLiteral("questionsAnswers")).toArray() };
    for (auto i{ 0 }; i < array.size(); ++i)
    {
        auto jsonObject{ array.at(i) };
        QJsonArray childQuestionList{ jsonObject.toObject().value(QStringLiteral("childQuestionList")).toArray() };
        QJsonObject question{ jsonObject.toObject().value(QStringLiteral("question")).toObject() };
        const auto count{QString::number(jsonObject.toObject().value(QStringLiteral("ordered")).toInt())};

        auto answerData{funCreateAnswerData(question, count)};
        QJsonArray childQuestionArray;

        QString questionsCountsStr{QString::number(i + 1)};
        if (!childQuestionList.isEmpty())
        {
            for (auto j{ 0 }; j < childQuestionList.size(); ++j)
            {
                QJsonObject childQuestion{ childQuestionList.at(j).toObject().value(QStringLiteral("question")).toObject() };
                const auto childCount{QString::number(childQuestionList.at(j).toObject().value(QStringLiteral("ordered")).toInt()).append(QStringLiteral("(")).append(QString::number(j + 1)).append(QStringLiteral(")"))};
                childQuestionArray.append(funCreateAnswerData(childQuestion, childCount));
            }
            questionsCountsStr.append(QStringLiteral("(%0~%1)").arg(QString::number(childQuestionList.at(0).toObject().value(QStringLiteral("ordered")).toInt()), QString::number(childQuestionList.at(childQuestionList.size() - 1).toObject().value(QStringLiteral("ordered")).toInt())));
        }
        else if(i + 1 != count.toInt())
        {
            questionsCountsStr.append(QStringLiteral("(%0)").arg(count));
        }
        questionsCountsStrList.append(questionsCountsStr);
        answerData.insert(QStringLiteral("childQuestionList"), childQuestionArray);
        answerDataList.append(answerData);
    }
    this->valid = true;
}

void TemplateAnalysis::deleteLater()
{
    this->QObject::deleteLater();
}

template<typename f>
void TemplateAnalysis::callFunc(const QJsonArray &jsonArray, const qsizetype index, f func)
{
    //不知道起啥名了,用于简化流程
    auto func2{[&func](const QJsonObject & object)
    {
        const auto array{object.value("childQuestionList").toArray()};
        if(array.isEmpty())
        {
            func(object);
        }
        else
        {
            for(auto &j : array)
            {
                func(j.toObject());
            }
        }
    }};

    if(index == -1)
    {
        for(const auto &i : jsonArray)
        {
            func2(i.toObject());
        }
    }
    else
    {
        func2(jsonArray.at(index).toObject());
    }
}

template<typename f1, typename f2>
void TemplateAnalysis::callFunc(const QJsonArray &jsonArray, const qsizetype index, f1 func1, f2 func2)
{
    //不知道起啥名了,用于简化流程
    auto func3{[&func1, &func2](const QJsonObject & object)
    {
        const auto array{object.value("childQuestionList").toArray()};
        if(array.isEmpty())
        {
            func1(object);
        }
        else
        {
            func2(object);
            for(auto &j : array)
            {
                func1(j.toObject());
            }
        }
    }};

    if(index == -1)
    {
        for(const auto &i : jsonArray)
        {
            func3(i.toObject());
        }
    }
    else
    {
        func3(jsonArray.at(index).toObject());
    }
}

