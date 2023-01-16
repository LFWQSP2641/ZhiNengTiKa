#include "XinjiaoyuEncryptioner.h"

QAESEncryption XinjiaoyuEncryptioner::encryptioner(QAESEncryption::AES_256, QAESEncryption::ECB, QAESEncryption::PKCS7);
const QByteArray XinjiaoyuEncryptioner::xinjiaoyuKey(QByteArrayLiteral("6f0c5ba452b24fa28989e9524d77407a"));


QByteArray XinjiaoyuEncryptioner::xinjiaoyuEncryption(const QByteArray &rawText)
{
    return XinjiaoyuEncryptioner::encryptioner.encode(rawText, XinjiaoyuEncryptioner::xinjiaoyuKey).toBase64();
}

QByteArray XinjiaoyuEncryptioner::xinjiaoyuDecryption(const QByteArray &rawText)
{
    try
    {
        const auto rawResult1{QByteArray::fromBase64(rawText)};
        const auto rawResult2{XinjiaoyuEncryptioner::encryptioner.decode(rawResult1, XinjiaoyuEncryptioner::xinjiaoyuKey)};
        const auto result{XinjiaoyuEncryptioner::encryptioner.removePadding(rawResult2)};
        return result;
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(Q_NULLPTR, QStringLiteral("critical"), e.what());
    }
    return QByteArray();
}

QByteArray XinjiaoyuEncryptioner::getXinjiaoyuMD5(const QByteArray &tValue, const QByteArray &clientSessionValue)
{
    return QCryptographicHash::hash(QByteArrayLiteral("jbyxinjiaoyu").append(tValue).append(clientSessionValue).append("android"), QCryptographicHash::Md5).toHex();
}
