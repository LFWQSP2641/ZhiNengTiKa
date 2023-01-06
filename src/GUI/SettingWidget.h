#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include "../Logic/AutoUpdate.h"

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
    QCheckBox *listAllTemplateCheckBox;
    QPushButton *commonProblemButton;
    QPushButton *knownProblemButton;
    QPushButton *aboutButton;
    QPushButton *aboutQtButton;
    QLabel *currentVersionLabel;
    QPushButton *checkNewVersionButton;
    QVBoxLayout *layout;
    QPushButton *cleanTempButton;
    QLabel *showTempSize;
    AutoUpdate autoUpdate = AutoUpdate(APP_VERSION);

    void setUserList();

public slots:
    void refreshTempSize();

signals:
};

#endif // SETTINGWIDGET_H
