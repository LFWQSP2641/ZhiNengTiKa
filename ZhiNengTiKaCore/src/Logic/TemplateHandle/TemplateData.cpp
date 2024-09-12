#include "TemplateData.h"

TemplateData::TemplateData()
{
}

TemplateData::TemplateData(const TemplateData &other)
{
    answerContent = other.answerContent;
    answerExplanation = other.answerExplanation;
    questionContent = other.questionContent;
    questionId = other.questionId;
    options = other.options;
    questionNumber = other.questionNumber;
    globalQuestionNumber = other.globalQuestionNumber;
    parentItem = other.parentItem;

    // 深拷贝 childQuestionList
    childQuestionList = other.cloneChildQuestionList();
}

TemplateData::~TemplateData()
{
    for (auto i : childQuestionList)
        delete i;
}

TemplateData &TemplateData::operator=(const TemplateData &other)
{
    if (this != &other)
    {
        answerContent = other.answerContent;
        answerExplanation = other.answerExplanation;
        questionContent = other.questionContent;
        questionId = other.questionId;
        options = other.options;
        questionNumber = other.questionNumber;
        globalQuestionNumber = other.globalQuestionNumber;
        parentItem = other.parentItem;

        // 深拷贝 childQuestionList
        childQuestionList = other.cloneChildQuestionList();
    }
    return *this;
}

QString TemplateData::getCommonQuestionNumberStr() const
{
    if (questionNumber.isEmpty())
        return globalQuestionNumber;
    else
        return QStringLiteral("%0(%1)").arg(globalQuestionNumber, questionNumber);
}

QString TemplateData::getAnswerAndAnalysisHtml() const
{
    QString data;
    if (childQuestionList.isEmpty())
    {
        data.append(QStringLiteral("<h1>第%0题:</h1>").arg(getCommonQuestionNumberStr()));
        data.append(QStringLiteral("<p><b>答案:</b>%0</p>").arg(answerContent));
        if (!answerExplanation.isEmpty())
            data.append(QStringLiteral("<p><b>解析:</b>%0</p>").arg(answerExplanation));
        data.append(QStringLiteral("<hr />"));
        return data;
    }
    for (const auto &i : childQuestionList)
        data.append(i->getAnswerAndAnalysisHtml());
    return data;
}

QString TemplateData::getAnswerHtml() const
{
    QString data;

    if (childQuestionList.isEmpty())
    {
        data.append(QStringLiteral("<b>%0.</b>%1&ensp;&ensp;&ensp;&ensp;")
                        .arg(getCommonQuestionNumberStr(),
                             QString(answerContent).remove("<p>").remove("</p>")));
    }
    else
    {
        for (const auto &i : childQuestionList)
            data.append(i->getAnswerHtml());
    }

    return data;
}

QString TemplateData::getQuestionHtml() const
{
    QString data;
    if (childQuestionList.isEmpty())
    {
        QString questionContentProcessed(questionContent);
        if (questionContentProcessed.startsWith(QStringLiteral("<p>")))
        {
            questionContentProcessed.remove(0, 3);
        }
        if (questionContentProcessed.endsWith(QStringLiteral("</p>")))
        {
            questionContentProcessed.resize(questionContentProcessed.size() - 4);
        }
        data.append(QStringLiteral("<h4>%0.&ensp;%1</h4><p>").arg(getCommonQuestionNumberStr(), questionContentProcessed));
        for (const auto &i : options)
            data.append(QStringLiteral("<br />").prepend(i));
        data.append(QStringLiteral("</p><hr />"));
        return data;
    }

    if (!questionContent.isEmpty())
    {
#ifdef Q_OS_WINDOWS
        data.append("<div style=\"border: 1px solid #000;\">");
#endif // Q_OS_WINDOWS
        data.append("<p>");
        data.append(questionContent);
        data.append("</p>");
#ifdef Q_OS_WINDOWS
        data.append("</div>");
#endif // Q_OS_WINDOWS
    }
    for (const auto &i : childQuestionList)
        data.append(i->getQuestionHtml());
    return data;
}

TemplateAnswerData TemplateData::getCountAndAnswer() const
{
    TemplateAnswerData data;

    if (childQuestionList.isEmpty())
    {
        data.count = getCommonQuestionNumberStr();
        data.questionId = questionId.toUtf8();
        if (!options.isEmpty())
        {
            data.choiceQuestion = true;
            data.choiceQuestionCount = options.size();
            data.answer = answerContent;
        }
        else
        {
            data.choiceQuestion = false;
        }
    }
    for (const auto &i : childQuestionList)
        data.addChild(i->getCountAndAnswer());
    return data;
}

QString TemplateData::getAnswerContent() const
{
    return answerContent;
}

QString TemplateData::getAnswerExplanation() const
{
    return answerExplanation;
}

QString TemplateData::getQuestionContent() const
{
    return questionContent;
}

QString TemplateData::getQuestionId() const
{
    return questionId;
}

QStringList TemplateData::getOptions() const
{
    return options;
}

QString TemplateData::getQuestionNumber() const
{
    return questionNumber;
}

QString TemplateData::getGlobalQuestionNumber() const
{
    return globalQuestionNumber;
}

QList<TemplateData *> TemplateData::getChildQuestionList() const
{
    return childQuestionList;
}

TemplateData *TemplateData::clone() const
{
    TemplateData *newObj = new TemplateData(*this);
    newObj->childQuestionList = this->cloneChildQuestionList();
    return newObj;
}

QList<TemplateData *> TemplateData::cloneChildQuestionList() const
{
    QList<TemplateData *> list;
    for (auto i : childQuestionList)
    {
        list.append(i->clone());
    }
    if (!list.isEmpty())
        qDebug() << childQuestionList << list;
    return list;
}

int TemplateData::childCount() const
{
    return childQuestionList.size();
}

int TemplateData::row() const
{
    if (parentItem == nullptr)
        return 0;
    return parentItem->childQuestionList.indexOf(this);
}

TemplateData *TemplateData::child(int row) const
{
    if (row < 0 || row >= childQuestionList.size())
        return nullptr;
    return childQuestionList.at(row);
}

void TemplateData::addChild(TemplateData *child)
{
    child->parentItem = this;
    this->childQuestionList.append(child);
}

void TemplateData::addChildren(QList<TemplateData *> &&children)
{
    for (auto i : children)
        i->parentItem = this;
    this->childQuestionList.append(children);
}

void TemplateData::addChild(const TemplateData &child)
{
    addChild(new TemplateData(child));
}

void TemplateData::addChildren(const QList<TemplateData> &children)
{
    QList<TemplateData *> list;
    for (const auto &i : children)
        list.append(new TemplateData(i));
    addChildren(std::move(list));
}

TemplateData *TemplateData::getParentItem() const
{
    return parentItem;
}
