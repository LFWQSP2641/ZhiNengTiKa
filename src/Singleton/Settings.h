#ifndef SETTINGS_H
#define SETTINGS_H

#include "../Logic/AccountManager.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    static void initOnce();
    static void resetSingletonSettings();
    static Settings *getSingletonSettings();

    int getFontPointSize() const;
    void setFontPointSize(int newFontPointSize);
    void resetFontPointSize();

    QString getFont() const;
    void setFont(const QString &newFont);
    void resetFont();

    QString getQmlStyle() const;
    void setQmlStyle(const QString &newQmlStyle);
    void resetQmlStyle();

    QString getUuid() const;
    void setUuid(const QString &newUuid);
    void resetUuid();

    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    QString getAnimeImageUrl() const;
    void setAnimeImageUrl(const QString &newAnimeImageUrl);
    void resetAnimeImageUrl();

    const QList<QPair<QString, QString>> animeImageUrlList = QList<QPair<QString, QString>>()
                                      << QPair<QString, QString>("IMGAPI(动漫竖屏)", "https://imgapi.cn/api.php?zd=mobile&fl=dongman&gs=images")
                                      << QPair<QString, QString>("IMGAPI(动漫横屏)", "https://imgapi.cn/api.php?zd=pc&fl=dongman&gs=images")
                                      << QPair<QString, QString>("IMGAPI(风景竖屏)", "https://imgapi.cn/api.php?zd=mobile&fl=fengjing&gs=images")
                                      << QPair<QString, QString>("IMGAPI(风景横屏)", "https://imgapi.cn/api.php?zd=pc&fl=fengjing&gs=images")
                                      << QPair<QString, QString>("t.mwm.moe(二次元随机图片API)(风景横屏)", "https://t.mwm.moe/fj")
                                      << QPair<QString, QString>("t.mwm.moe(二次元随机图片API)(竖屏)", "https://t.mwm.moe/mp")
                                      << QPair<QString, QString>("t.mwm.moe(二次元随机图片API)(横屏)", "https://t.mwm.moe/pc")
                                      << QPair<QString, QString>("樱道(动漫综合1~18)", "https://api.r10086.com/樱道随机图片api接口.php?图片系列=动漫综合^(1~18)^")
                                      << QPair<QString, QString>("樱道(某科学的超电磁炮竖屏系列)", "https://api.r10086.com/樱道随机图片api接口.php?图片系列=某科学的超电磁炮竖屏系列1")
                                      << QPair<QString, QString>("樱道(某科学的超电磁炮横屏系列)", "https://api.r10086.com/樱道随机图片api接口.php?图片系列=某科学的超电磁炮横屏系列1")
                                      << QPair<QString, QString>("樱道(原神竖屏系列)", "https://api.r10086.com/樱道随机图片api接口.php?图片系列=原神竖屏系列1")
                                      << QPair<QString, QString>("樱道(原神横屏系列)", "https://api.r10086.com/樱道随机图片api接口.php?图片系列=原神横屏系列1")
                                      << QPair<QString, QString>("樱道(物语系列1)", "https://api.r10086.com/樱道随机图片api接口.php?图片系列=物语系列1")
                                      << QPair<QString, QString>("樱道(物语系列2)", "https://api.r10086.com/樱道随机图片api接口.php?图片系列=物语系列2")
                                      << QPair<QString, QString>("超级小兔的随机图", "https://imgapi.xl0408.top/index.php")
                                      << QPair<QString, QString>("岁月小筑", "https://img.xjh.me/random_img.php")
                                      << QPair<QString, QString>("韩小韩API接口站", "https://api.vvhan.com/api/acgimg")
                                      << QPair<QString, QString>("保罗API", "https://api.paugram.com/wallpaper")
                                      << QPair<QString, QString>("樱花API", "https://www.dmoe.cc/random.php")
                                      << QPair<QString, QString>("EEEDOG", "https://api.yimian.xyz/img")
                                      << QPair<QString, QString>("paulzzh(东方Project)", "https://img.paulzzh.com/touhou/random")
                                      << QPair<QString, QString>("dmoe(二次元)", "http://www.dmoe.cc/random.php");

    QList<QPair<QString, QString> > getAnimeImageUrlList() const;

    AccountManager *getAccountManager() const;

public slots:
    void saveToFile() const;

protected:
    explicit Settings(QObject *parent = nullptr);
    static Settings *singletonSettings;

    int fontPointSize;
    QString font;
    QString qmlStyle;

    AccountManager *accountManager;

    QString uuid;

    QString animeImageUrl;

signals:
    void fontPointSizeChanged();

    void fontChanged();

    void qmlStyleChanged();

    void uuidChanged();

    void animeImageUrlChanged();

private:
    Q_PROPERTY(int fontPointSize READ getFontPointSize WRITE setFontPointSize RESET resetFontPointSize NOTIFY fontPointSizeChanged)
    Q_PROPERTY(QString font READ getFont WRITE setFont RESET resetFont NOTIFY fontChanged)
    Q_PROPERTY(QString qmlStyle READ getQmlStyle WRITE setQmlStyle RESET resetQmlStyle NOTIFY qmlStyleChanged)
    Q_PROPERTY(QString uuid READ getUuid WRITE setUuid RESET resetUuid NOTIFY uuidChanged)
    Q_PROPERTY(QString animeImageUrl READ getAnimeImageUrl WRITE setAnimeImageUrl RESET resetAnimeImageUrl NOTIFY animeImageUrlChanged FINAL)
    Q_PROPERTY(AccountManager *accountManager READ getAccountManager CONSTANT FINAL)
};

#endif // SETTINGS_H
