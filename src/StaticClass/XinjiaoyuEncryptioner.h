#pragma once


namespace XinjiaoyuEncryptioner
{
extern QByteArray xinjiaoyuEncryption(const QByteArray &rawText);
extern QByteArray xinjiaoyuDecryption(const QByteArray &rawText);
extern QByteArray getXinjiaoyuMD5(const QByteArray &tValue, const QByteArray &clientSessionValue);

extern QAESEncryption encryptioner;
extern const QByteArray xinjiaoyuKey;
};

