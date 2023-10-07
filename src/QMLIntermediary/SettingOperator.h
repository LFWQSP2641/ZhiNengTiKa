#ifndef SETTINGOPERATOR_H
#define SETTINGOPERATOR_H

class SettingOperator : public QObject
{
    Q_OBJECT
public:
    explicit SettingOperator(QObject *parent = nullptr);

    Q_INVOKABLE QString getVersion() const;
    Q_INVOKABLE QVariant getAnimeImageNameList() const;
    Q_INVOKABLE QString getAnimeImageUrl(qsizetype index) const;
    Q_INVOKABLE qsizetype getCurrentAnimeImageNameIndex() const;
public slots:
    void deleteTemplateFile();
signals:
};

#endif // SETTINGOPERATOR_H
