#pragma once

class AnswerDetailData
{
public:
    AnswerDetailData(const QString &count, const QByteArray &questionId, const bool isAChoiceQuestion, const QBitArray &answer = QBitArray())
        : count(count), questionId(questionId), choiceQuestion(isAChoiceQuestion), answer(answer)
    {
        const auto trueCount{answer.count(true)};
        if(trueCount > 1)
        {
            multipleChoiceQuestion = true;
        }
    }
    AnswerDetailData(const AnswerDetailData &others)
        : count(others.getCount()), questionId(others.getQuestionId()), choiceQuestion(others.isChoiceQuestion()), answer(others.getAnswer()), multipleChoiceQuestion(others.isMultipleChoiceQuestion()) {}

    AnswerDetailData operator=(const AnswerDetailData &others)
    {
        return AnswerDetailData(others);
    }

    QString getCount() const
    {
        return count;
    }
    QByteArray getQuestionId() const
    {
        return questionId;
    }
    QBitArray getAnswer() const
    {
        return answer;
    }
    bool isChoiceQuestion() const
    {
        return choiceQuestion;
    }
    bool isMultipleChoiceQuestion() const
    {
        return multipleChoiceQuestion;
    }
protected:
    const QString count;
    const QByteArray questionId;
    const bool choiceQuestion;
    const QBitArray answer;
    bool multipleChoiceQuestion = false;
};
