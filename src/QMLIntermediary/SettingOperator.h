#ifndef SETTINGOPERATOR_H
#define SETTINGOPERATOR_H

#include <QObject>

class SettingOperator : public QObject
{
    Q_OBJECT
public:
    explicit SettingOperator(QObject *parent = nullptr);

    Q_INVOKABLE bool login(const QString &id, const QString &pw);
    Q_INVOKABLE bool logined();
    Q_INVOKABLE QStringListModel *getUserDataListModel();
    Q_INVOKABLE QString getStyle();
    Q_INVOKABLE void setStyle(const QString &style);
    Q_INVOKABLE QString getFont();
    Q_INVOKABLE void setFont(const QString &font);
    Q_INVOKABLE bool getListLatestTemplatePreferentially();
    Q_INVOKABLE void setListLatestTemplatePreferentially(bool state);
    Q_INVOKABLE bool getCompressQRCodeImage();
    Q_INVOKABLE void setCompressQRCodeImage(bool state);
    Q_INVOKABLE void deleteTemplateFile();
    Q_INVOKABLE void deleteCacheImage();
    Q_INVOKABLE QString getVersion();
public slots:
    void save();
    void logout();
    void userDataListToFirst(qsizetype index);

protected:
    QStringListModel *userDataListModel = nullptr;

signals:

};

#endif // SETTINGOPERATOR_H
