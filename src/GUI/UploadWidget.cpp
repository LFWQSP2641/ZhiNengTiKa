#include "UploadWidget.h"
#include "UploadChildWidget.h"
#include "../Logic/AnalysisWebRawData.h"
#include "../StaticClass/Setting.h"
#include "../StaticClass/Network.h"
#include "../StaticClass/XinjiaoyuNetwork.h"
#include "../StaticClass/Login.h"

UploadWidget::UploadWidget(AnalysisWebRawData &analysisWebRawData, QWidget *parent)
    : QWidget{parent}, analysisWebRawData(analysisWebRawData)
{
    mainLayout = new QVBoxLayout(this);
    uploadChildWidgetLayout = new QVBoxLayout;
    rightAnswerPrecedenceCheckBox = new QRadioButton(QStringLiteral("正确答案优先"), this);
    uploadAnswerPrecedenceCheckBox = new QRadioButton(QStringLiteral("上传结果优先"), this);
    uploadButton = new QPushButton("提交", this);
    editRawDataButton = new QPushButton("查看原数据", this);
    auto tempLayout1{new QHBoxLayout};
    tempLayout1->addWidget(editRawDataButton, 1);
    tempLayout1->addWidget(uploadButton, 3);

    answerPrecedenceCheckBoxGroupBox = new QGroupBox(this);
    auto tempLayout2{new QHBoxLayout(answerPrecedenceCheckBoxGroupBox)};
    tempLayout2->addWidget(rightAnswerPrecedenceCheckBox);
    tempLayout2->addWidget(uploadAnswerPrecedenceCheckBox);

    mainLayout->addWidget(answerPrecedenceCheckBoxGroupBox);
    mainLayout->addLayout(uploadChildWidgetLayout);
    mainLayout->addLayout(tempLayout1);

    connect(&analysisWebRawData, &AnalysisWebRawData::analysisFinished, [this]
    {
        if(this->isVisible())
        {
            analysis();
        }
        else
        {
            analysisWebRawDataStateChanged = true;
        }
    });
    connect(uploadButton, &QPushButton::clicked, this, &UploadWidget::upload);
    connect(editRawDataButton, &QPushButton::clicked, this, &UploadWidget::editRawData);
    connect(rightAnswerPrecedenceCheckBox, &QRadioButton::clicked, this, &UploadWidget::switchRightAnswerPrecedence);
    connect(uploadAnswerPrecedenceCheckBox, &QRadioButton::clicked, this, &UploadWidget::switchuploadAnswerPrecedence);
}

bool UploadWidget::upload()
{
    this->uploadButton->setEnabled(false);

    const auto jsonObject{getAnswerJsonObject()};
    const auto answerArray{jsonObject.value("answers").toArray()};

    int missCount(0);
    for(auto &i : answerArray)
    {
        auto object{i.toObject()};
        if(object.value(QStringLiteral("answer")).toString().isEmpty() && object.value(QStringLiteral("rawScan")).toString().isEmpty())
        {
            ++missCount;
        }
    }
    if(missCount > 0)
    {
        const auto result{QMessageBox::question(this, QStringLiteral("question"), QStringLiteral("还有%0题未答,是否提交").arg(missCount))};
        if(result == QMessageBox::No)
        {
            this->uploadButton->setEnabled(true);
            return false;
        }
    }

    const auto data{QJsonDocument(jsonObject).toJson(QJsonDocument::Compact)};

    this->uploadButton->setEnabled(true);
    return uploadData(data);
}

bool UploadWidget::uploadData(const QByteArray &data)
{
    auto request{XinjiaoyuNetwork::setRequest(QStringLiteral("https://www.xinjiaoyu.com/api/v3/server_homework/student/homework/answer"))};
    if(!request.url().isEmpty())
    {
        request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
        const auto replyData{ Network::postData(request, data) };
        if(replyData == QStringLiteral("{\"code\":200,\"data\":null,\"msg\":\"操作成功！\"}"))
        {
            QMessageBox::information(this, QStringLiteral("information"), QStringLiteral("提交成功！"));
            this->uploadButton->setEnabled(true);
            return true;
        }
        else
        {
            QMessageBox::warning(this, QStringLiteral("warning"), QStringLiteral("提交失败！\n").append(replyData));
            this->uploadButton->setEnabled(true);
            return false;
        }
    }
    else
    {
        return false;
    }
}

void UploadWidget::getUserAnswer()
{
    auto templateCode{analysisWebRawData.getTemplateCode()};
    if(templateCode.isEmpty())
    {
        return;
    }

    auto funCreateAnswerData{[](const QJsonObject & object)
    {
        QJsonObject outputObject;
        outputObject.insert(QStringLiteral("userAnswer"), object.value("answer"));
        outputObject.insert(QStringLiteral("rawScan"), object.value("rawScan"));
        return outputObject;
    }};

    QByteArray webRawData;
    try
    {
        webRawData = XinjiaoyuNetwork::getTemplateCodeData(templateCode);
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(Q_NULLPTR, QStringLiteral("critical"), e.what());
        return;
    }

    QJsonArray array{ QJsonDocument::fromJson(webRawData).object().value(QStringLiteral("data")).toObject().value(QStringLiteral("questions")).toArray().at(0).toObject().value(QStringLiteral("questionsAnswers")).toArray() };
    for (const auto &j : array)
    {
        auto jObject{j.toObject()};
        QJsonArray childQuestionList{ jObject.value(QStringLiteral("childQuestionList")).toArray() };

        if (childQuestionList.isEmpty())
        {
            userAnswerList.append(funCreateAnswerData(jObject.value(QStringLiteral("answer")).toObject()));
        }
        else
        {
            for (auto i{ 0 }; i < childQuestionList.size(); ++i)
            {
                userAnswerList.append(funCreateAnswerData(childQuestionList.at(i).toObject().value(QStringLiteral("answer")).toObject()));
            }
        }
    }
}

void UploadWidget::switchRightAnswerPrecedence()
{
    this->setEnabled(false);
    if(userAnswerList.isEmpty())
    {
        getUserAnswer();
    }
    for(auto i{0}; i < uploadChildWidgetList.count(); ++i)
    {
        if(!uploadChildWidgetList.at(i)->isChoiceQuestions())
        {
            uploadChildWidgetList.at(i)->setChecked(rightAnswer.at(i));
            const auto rawScan{userAnswerList.at(i).toObject().value(QStringLiteral("rawScan")).toString()};
            if(rawScan.isEmpty())
            {
                continue;
            }
            uploadChildWidgetList.at(i)->setPixmapFromNetwork(rawScan);
        }
    }
    this->setEnabled(true);
}

void UploadWidget::switchuploadAnswerPrecedence()
{
    this->setEnabled(false);
    if(userAnswerList.isEmpty())
    {
        getUserAnswer();
    }
    for(auto i{0}; i < uploadChildWidgetList.count(); ++i)
    {
        if(uploadChildWidgetList.at(i)->isChoiceQuestions())
        {
            const auto userAnswer{userAnswerList.at(i).toObject().value(QStringLiteral("userAnswer")).toString()};
            QBitArray answer(rightAnswer.at(i).size());
            if(!userAnswer.isEmpty())
            {
                for(auto j{0}; j < rightAnswer.at(i).size(); ++j)
                {
                    auto option{ QString(QByteArray::fromHex(QString::number(41 + j).toUtf8())) };
                    answer.setBit(j, userAnswer.contains(option));
                }
                uploadChildWidgetList.at(i)->setChecked(answer);
            }
        }
        else
        {
            const auto rawScan{userAnswerList.at(i).toObject().value(QStringLiteral("rawScan")).toString()};
            if(!rawScan.isEmpty())
            {
                uploadChildWidgetList.at(i)->setPixmapFromNetwork(rawScan);
            }
        }
    }
    this->setEnabled(true);
}

void UploadWidget::editRawData()
{
    auto tWidget{ new QWidget };
    tWidget->setAttribute(Qt::WA_DeleteOnClose);
    auto tLayout{ new QVBoxLayout(tWidget) };
    auto rawData{ QJsonDocument(getAnswerJsonObject()).toJson(QJsonDocument::Indented) };
    auto tRawDataEdit{ new QTextEdit(tWidget) };
    tRawDataEdit->setText(rawData);
    auto tUploadButton{ new QPushButton(QStringLiteral("上传"), tWidget) };
    tLayout->addWidget(tRawDataEdit);
    tLayout->addWidget(tUploadButton);

    connect(tUploadButton, &QPushButton::clicked, [this, tWidget, tRawDataEdit]
    {
        QJsonParseError error;
        auto jsonDocument{ QJsonDocument::fromJson(tRawDataEdit->toPlainText().toUtf8(), &error) };
        if(error.error == QJsonParseError::NoError)
        {
            auto state{ uploadData(jsonDocument.toJson(QJsonDocument::Compact)) };
            if(state)
            {
                tWidget->close();
            }
        }
        else
        {
            QMessageBox::warning(this, QStringLiteral("warning"), error.errorString());
        }
    });
    tWidget->show();
}

void UploadWidget::showEvent(QShowEvent *event)
{
    if(analysisWebRawDataStateChanged)
    {
        analysisWebRawDataStateChanged = false;
        analysis();
    }
    this->setEnabled(analysised && Setting::logined());
    event->accept();
}

QJsonObject UploadWidget::getAnswerJsonObject()
{
    QJsonObject rootObject;
    rootObject.insert(QStringLiteral("schoolId"), QString(Setting::schoolId()));
    rootObject.insert(QStringLiteral("studentId"), QString(Setting::studentId()));
    rootObject.insert(QStringLiteral("templateCode"), analysisWebRawData.getTemplateCode());
    QJsonArray array;
    for(auto &i : uploadChildWidgetList)
    {
        auto object{i->getJsonObject()};
        array.append(object);
    }
    rootObject.insert(QStringLiteral("answers"), array);
    return rootObject;
}

void UploadWidget::analysis()
{
    userAnswerList = QJsonArray();
    rightAnswer.clear();
    rightAnswerPrecedenceCheckBox->setAutoExclusive(false);
    uploadAnswerPrecedenceCheckBox->setAutoExclusive(false);
    rightAnswerPrecedenceCheckBox->setChecked(false);
    uploadAnswerPrecedenceCheckBox->setChecked(false);
    rightAnswerPrecedenceCheckBox->setAutoExclusive(true);
    uploadAnswerPrecedenceCheckBox->setAutoExclusive(true);

    for(auto i : uploadChildWidgetList)
    {
        uploadChildWidgetLayout->removeWidget(i);
        i->setParent(Q_NULLPTR);
        i->deleteLater();
    }

    uploadChildWidgetList.clear();
    const auto questionList{analysisWebRawData.getCountAndAnswer()};

    for(const auto &i : questionList)
    {
        rightAnswer.append(i.getAnswer());
        const auto uploadChildWidget{new UploadChildWidget(i, this)};
        uploadChildWidgetList.append(uploadChildWidget);
        uploadChildWidgetLayout->addWidget(uploadChildWidget);
    }
    analysised = true;
}
