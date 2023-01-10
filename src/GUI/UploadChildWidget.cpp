#include "UploadChildWidget.h"
#include "PixmapLabel.h"
#include "PlusSignLabel.h"
#include "../StaticClass/Network.h"

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
        plusSignLabel = new PlusSignLabel(this);
        connect(plusSignLabel, &PlusSignLabel::addPixmapLabel, this, &UploadChildWidget::addPixmapLabelByPixmap);
        connect(plusSignLabel, &PlusSignLabel::addPixmapLabels, [this](const QList<QUrl> &urlList)
        {
            for(const auto &i : urlList)
            {
                this->addPixmapLabelFromUrl(i);
            }
        });
        answerLayout->addWidget(plusSignLabel);
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
    else
    {
        for(auto &i : pixmapLabelList)
        {
            rawScanData.append(i->getUrl().append(QStringLiteral(",")));
        }
        while(rawScanData.endsWith(","))
        {
            rawScanData.resize(rawScanData.size() - 1);
        }
    }

    object.insert(QStringLiteral("answer"), answerData);
    object.insert(QStringLiteral("rawScan"), rawScanData);

    return object;
}

void UploadChildWidget::setPixmapFromNetwork(const QUrl &url)
{
    clearPixmapLabelList();
    addPixmapLabelFromUrl(url);
}

PixmapLabel *UploadChildWidget::addPixmapLabelByPixmap(const QPixmap &pixmap)
{
    auto newPixmapLabel{this->addPixmapLabel()};
    newPixmapLabel->setPixmap(pixmap);
    return newPixmapLabel;
}

PixmapLabel *UploadChildWidget::addPixmapLabelFromUrl(const QUrl &url)
{
    if(!url.isValid())
    {
        return Q_NULLPTR;
    }
    auto newPixmapLabel{this->addPixmapLabel()};
    auto reply{ Network::networkAccessManager.get(QNetworkRequest(url)) };
    connect(reply, &QNetworkReply::finished, [newPixmapLabel, reply]
    {
        QPixmap pixmap;
        if(pixmap.loadFromData(Network::replyReadAll(reply)))
        {
            newPixmapLabel->setPixmap(pixmap);
        }
        else
        {
            newPixmapLabel->remove();
        }
    });
    return newPixmapLabel;
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

void UploadChildWidget::clearPixmapLabelList()
{
    for(auto &i : pixmapLabelList)
    {
        answerLayout->removeWidget(i);
        i->setParent(Q_NULLPTR);
        i->deleteLater();
    }
    pixmapLabelList.clear();
}

PixmapLabel* UploadChildWidget::addPixmapLabel()
{
    auto pixmapLabel{new PixmapLabel(this)};
    pixmapLabelList.append(pixmapLabel);
    answerLayout->insertWidget(answerLayout->count() - 1, pixmapLabel);
    connect(pixmapLabel, &PixmapLabel::removed, [this](PixmapLabel * point)
    {
        pixmapLabelList.removeOne(point);
        answerLayout->removeWidget(point);
        point->setParent(Q_NULLPTR);
        point->deleteLater();
    });
    return pixmapLabel;
}
