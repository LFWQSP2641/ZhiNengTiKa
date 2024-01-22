#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

class Settings;

class SettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingWidget(QWidget *parent = nullptr);

private:
    Settings *settings;

    QComboBox *userListComboBox;
    QPushButton *loginButton;
    QPushButton *logoutButton;
    QFont testFont;
    QComboBox *fontComboBox;
    QSpinBox *fontPointSizeSpinBox;
    QLabel *resultTestLabel;
    QComboBox *schoolNameComboBox;
    QComboBox *levelComboBox;
    QPushButton *commonProblemButton;
    QPushButton *knownProblemButton;
    QPushButton *aboutButton;
    QPushButton *aboutQtButton;
    QPushButton *usedSourceButton;
    QPushButton *openSourceLicenseButton;
    QLabel *currentVersionLabel;
    QPushButton *checkNewVersionButton;
    QVBoxLayout *layout;
    QPushButton *cleanTempButton;
    QLabel *showTempSize;

    void setUserList();

public slots:
    void refreshTempSize();
    void askRestart();

signals:
};

#endif // SETTINGWIDGET_H
