#include "QRCodeScanner.h"
#include "../Singleton/Settings.h"
#include "../Singleton/Network.h"

QRCodeScanner::QRCodeScanner(QObject *parent)
    : QObject{parent},
      intervalTimer(new QTimer(this))
{
    intervalTimer->setInterval(this->interval);
}

void QRCodeScanner::scanQRCode(const QImage &image)
{
    this->finish = false;
    auto settings(Settings::getSingletonSettings());
    // 检查api参数是否过期
    if(settings->getJsonObjectApiQRCodeScanner().value(QStringLiteral("expire")).toInt(0) < QDateTime::currentSecsSinceEpoch())
    {
        // 重新获取api参数
        if(!this->initializeApi())
        {
            // 获取api参数失败
            emit this->analysisFinished(QString());
            this->finish = true;
            return;
        }
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

    QNetworkRequest request(QStringLiteral("https://img.2weima.com/"));
    request.setRawHeader(QByteArrayLiteral("sec-ch-ua"), QByteArrayLiteral(R"("Not.A/Brand";v="8", "Chromium";v="114", "Google Chrome";v="114")"));
    request.setRawHeader(QByteArrayLiteral("sec-ch-ua-platform"), QByteArrayLiteral("\"Windows\""));
    request.setRawHeader(QByteArrayLiteral("sec-ch-ua-mobile"), QByteArrayLiteral("?0"));
    request.setRawHeader(QByteArrayLiteral("User-Agent"), QByteArrayLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36"));
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
    auto multiPart(new QHttpMultiPart(QHttpMultiPart::FormDataType));

    multiPart->setBoundary(boundary.toUtf8());

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    if(settings->getCompressQRCodeImage())
    {
        if(image.size().width() > 1600 || image.size().height() > 900)
        {
            image.convertToFormat(QImage::Format_Grayscale8).scaled(QSize(1600, 900), Qt::KeepAspectRatio).save(&buffer, "JPG", 70);
        }
        else
        {
            image.convertToFormat(QImage::Format_Grayscale8).save(&buffer, "JPG", 70);
        }
    }
    else
    {
        image.save(&buffer, "PNG");
    }

    auto initializeHttpPart{[](const QByteArray & name, const QByteArray & body)
    {
        QHttpPart textPart;
        textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QByteArrayLiteral("form-data; name=\"").append(name).append("\""));
        textPart.setBody(body);
        return textPart;
    }};
    const QByteArray baFormat{QImageReader(ba).format()};
    multiPart->append(initializeHttpPart(QByteArrayLiteral("id"), QByteArrayLiteral("WU_FILE_0")));
    multiPart->append(initializeHttpPart(QByteArrayLiteral("name"), QByteArrayLiteral("image.").append(baFormat)));
    multiPart->append(initializeHttpPart(QByteArrayLiteral("type"), QByteArrayLiteral("image/").append(baFormat)));
    multiPart->append(initializeHttpPart(QByteArrayLiteral("lastModifiedDate"), QDateTime::currentDateTimeUtc().toString().append(QStringLiteral(" GMT+0800 (中国标准时间)")).toUtf8()));
    multiPart->append(initializeHttpPart(QByteArrayLiteral("size"), QString::number(buffer.size()).toUtf8()));
    QString fileUploadName;
    for(auto i{0}; i < 15; ++i)
    {
        fileUploadName.append(generateRandomString());
    }
    const auto apiJsonObject(settings->getJsonObjectApiQRCodeScanner());
    multiPart->append(initializeHttpPart(QByteArrayLiteral("key"), apiJsonObject.value(QStringLiteral("dir")).toString().append(fileUploadName).append(QStringLiteral(".")).append(baFormat).toUtf8()));
    multiPart->append(initializeHttpPart(QByteArrayLiteral("policy"), apiJsonObject.value(QStringLiteral("policy")).toString().toUtf8()));
    multiPart->append(initializeHttpPart(QByteArrayLiteral("OSSAccessKeyId"), apiJsonObject.value(QStringLiteral("accessid")).toString().toUtf8()));
    multiPart->append(initializeHttpPart(QByteArrayLiteral("success_action_status"), QByteArrayLiteral("200")));
    multiPart->append(initializeHttpPart(QByteArrayLiteral("callback"), apiJsonObject.value(QStringLiteral("callback")).toString().toUtf8()));
    multiPart->append(initializeHttpPart(QByteArrayLiteral("signature"), apiJsonObject.value(QStringLiteral("signature")).toString().toUtf8()));
    multiPart->append(initializeHttpPart(QByteArrayLiteral("x-oss-security-token"), apiJsonObject.value(QStringLiteral("SecurityToken")).toString().toUtf8()));
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(QByteArrayLiteral("form-data; name=\"file\"; filename=\"image.").append(baFormat).append("\"")));
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QByteArrayLiteral("image/").append(baFormat)));
    imagePart.setBody(ba);
    multiPart->append(imagePart);

    auto reply(Network::getGlobalNetworkManager()->post(request, multiPart));
    connect(reply, &QNetworkReply::finished, [reply, this]
    {
        const auto data(Network::getGlobalNetworkManager()->replyReadAll(reply));
        const auto result(QJsonDocument::fromJson(data).object().value(QStringLiteral("data")).toString().toUtf8());
        qDebug() << "scanQRCodeResultData:" << data;
        emit this->analysisFinished(result);
    });
    this->finish = true;
}

bool QRCodeScanner::getFinish() const
{
    return finish;
}

bool QRCodeScanner::getContinueScan() const
{
    return continueScan;
}

int QRCodeScanner::getInterval() const
{
    return interval;
}

void QRCodeScanner::setInterval(int newInterval)
{
    if (interval == newInterval)
        return;
    interval = newInterval;
    intervalTimer->setInterval(newInterval);
    emit intervalChanged();
}

QVideoSink *QRCodeScanner::getVideoSink() const
{
    return videoSink;
}

void QRCodeScanner::setVideoSink(QVideoSink *newVideoSink)
{
    if (videoSink == newVideoSink)
    {
        return;
    }
    if(this->videoSink != nullptr)
    {
        this->intervalTimer->stop();
        disconnect(this->videoSink, &QVideoSink::videoFrameChanged, this, &QRCodeScanner::readVideoFrame);
        disconnect(this->intervalTimer, &QTimer::timeout, nullptr, nullptr);
    }
    this->videoSink = newVideoSink;
    // 频率太快了, 如果是本地解析还好
    // 但是这用的是别人的服务器, 限制一下
    connect(newVideoSink, &QVideoSink::videoFrameChanged, this, &QRCodeScanner::readVideoFrame);
    connect(this->intervalTimer, &QTimer::timeout, [this]
    {
        this->continueScan = true;
    });
    this->intervalTimer->start();
    emit videoSinkChanged();
}

bool QRCodeScanner::initializeApi()
{
    emit this->apiInitializing();
    QNetworkRequest requestFir{QStringLiteral("https://jie.2weima.com/")};
    requestFir.setRawHeader(QByteArrayLiteral("sec-ch-ua"), QByteArrayLiteral(R"("Not.A/Brand";v="8", "Chromium";v="114", "Google Chrome";v="114")"));
    requestFir.setRawHeader(QByteArrayLiteral("sec-ch-ua-mobile"), QByteArrayLiteral("?0"));
    requestFir.setRawHeader(QByteArrayLiteral("sec-ch-ua-platform"), QByteArrayLiteral("\"Windows\""));
    requestFir.setRawHeader(QByteArrayLiteral("Upgrade-Insecure-Requests"), QByteArrayLiteral("1"));
    requestFir.setRawHeader(QByteArrayLiteral("User-Agent"), QByteArrayLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36"));
    requestFir.setRawHeader(QByteArrayLiteral("Accept"), QByteArrayLiteral("text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7"));
    requestFir.setRawHeader(QByteArrayLiteral("Sec-Fetch-Site"), QByteArrayLiteral("same-origin"));
    requestFir.setRawHeader(QByteArrayLiteral("Sec-Fetch-Mode"), QByteArrayLiteral("navigate"));
    requestFir.setRawHeader(QByteArrayLiteral("Sec-Fetch-User"), QByteArrayLiteral("?1"));
    requestFir.setRawHeader(QByteArrayLiteral("Sec-Fetch-Dest"), QByteArrayLiteral("document"));
    requestFir.setRawHeader(QByteArrayLiteral("Accept-Encoding"), QByteArrayLiteral("deflate, br"));
    requestFir.setRawHeader(QByteArrayLiteral("Accept-Language"), QByteArrayLiteral("zh-CN,zh;q=0.9"));
    auto replyFir{Network::getGlobalNetworkManager()->getAndWaitForFinished(requestFir)};
    QString cookie(replyFir->rawHeader("Set-Cookie"));
    cookie = cookie.mid(0, cookie.indexOf(";") + 2);
    cookie.append(QStringLiteral("Hm_lvt_69f82fc88fac96306852119d0e11795e="));
    const auto currentSecsSinceEpoch{QString::number(QDateTime::currentSecsSinceEpoch())};
    cookie.append(currentSecsSinceEpoch);
    cookie.append(QStringLiteral("; Hm_lpvt_69f82fc88fac96306852119d0e11795e="));
    cookie.append(currentSecsSinceEpoch);
    QString xCsrfToken(QString::fromUtf8(Network::getGlobalNetworkManager()->replyReadAll(replyFir)).mid(340, 100));
    xCsrfToken = xCsrfToken.mid(xCsrfToken.indexOf("<meta name=\"csrf-token\" content=\"") + 33, 32);
    if(cookie.isEmpty() || xCsrfToken.isEmpty())
    {
        emit this->initializeApiFinished(false);
        return false;
    }

    QNetworkRequest requestSec{QStringLiteral("https://jie.2weima.com/Api/oss_sign.html?source=qrdecode")};
    requestSec.setRawHeader(QByteArrayLiteral("Content-Length"), QByteArrayLiteral("0"));
    requestSec.setRawHeader(QByteArrayLiteral("sec-ch-ua"), QByteArrayLiteral(R"("Not.A/Brand";v="8", "Chromium";v="114", "Google Chrome";v="114")"));
    requestSec.setRawHeader(QByteArrayLiteral("pragma-ssl"), QByteArrayLiteral("no-cache"));
    requestSec.setRawHeader(QByteArrayLiteral("X-CSRF-TOKEN"), xCsrfToken.toUtf8());
    requestSec.setRawHeader(QByteArrayLiteral("sec-ch-ua-mobile"), QByteArrayLiteral("?0"));
    requestSec.setRawHeader(QByteArrayLiteral("User-Agent"), QByteArrayLiteral("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36"));
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

    const auto replyDataSec{Network::getGlobalNetworkManager()->getData(requestSec)};
    auto apiArguments = QJsonDocument::fromJson(replyDataSec).object();
    if(apiArguments.isEmpty())
    {
        qWarning() << "QRCodeScanner::apiInitialize() : apiArguments.isEmpty()";
        qWarning() << replyDataSec;
        emit this->initializeApiFinished(false);
        return false;
    }
    else
    {
        Settings::getSingletonSettings()->setJsonObjectApiQRCodeScanner(apiArguments);
#ifdef Q_OS_ANDROID
        Settings::getSingletonSettings()->saveToFile();
#endif // Q_OS_ANDROID
        emit this->initializeApiFinished(true);
        return true;
    }
}

void QRCodeScanner::readVideoFrame(const QVideoFrame &frame)
{
    if(this->finish && this->continueScan)
    {
        this->scanQRCode(frame.toImage());
    }
    this->continueScan = false;
}
