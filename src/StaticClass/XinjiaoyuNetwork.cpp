#include "XinjiaoyuNetwork.h"
#include "Setting.h"
#include "Network.h"
#include "Login.h"
#include "XinjiaoyuEncryptioner.h"

QNetworkRequest XinjiaoyuNetwork::setRequest(const QUrl &url)
{
    QNetworkRequest requestInfo;
    requestInfo.setUrl(QUrl(url));
    requestInfo.setRawHeader(QByteArrayLiteral("Accept-Encoding"), QByteArrayLiteral("gzip"));
    requestInfo.setRawHeader(QByteArrayLiteral("app"), QByteArrayLiteral("student"));
    requestInfo.setRawHeader(QByteArrayLiteral("client"), QByteArrayLiteral("android"));
    requestInfo.setRawHeader(QByteArrayLiteral("Content-Type"), QByteArrayLiteral("application/json"));
    requestInfo.setRawHeader(QByteArrayLiteral("User-Agent"), QByteArrayLiteral("okhttp/4.9.3"));
    requestInfo.setRawHeader(QByteArrayLiteral("Authorization"), Setting::authorization());
    requestInfo.setRawHeader(QByteArrayLiteral("accessToken"), Setting::accessToken());
    requestInfo.setRawHeader(QByteArrayLiteral("clientSession"), Setting::clientSession());

    const auto tVal{QString::number(QDateTime::currentMSecsSinceEpoch()).toUtf8()};
    requestInfo.setRawHeader(QByteArrayLiteral("t"), tVal);
    const auto encryptVal{XinjiaoyuEncryptioner::getXinjiaoyuMD5(tVal, Setting::clientSession())};
    requestInfo.setRawHeader(QByteArrayLiteral("encrypt"), encryptVal);
    return requestInfo;
}

QByteArray XinjiaoyuNetwork::getTemplateCodeData(const QString &templateCode)
{
    if(!Setting::logined())
    {
        QMessageBox::warning(nullptr, QStringLiteral("warning"), QStringLiteral("未登录"));
        return QByteArray();
    }
    QByteArray responseByte;
    responseByte = Network::getData(
                       XinjiaoyuNetwork::setRequest(
                           QStringLiteral("https://www.xinjiaoyu.com/api/v3/server_homework/"
                                          "homework/template/question/list?templateCode=%0&studentId=%1&isEncrypted=true").arg(templateCode, Setting::studentId())));

    const auto stateCode{responseByte.mid(8, 3)};

    if (stateCode == QByteArrayLiteral("200"))
    {
        //解密
        auto funcDecryption{[](const QJsonObject & object)
        {
            QJsonObject question{ object.value(QStringLiteral("question")).toObject() };

            auto answerContent{question.value(QStringLiteral("answer")).toString()};
            auto answerExplanation{ question.value(QStringLiteral("answerExplanation")).toString() };

            if(!answerContent.isEmpty())
            {
                answerContent = XinjiaoyuEncryptioner::xinjiaoyuDecryption(answerContent.toUtf8());
                question.insert(QStringLiteral("answer"), answerContent);
            }
            if(!answerExplanation.isEmpty())
            {
                answerExplanation = XinjiaoyuEncryptioner::xinjiaoyuDecryption(answerExplanation.toUtf8());
                question.insert(QStringLiteral("answerExplanation"), answerExplanation);
            }

            return question;
        }};

        QJsonObject tempObject(QJsonDocument::fromJson(responseByte).object());

        QJsonArray array{ tempObject.value(QStringLiteral("data")).toObject().value(QStringLiteral("questions")).toArray().at(0).toObject().value(QStringLiteral("questionsAnswers")).toArray() };

        QJsonArray newArray;
        for (auto j{ 0 }; j < array.size(); ++j)
        {
            auto object{array[j].toObject()};
            object.insert(QStringLiteral("question"), funcDecryption(object));

            QJsonArray childQuestionList{ object.value(QStringLiteral("childQuestionList")).toArray() };
            if (!childQuestionList.isEmpty())
            {
                QJsonArray childQuestionArray;
                for (auto i{ 0 }; i < childQuestionList.size(); ++i)
                {
                    auto obj{childQuestionList[i].toObject()};
                    obj.insert(QStringLiteral("question"), funcDecryption(childQuestionList[i].toObject()));
                    childQuestionArray.append(obj);
                }
                object.insert(QStringLiteral("childQuestionList"), childQuestionArray);
            }
            newArray.append(object);
        }

        //md真的要一层一层叠上去?
        auto obj2{tempObject.value(QStringLiteral("data")).toObject().value(QStringLiteral("questions")).toArray().at(0).toObject()};
        obj2.insert(QStringLiteral("questionsAnswers"), newArray);
        QJsonArray arr3;
        arr3.append(obj2);
        auto obj4{tempObject.value(QStringLiteral("data")).toObject()};
        obj4.insert(QStringLiteral("questions"), arr3);
        tempObject.insert(QStringLiteral("data"), obj4);
        //6

        responseByte = QJsonDocument(tempObject).toJson(QJsonDocument::Compact);
        return responseByte;
    }
    else if (responseByte.isEmpty())
    {
        QMessageBox::warning(nullptr, QStringLiteral("warning"), QStringLiteral("请检查网络连接\n") + QString(responseByte));
    }
    else
    {
        QMessageBox::warning(nullptr, QStringLiteral("warning"), QString(responseByte));
    }
    return QByteArray();
}

QString XinjiaoyuNetwork::uploadFile(const QByteArray &fileData, const QString &fileName)
{
    const auto returnData{ Network::replyReadAll(Network::waitForFinished(uploadFileReply(fileData, fileName))) };

    if(returnData.mid(8, 3) != QByteArrayLiteral("200"))
    {
        return QString();
    }

    auto fileUrl{ QJsonDocument::fromJson(returnData).object().value("data").toObject().value("accessUrl").toString() };
    const auto lastSlashIndex{fileUrl.lastIndexOf("/")};
    fileUrl.remove(lastSlashIndex + 1, fileUrl.size() - 1);
    fileUrl.append(fileName.toUtf8().toPercentEncoding());
    return fileUrl;
}

QNetworkReply *XinjiaoyuNetwork::uploadFileReply(const QByteArray &fileData, const QString &fileName)
{
    const auto uuid{QUuid::createUuid().toString(QUuid::WithoutBraces)};
    QNetworkRequest request;
    request = XinjiaoyuNetwork::setRequest(QStringLiteral("https://www.xinjiaoyu.com/api/v3/server_homework/homework/upload"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QString(QString("multipart/form-data; boundary=") + uuid).toUtf8());

    QByteArray data;
    data.append("--");
    data.append(uuid.toUtf8());
    data.append("\r\n");
    data.append("Content-Disposition: form-data; name=\"file\"; filename=\"");
    data.append(fileName.toUtf8());
    data.append("\"\r\n"
                "Content-Type: multipart/form-data\r\n"
                "Content-Length: ");
    data.append(QString::number(fileData.size()).toUtf8());
    data.append("\r\n\r\n");
    data.append(fileData);
    data.append("\r\n--");
    data.append(uuid.toUtf8());
    data.append("--");

    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());

    return Network::networkAccessManager.post(request, data);
}
