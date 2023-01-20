#include "QRCodeScanner.h"
#include "../StaticClass/Network.h"
#include "Setting.h"

QJsonObject QRCodeScanner::apiArguments;

void QRCodeScanner::initialize()
{
}

void QRCodeScanner::initialize(const QJsonObject &apiJsonObject)
{
    if(apiJsonObject.contains(QStringLiteral("SecurityToken")))
    {
        apiArguments = apiJsonObject;
    }
}

QByteArray QRCodeScanner::scanQRCode(const QImage &image, const char *format, int quality)
{
    if(QRCodeScanner::apiArguments.value(QStringLiteral("expire")).toInt(0) < QDateTime::currentSecsSinceEpoch())
    {
        QRCodeScanner::apiInitialize();
    }

    auto generateRandomString{[]
        {
            const auto randomInt{ QRandomGenerator::global()->bounded(62) };
            QString randomString;
            if(randomInt < 10)
            {
                randomString = QString::number(randomInt);
            }
            else if(randomInt < 36)
            {
                randomString = static_cast<char>(55 + randomInt);
            }
            else
            {
                randomString = static_cast<char>(61 + randomInt);
            }
            return randomString;
        }};
    QString boundary;
    boundary.reserve(38);
    boundary.append("----WebKitFormBoundary");
    for(auto i{0}; i < 16; ++i)
    {
        boundary.append(generateRandomString());
    }

    QNetworkRequest request(QUrl(QStringLiteral("https://img.2weima.com/")));
    request.setRawHeader(QByteArrayLiteral("sec-ch-ua"), QByteArrayLiteral(R"("Not?A_Brand";v="8", "Chromium";v="108", "Microsoft Edge";v="108")"));
    request.setRawHeader(QByteArrayLiteral("sec-ch-ua-platform"), QByteArrayLiteral("\"Windows\""));
    request.setRawHeader(QByteArrayLiteral("sec-ch-ua-mobile"), QByteArrayLiteral("?0"));
    request.setRawHeader(QByteArrayLiteral("User-Agent"), QByteArrayLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36 Edg/108.0.1462.76"));
    request.setRawHeader(QByteArrayLiteral("Content-Type"), QByteArrayLiteral("multipart/form-data; boundary=").append(boundary.toUtf8()));
    request.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("*/*"));
    request.setRawHeader(QByteArrayLiteral("Origin"), QByteArrayLiteral("https://jie.2weima.com"));
    request.setRawHeader(QByteArrayLiteral("Sec-Fetch-Site"), QByteArrayLiteral("same-origin"));
    request.setRawHeader(QByteArrayLiteral("Sec-Fetch-Mode"), QByteArrayLiteral("cors"));
    request.setRawHeader(QByteArrayLiteral("Sec-Fetch-Dest"), QByteArrayLiteral("empty"));
    request.setRawHeader(QByteArrayLiteral("Referer"), QByteArrayLiteral("https://jie.2weima.com/"));
    request.setRawHeader(QByteArrayLiteral("Accept-Encoding"), QByteArrayLiteral("deflate, br"));
    request.setRawHeader(QByteArrayLiteral("Accept-Language"), QByteArrayLiteral("zh-CN,zh;q=0.9"));
    request.setRawHeader(QByteArrayLiteral("Cookie"), QString(QChar(1)).toUtf8());
    request.setRawHeader(QByteArrayLiteral("MIME-Version"), QString(QChar(1)).toUtf8());
    QHttpMultiPart multiPart(QHttpMultiPart::FormDataType);

    multiPart.setBoundary(boundary.toUtf8());

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, format, quality);

    auto initializeHttpPart{[](const QByteArray & name, const QByteArray & body)
    {
        QHttpPart textPart;
        textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QByteArrayLiteral("form-data; name=\"").append(name).append("\""));
        textPart.setBody(body);
        return textPart;
    }};
    const QByteArray baFormat{format};
    multiPart.append(initializeHttpPart(QByteArrayLiteral("id"), QByteArrayLiteral("WU_FILE_0")));
    multiPart.append(initializeHttpPart(QByteArrayLiteral("name"), QByteArrayLiteral("image.").append(baFormat)));
    multiPart.append(initializeHttpPart(QByteArrayLiteral("type"), QByteArrayLiteral("image/").append(baFormat)));
    multiPart.append(initializeHttpPart(QByteArrayLiteral("lastModifiedDate"), QDateTime::currentDateTimeUtc().toString().append(QStringLiteral(" GMT+0800 (中国标准时间)")).toUtf8()));
    multiPart.append(initializeHttpPart(QByteArrayLiteral("size"), QString::number(buffer.size()).toUtf8()));
    QString fileUploadName;
    for(auto i{0}; i < 15; ++i)
    {
        fileUploadName.append(generateRandomString());
    }
    multiPart.append(initializeHttpPart(QByteArrayLiteral("key"), QRCodeScanner::apiArguments.value(QStringLiteral("dir")).toString().append(fileUploadName).append(QStringLiteral(".")).append(format).toUtf8()));
    multiPart.append(initializeHttpPart(QByteArrayLiteral("policy"), QRCodeScanner::apiArguments.value(QStringLiteral("policy")).toString().toUtf8()));
    multiPart.append(initializeHttpPart(QByteArrayLiteral("OSSAccessKeyId"), QRCodeScanner::apiArguments.value(QStringLiteral("accessid")).toString().toUtf8()));
    multiPart.append(initializeHttpPart(QByteArrayLiteral("success_action_status"), QByteArrayLiteral("200")));
    multiPart.append(initializeHttpPart(QByteArrayLiteral("callback"), QRCodeScanner::apiArguments.value(QStringLiteral("callback")).toString().toUtf8()));
    multiPart.append(initializeHttpPart(QByteArrayLiteral("signature"), QRCodeScanner::apiArguments.value(QStringLiteral("signature")).toString().toUtf8()));
    multiPart.append(initializeHttpPart(QByteArrayLiteral("x-oss-security-token"), QRCodeScanner::apiArguments.value(QStringLiteral("SecurityToken")).toString().toUtf8()));
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QByteArrayLiteral("form-data; name=\"file\"; filename=\"image.").append(baFormat).append("\"")));
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QByteArrayLiteral("image/").append(baFormat)));
    imagePart.setBody(ba);
    multiPart.append(imagePart);

    auto replyData{Network::replyReadAll(Network::waitForFinished(Network::networkAccessManager->post(request, &multiPart)))};
    qDebug() << replyData;
    return QJsonDocument::fromJson(replyData).object().value(QStringLiteral("data")).toString().toUtf8();
}

QByteArray QRCodeScanner::scanQRCode(const QString &imagePath, const char *format, int quality)
{
    QFileInfo fileInfo(imagePath);
    if(!fileInfo.exists())
    {
        throw std::runtime_error("文件不存在");
    }
    if(format == nullptr)
    {
        return scanQRCode(QImage(imagePath), fileInfo.suffix().toStdString().c_str(), quality);
    }
    return scanQRCode(QImage(imagePath), format, quality);
}

void QRCodeScanner::apiInitialize()
{
    QNetworkRequest requestFir{QUrl(QStringLiteral("https://jie.2weima.com/"))};
    requestFir.setRawHeader(QByteArrayLiteral("sec-ch-ua"), QByteArrayLiteral(R"("Not?A_Brand";v="8", "Chromium";v="108", "Microsoft Edge";v="108")"));
    requestFir.setRawHeader(QByteArrayLiteral("sec-ch-ua-mobile"), QByteArrayLiteral("?0"));
    requestFir.setRawHeader(QByteArrayLiteral("sec-ch-ua-platform"), QByteArrayLiteral("\"Windows\""));
    requestFir.setRawHeader(QByteArrayLiteral("Upgrade-Insecure-Requests"), QByteArrayLiteral("1"));
    requestFir.setRawHeader(QByteArrayLiteral("User-Agent"), QByteArrayLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36 Edg/108.0.1462.76"));
    requestFir.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"));
    requestFir.setRawHeader(QByteArrayLiteral("Sec-Fetch-Site"), QByteArrayLiteral("same-origin"));
    requestFir.setRawHeader(QByteArrayLiteral("Sec-Fetch-Mode"), QByteArrayLiteral("navigate"));
    requestFir.setRawHeader(QByteArrayLiteral("Sec-Fetch-User"), QByteArrayLiteral("?1"));
    requestFir.setRawHeader(QByteArrayLiteral("Sec-Fetch-Dest"), QByteArrayLiteral("document"));
    requestFir.setRawHeader(QByteArrayLiteral("Accept-Encoding"), QByteArrayLiteral("deflate, br"));
    requestFir.setRawHeader(QByteArrayLiteral("Accept-Language"), QByteArrayLiteral("zh-CN,zh;q=0.9"));
    auto replyFir{Network::requestAndWaitForFinished(requestFir)};
    QString cookie(replyFir->rawHeader("Set-Cookie"));
    cookie = cookie.mid(0, cookie.indexOf(";") + 2);
    cookie.append(QStringLiteral("Hm_lvt_69f82fc88fac96306852119d0e11795e="));
    const auto currentSecsSinceEpoch{QString::number(QDateTime::currentSecsSinceEpoch())};
    cookie.append(currentSecsSinceEpoch);
    cookie.append(QStringLiteral("; Hm_lpvt_69f82fc88fac96306852119d0e11795e="));
    cookie.append(currentSecsSinceEpoch);
    QString xCsrfToken(QString::fromUtf8(Network::replyReadAll(replyFir)).mid(340, 100));
    xCsrfToken = xCsrfToken.mid(xCsrfToken.indexOf("<meta name=\"csrf-token\" content=\"") + 33, 32);
    if(cookie.isEmpty() || xCsrfToken.isEmpty())
    {
        throw std::runtime_error("返回数据为空,可能无法连接网络");
    }

    QNetworkRequest requestSec{QUrl(QStringLiteral("https://jie.2weima.com/Api/oss_sign.html?source=qrdecode"))};
    requestSec.setRawHeader(QByteArrayLiteral("Content-Length"), QByteArrayLiteral("0"));
    requestSec.setRawHeader(QByteArrayLiteral("sec-ch-ua"), QByteArrayLiteral(R"("Not?A_Brand";v="8", "Chromium";v="108", "Microsoft Edge";v="108")"));
    requestSec.setRawHeader(QByteArrayLiteral("pragma-ssl"), QByteArrayLiteral("no-cache"));
    requestSec.setRawHeader(QByteArrayLiteral("X-CSRF-TOKEN"), xCsrfToken.toUtf8());
    requestSec.setRawHeader(QByteArrayLiteral("sec-ch-ua-mobile"), QByteArrayLiteral("?0"));
    requestSec.setRawHeader(QByteArrayLiteral("User-Agent"), QByteArrayLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36 Edg/108.0.1462.76"));
    requestSec.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("*/*"));
    requestSec.setRawHeader(QByteArrayLiteral("X-Requested-With"), QByteArrayLiteral("XMLHttpRequest"));
    requestSec.setRawHeader(QByteArrayLiteral("sec-ch-ua-platform"), QByteArrayLiteral("\"Windows\""));
    requestSec.setRawHeader(QByteArrayLiteral("Origin"), QByteArrayLiteral("https://jie.2weima.com"));
    requestSec.setRawHeader(QByteArrayLiteral("Sec-Fetch-Site"), QByteArrayLiteral("same-origin"));
    requestSec.setRawHeader(QByteArrayLiteral("Sec-Fetch-Mode"), QByteArrayLiteral("cors"));
    requestSec.setRawHeader(QByteArrayLiteral("Sec-Fetch-Dest"), QByteArrayLiteral("empty"));
    requestSec.setRawHeader(QByteArrayLiteral("Referer"), QByteArrayLiteral("https://jie.2weima.com/"));
    requestSec.setRawHeader(QByteArrayLiteral("Accept-Encoding"), QByteArrayLiteral("deflate, br"));
    requestSec.setRawHeader(QByteArrayLiteral("Accept-Language"), QByteArrayLiteral("zh-CN,zh;q=0.9"));
    requestSec.setRawHeader(QByteArrayLiteral("Cookie"), cookie.toUtf8());

    const auto replyDataSec{Network::getData(requestSec)};
    apiArguments = QJsonDocument::fromJson(replyDataSec).object();
    if(apiArguments.isEmpty())
    {
        qWarning() << "QRCodeScanner::apiInitialize() : apiArguments.isEmpty()";
        qWarning() << replyDataSec;
    }
    else
    {
        Setting::jsonObjectApiQRCodeScanner = QRCodeScanner::apiArguments;
#ifdef Q_OS_ANDROID
        Setting::saveToFile();
#endif // Q_OS_ANDROID
    }
}
