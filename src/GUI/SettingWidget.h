#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

class SettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingWidget(QWidget *parent = nullptr);

private:
    QComboBox *userListComboBox;
    QPushButton *loginButton;
    QPushButton *logoutButton;
    QFont testFont;
    QComboBox *fontComboBox;
    QSpinBox *fontPointSizeSpinBox;
    QSpinBox *smallFontPointSizeSpinBox;
    QLabel *resultTestLabel;
    QCheckBox *listLatestTemplatePreferentiallyCheckBox;
    QCheckBox *getTemplateCodeDataAfterScanQRCodeSuccessfullyCheckBox;
    QCheckBox *compressQRCodeImageCheckBox;
    QPushButton *commonProblemButton;
    QPushButton *knownProblemButton;
    QPushButton *aboutButton;
    QPushButton *aboutQtButton;
    QLabel *currentVersionLabel;
    QPushButton *checkNewVersionButton;
    QVBoxLayout *layout;
    QPushButton *cleanTempButton;
    QLabel *showTempSize;

    void setUserList();

public slots:
    void refreshTempSize();

signals:
};

#endif // SETTINGWIDGET_H
