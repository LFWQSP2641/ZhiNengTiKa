#include "Version.h"

Version::Version()
    : versionData({QStringLiteral("0"), QStringLiteral("0"), QStringLiteral("0")})
{

}

Version::Version(const QString &versionString)
    : versionData(versionString.split(QStringLiteral(".")))
{
}

Version::Version(int major, int minor, int revision)
    : versionData({QString::number(major), QString::number(minor), QString::number(revision)})
{

}

QString Version::toString() const
{
    QString string;
    for(const auto &i : versionData)
    {
        string.append(i);
        string.append(QStringLiteral("."));
    }
    string.resize(string.size() - 1);
    string.squeeze();
    return string;
}

bool Version::operator<(const Version &other) const
{
    for (int i = 0; i < qMin(this->versionData.size(), other.versionData.size()); ++i)
    {
        int thisPart = versionData.at(i).toInt();
        int otherPart = other.versionData.at(i).toInt();

        if (thisPart < otherPart)
        {
            return true;
        }
        else if (thisPart > otherPart)
        {
            return false;
        }
    }

    return false;
}

bool Version::operator<=(const Version &other) const
{
    return (*this < other) || (*this == other);
}

bool Version::operator>(const Version &other) const
{
    return !(*this <= other);
}

bool Version::operator>=(const Version &other) const
{
    return !(*this < other);
}

bool Version::operator==(const Version &other) const
{
    return versionData == other.versionData;
}

bool Version::operator!=(const Version &other) const
{
    return !(*this == other);
}

QDebug operator<<(QDebug dbg, const Version &version)
{
    dbg << version.toString();
    return dbg;
}
