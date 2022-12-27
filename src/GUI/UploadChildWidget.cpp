#include "UploadChildWidget.h"
#include "PixmapLabel.h"

UploadChildWidget::UploadChildWidget(const AnswerDetailData &answerDetailData, QWidget *parent)
    : QWidget{parent}, answerDetailData(answerDetailData)
{
    mainLayout = new QHBoxLayout(this);
    answerLayout = new QHBoxLayout;
    options = new QButtonGroup(this);

    auto getConstTextLabel{[this](const QString & text)
    {
        auto label{new QLabel(text)};
        label->setFixedSize(this->fontMetrics().size(Qt::TextSingleLine, text));
        return label;
    }};

    auto tempVBoxLayout{new QVBoxLayout};
    tempVBoxLayout->addWidget(getConstTextLabel(answerDetailData.getCount()));

    mainLayout->addLayout(tempVBoxLayout);

    if(answerDetailData.isChoiceQuestion())
    {
        choiceQuestions = true;
        auto tempBtnGroupBox{new QGroupBox(this)};
        auto tempBtnLayout{new QHBoxLayout(tempBtnGroupBox)};

        QPalette red;
        red.setColor(QPalette::WindowText, Qt::red);
        QPalette green;
        green.setColor(QPalette::WindowText, Qt::green);

        if(answerDetailData.isMultipleChoiceQuestion())
        {
            options->setExclusive(false);
            for(auto i{0}; i < answerDetailData.getAnswer().count(); ++i)
            {
                auto btn{new QCheckBox(QString(QByteArray::fromHex(QString::number(41 + i).toUtf8())))};
                if(answerDetailData.getAnswer().at(i))
                {
                    btn->setChecked(true);
                    btn->setPalette(green);
                }
                else
                {
                    btn->setPalette(red);
                }
                options->addButton(btn);
                tempBtnLayout->addWidget(btn);
            }
        }
        else
        {
            options->setExclusive(true);
            for(auto i{0}; i < answerDetailData.getAnswer().count(); ++i)
            {
                auto btn{new QRadioButton(QString(QByteArray::fromHex(QString::number(41 + i).toUtf8())))};
                if(answerDetailData.getAnswer().at(i))
                {
                    btn->setChecked(true);
                    btn->setPalette(green);
                }
                else
                {
                    btn->setPalette(red);
                }
                options->addButton(btn);
                tempBtnLayout->addWidget(btn);
            }
        }
        answerLayout->addWidget(tempBtnGroupBox);
    }
    else
    {
        choiceQuestions = false;
        addPixmapLabel();
    }
    mainLayout->addLayout(answerLayout);
    mainLayout->addStretch();
}

QJsonObject UploadChildWidget::getJsonObject()
{
    QJsonObject object;
    object.insert(QStringLiteral("questionId"), QString(answerDetailData.getQuestionId()));

    QString answerData;
    QString rawScanData;
    if(options != Q_NULLPTR)
    {
        const auto btnList{options->buttons()};
        for(auto i{0}; i < btnList.count(); ++i)
        {
            if(btnList.at(i)->isChecked())
            {
                answerData.append(QString(QByteArray::fromHex(QString::number(41 + i).toUtf8())));
            }
        }
    }
    for(auto &i : pixmapLabelList)
    {
        rawScanData.append(i->getUrl().append(QStringLiteral(",")));
    }
    while(rawScanData.endsWith(","))
    {
        rawScanData.resize(rawScanData.size() - 1);
    }

    object.insert(QStringLiteral("answer"), answerData);
    object.insert(QStringLiteral("rawScan"), rawScanData);

    return object;
}

void UploadChildWidget::setPixmapFromNetwork(const QString &url)
{
    //清空pixmapLabelList
    //因为图片只有一个
    for(auto &i : pixmapLabelList)
    {
        answerLayout->removeWidget(i);
        i->deleteLater();
    }
    pixmapLabelList.clear();
    addPixmapLabel()->setPixmapFromNetwork(url);
}

PixmapLabel* UploadChildWidget::addPixmapLabel()
{
    auto pixmapLabel{new PixmapLabel(this)};
    pixmapLabelList.append(pixmapLabel);
    answerLayout->addWidget(pixmapLabel);
    connect(pixmapLabel, &PixmapLabel::pixmapRemoved, [this](PixmapLabel * point)
    {
        pixmapLabelList.remove(pixmapLabelList.indexOf(point));
        answerLayout->removeWidget(point);
        point->deleteLater();
        if(pixmapLabelList.isEmpty())
        {
            addPixmapLabel();
        }
    });
    connect(pixmapLabel, &PixmapLabel::pixmapSet, this, &UploadChildWidget::addPixmapLabel);
    return pixmapLabel;
}

void UploadChildWidget::setChecked(const QBitArray &on)
{
    const auto btnList{options->buttons()};
    auto count{on.count() < btnList.count() ? on.count() : btnList.count()};
    for(auto i{0}; i < count; ++i)
    {
        btnList.at(i)->setChecked(on.at(i));
    }
}
