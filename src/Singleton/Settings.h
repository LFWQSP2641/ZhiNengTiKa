#ifndef SETTINGS_H
#define SETTINGS_H

#include "../Logic/UserDataList.hpp"

class Settings : public QObject
{
    Q_OBJECT
public:
    static void initOnce();
    static void resetSingletonSettings();
    static Settings *getSingletonSettings();

    bool getListLatestTemplatePreferentially() const;
    void setListLatestTemplatePreferentially(bool newListLatestTemplatePreferentially);
    void resetListLatestTemplatePreferentially();

    bool getCompressQRCodeImage() const;
    void setCompressQRCodeImage(bool newCompressQRCodeImage);
    void resetCompressQRCodeImage();

    int getFontPointSize() const;
    void setFontPointSize(int newFontPointSize);
    void resetFontPointSize();

    QString getFont() const;
    void setFont(const QString &newFont);
    void resetFont();

    QString getQmlStyle() const;
    void setQmlStyle(const QString &newQmlStyle);
    void resetQmlStyle();

    Q_INVOKABLE bool isLogin() const;
    UserData currentUserData() const;
    UserDataList getUserDataList() const;
    UserDataList &getUserDataListReference();

    QJsonObject getJsonObjectApiQRCodeScanner() const;
    void setJsonObjectApiQRCodeScanner(const QJsonObject &newJsonObjectApiQRCodeScanner);
    void resetJsonObjectApiQRCodeScanner();

    QString getUuid() const;
    void setUuid(const QString &newUuid);
    void resetUuid();

    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

public slots:
    void saveToFile() const;
    void logout();
    void userDataListPrepend(const UserData &newUserData);
    void userDataListRemoveFirst();
    void userDataListToFirst(qsizetype index);

protected:
    explicit Settings(QObject *parent = nullptr);
    static Settings *singletonSettings;

    bool listLatestTemplatePreferentially;

    bool compressQRCodeImage;

    int fontPointSize;
    QString font;
    QString qmlStyle;

    UserDataList userDataList;

    QJsonObject jsonObjectApiQRCodeScanner;

    QString uuid;


signals:
    void listLatestTemplatePreferentiallyChanged();

    void compressQRCodeImageChanged();

    void fontPointSizeChanged();

    void fontChanged();

    void qmlStyleChanged();

    void jsonObjectApiQRCodeScannerChanged();

    void uuidChanged();

private:
    Q_PROPERTY(bool listLatestTemplatePreferentially READ getListLatestTemplatePreferentially WRITE setListLatestTemplatePreferentially RESET resetListLatestTemplatePreferentially NOTIFY listLatestTemplatePreferentiallyChanged)
    Q_PROPERTY(bool compressQRCodeImage READ getCompressQRCodeImage WRITE setCompressQRCodeImage RESET resetCompressQRCodeImage NOTIFY compressQRCodeImageChanged)
    Q_PROPERTY(int fontPointSize READ getFontPointSize WRITE setFontPointSize RESET resetFontPointSize NOTIFY fontPointSizeChanged)
    Q_PROPERTY(QString font READ getFont WRITE setFont RESET resetFont NOTIFY fontChanged)
    Q_PROPERTY(QString qmlStyle READ getQmlStyle WRITE setQmlStyle RESET resetQmlStyle NOTIFY qmlStyleChanged)
    Q_PROPERTY(QJsonObject jsonObjectApiQRCodeScanner READ getJsonObjectApiQRCodeScanner WRITE setJsonObjectApiQRCodeScanner RESET resetJsonObjectApiQRCodeScanner NOTIFY jsonObjectApiQRCodeScannerChanged)
    Q_PROPERTY(QString uuid READ getUuid WRITE setUuid RESET resetUuid NOTIFY uuidChanged)
    Q_PROPERTY(UserDataList userDataList READ getUserDataList CONSTANT)
};

#endif // SETTINGS_H
