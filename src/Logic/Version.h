#ifndef VERSION_H
#define VERSION_H


class Version
{
public:
    Version();
    explicit Version(const QString &versionString);
    Version(int major, int minor, int revision);

    QString toString() const;
    bool isEmpty() const;

    bool operator<(const Version &other) const;
    bool operator<=(const Version &other) const;
    bool operator>(const Version &other) const;
    bool operator>=(const Version &other) const;
    bool operator==(const Version &other) const;
    bool operator!=(const Version &other) const;
protected:
    QStringList versionData;
};

Q_DECLARE_METATYPE(Version);
QDebug operator<<(QDebug dbg, const Version &version);

#endif // VERSION_H
