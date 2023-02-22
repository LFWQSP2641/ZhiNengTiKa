#include "SettingWidget.h"
#include "../StaticClass/Global.h"
#include "../StaticClass/Login.h"
#include "../StaticClass/Setting.h"

SettingWidget::SettingWidget(QWidget *parent)
    : QWidget{ parent }
{
    QGroupBox *accountGroupBox = new QGroupBox(QStringLiteral("账号"), this);
    QVBoxLayout *accountLayout = new QVBoxLayout(accountGroupBox);
    QGroupBox *appearanceGroupBox = new QGroupBox(QStringLiteral("外观(重启生效)"), this);
    QFormLayout *appearanceLayout = new QFormLayout(appearanceGroupBox);
    QGroupBox *templateListGroupBox = new QGroupBox(QStringLiteral("题卡列表"), this);
    QVBoxLayout *templateListLayout = new QVBoxLayout(templateListGroupBox);
    QGroupBox *cacheGroupBox = new QGroupBox(QStringLiteral("缓存"), this);
    QVBoxLayout *cacheLayout = new QVBoxLayout(cacheGroupBox);
    QGroupBox *problemGroupBox = new QGroupBox(QStringLiteral("问题"), this);
    QVBoxLayout *problemLayout = new QVBoxLayout(problemGroupBox);
    QGroupBox *versionGroupBox = new QGroupBox(QStringLiteral("版本"), this);
    QVBoxLayout *versionLayout = new QVBoxLayout(versionGroupBox);
    QGroupBox *aboutGroupBox = new QGroupBox(QStringLiteral("关于"), this);
    QVBoxLayout *aboutLayout = new QVBoxLayout(aboutGroupBox);

    userListComboBox = new QComboBox(this);
    loginButton = new QPushButton(QStringLiteral("登录"), this);
    logoutButton = new QPushButton(QStringLiteral("登出"), this);
    fontComboBox = new QComboBox(this);
    fontPointSizeSpinBox = new QSpinBox(this);
    smallFontPointSizeSpinBox = new QSpinBox(this);
    resultTestLabel = new QLabel(this);
    listAllTemplateCheckBox = new QCheckBox(QStringLiteral("显示所有"), this);
    getTemplateCodeDataAfterScanQRCodeSuccessfullyCheckBox = new QCheckBox(QStringLiteral("扫码成功后自动获取"), this);
    autoShowDetailWidgetAfterGetTemplateCodeDataSuccessfullyCheckBox = new QCheckBox(QStringLiteral("获取成功后自动显示答案"), this);
    cleanTempButton = new QPushButton(QStringLiteral("删除缓存"), this);
    showTempSize = new QLabel(this);
    commonProblemButton = new QPushButton(QStringLiteral("常见问题"), this);
    knownProblemButton = new QPushButton(QStringLiteral("已知问题"), this);
    currentVersionLabel = new QLabel(APP_VERSION, this);
    checkNewVersionButton = new QPushButton(QStringLiteral("检查更新"), this);
    aboutButton = new QPushButton(QStringLiteral("关于"), this);
    aboutQtButton = new QPushButton(QStringLiteral("关于Qt"), this);
    layout = new QVBoxLayout(this);

    setUserList();

    resultTestLabel->setFixedHeight(resultTestLabel->fontMetrics().height());
    resultTestLabel->setText(QStringLiteral("AbCd字体测试1234"));
    fontComboBox->addItems(QFontDatabase::families(QFontDatabase::SimplifiedChinese));
    fontComboBox->setCurrentText(Setting::font);
    smallFontPointSizeSpinBox->setValue(Setting::smallFontPointSize);
    fontPointSizeSpinBox->setValue(Setting::fontPointSize);

    listAllTemplateCheckBox->setChecked(Setting::listAllTemplate);
    getTemplateCodeDataAfterScanQRCodeSuccessfullyCheckBox->setChecked(Setting::getTemplateCodeDataAfterScanQRCodeSuccessfully);
    autoShowDetailWidgetAfterGetTemplateCodeDataSuccessfullyCheckBox->setChecked(Setting::autoShowDetailWidgetAfterGetTemplateCodeDataSuccessfully);

    auto addTwoWidgetToHBoxLayout{[](QWidget * widget1, QWidget * widget2)
    {
        auto tempLayout{new QHBoxLayout};
        tempLayout->addWidget(widget1);
        tempLayout->addWidget(widget2);
        return tempLayout;
    }};

    accountLayout->addWidget(userListComboBox);
    accountLayout->addLayout(addTwoWidgetToHBoxLayout(loginButton, logoutButton));
    appearanceLayout->addRow(QStringLiteral("字体:"), fontComboBox);
    appearanceLayout->addRow(QStringLiteral("字体大小:"), fontPointSizeSpinBox);
    appearanceLayout->addRow(QStringLiteral("小字体部件:"), smallFontPointSizeSpinBox);
    appearanceLayout->addRow(QStringLiteral("效果:"), resultTestLabel);
    templateListLayout->addWidget(listAllTemplateCheckBox);
    templateListLayout->addWidget(getTemplateCodeDataAfterScanQRCodeSuccessfullyCheckBox);
    templateListLayout->addWidget(autoShowDetailWidgetAfterGetTemplateCodeDataSuccessfullyCheckBox);
    cacheLayout->addLayout(addTwoWidgetToHBoxLayout(showTempSize, cleanTempButton));
    problemLayout->addLayout(addTwoWidgetToHBoxLayout(commonProblemButton, knownProblemButton));
    versionLayout->addLayout(addTwoWidgetToHBoxLayout(currentVersionLabel, checkNewVersionButton));
    aboutLayout->addLayout(addTwoWidgetToHBoxLayout(aboutButton, aboutQtButton));

    layout->addWidget(accountGroupBox);
    layout->addWidget(appearanceGroupBox);
    layout->addWidget(templateListGroupBox);
    layout->addWidget(cacheGroupBox);
    layout->addWidget(problemGroupBox);
    layout->addWidget(versionGroupBox);
    layout->addWidget(aboutGroupBox);


    connect(this->userListComboBox, &QComboBox::currentIndexChanged, [this](int index)
    {
        if(index > 0)
        {
            Setting::userDataList.toFirst(index);
            setUserList();
        }
    });

    connect(this->loginButton, &QPushButton::clicked, [this]
    {
        QDialog dialog(this);
        QVBoxLayout layout(&dialog);
        QFormLayout loginInfoLayout;
        QLineEdit usernameLineEdit(&dialog);
        QLineEdit passwordLineEdit(&dialog);
        QPushButton OKButton(QStringLiteral("确定"), &dialog);
        QPushButton cancelButton(QStringLiteral("取消"), &dialog);

        usernameLineEdit.setText(QStringLiteral("jcgjzx"));
        passwordLineEdit.setText(QStringLiteral("abc123"));
        usernameLineEdit.setPlaceholderText(QStringLiteral("新教育账号"));
        passwordLineEdit.setPlaceholderText(QStringLiteral("登录密码"));

        loginInfoLayout.addRow(QStringLiteral("账号:"), &usernameLineEdit);
        loginInfoLayout.addRow(QStringLiteral("密码:"), &passwordLineEdit);
        QHBoxLayout tempLayout;
        tempLayout.addWidget(&OKButton);
        tempLayout.addWidget(&cancelButton);

        layout.addLayout(&loginInfoLayout);
        layout.addLayout(&tempLayout);

        connect(&OKButton, &QPushButton::clicked, [&usernameLineEdit, &passwordLineEdit, &OKButton, this, &dialog]
        {
            OKButton.setEnabled(false);
            const auto username{usernameLineEdit.text().trimmed()};
            const auto password{passwordLineEdit.text().trimmed()};
            if(username == QStringLiteral("jcgjzx"))
            {
                QMessageBox::warning(this, QStringLiteral("warning"), QStringLiteral("请输入正确的账号"));
                return;
            }
            if(username.isEmpty() || password.isEmpty())
            {
                QMessageBox::warning(this, QStringLiteral("warning"), QStringLiteral("登录失败,请检查输入是否正确"));
                OKButton.setEnabled(true);
            }
            else
            {
                UserData newUserData;
                try
                {
                    newUserData = Login::login(username.toUtf8(), password.toUtf8());
                }
                catch(const std::exception &e)
                {
                    const QString whatStr{e.what()};
                    if(whatStr.contains(QStringLiteral("锁定")))
                    {
                        QMessageBox::information(this, QStringLiteral("information"), QStringLiteral("不用管那个锁定,大概率误报"));
                    }
                    QMessageBox::warning(this, QStringLiteral("warning"), whatStr);
                    OKButton.setEnabled(true);
                    return;
                }
                if(newUserData.detailData().isEmpty())
                {
                    auto ret{QMessageBox::question(this, QStringLiteral("question"), QStringLiteral("账号部分信息为空,是否继续使用此账号"))};
                    if(ret == QMessageBox::No)
                    {
                        dialog.close();
                        return;
                    }
                }
                Setting::userDataList.prepend(newUserData);
#ifdef Q_OS_ANDROID
                Setting::saveToFile();
#endif // Q_OS_ANDROID
                setUserList();
                QMessageBox::information(this, QStringLiteral("information"), QStringLiteral("登录成功\n当前账号:").append(newUserData.detailData().value(QStringLiteral("realName")).toString() + QStringLiteral("  ") + newUserData.username()));
                dialog.close();
            }
        });
        connect(&cancelButton, &QPushButton::clicked, [&dialog] {dialog.close(); });

        dialog.exec();
    });

    connect(this->logoutButton, &QPushButton::clicked, [this]
    {
        if(!Setting::logined())
        {
            return;
        }
        Setting::userDataList.removeFirst();
#ifdef Q_OS_ANDROID
        Setting::saveToFile();
#endif // Q_OS_ANDROID
        QMessageBox::information(this, QStringLiteral("information"), QStringLiteral("登出成功"));
        setUserList();
    });

    auto askRestart
    {
        []
        {
            QMessageBox msgBox;
            msgBox.setText(QStringLiteral("重启生效,是否关闭程序?"));
            QPushButton *yesButton = msgBox.addButton(QStringLiteral("关闭"), QMessageBox::ActionRole);
            msgBox.addButton(QStringLiteral("稍后"), QMessageBox::ActionRole);
            msgBox.setDefaultButton(yesButton);
            msgBox.exec();

            if (msgBox.clickedButton() == yesButton)
            {
                QApplication::exit(0);
            }
        }
    };

    connect(fontComboBox, &QComboBox::currentTextChanged, [this](const QString & text)
    {
        testFont.setFamily(text);
        resultTestLabel->setFont(testFont);
        resultTestLabel->setFixedHeight(QFontMetrics(testFont).height());
        Setting::font = text;
#ifdef Q_OS_ANDROID
        Setting::saveToFile();
#endif // Q_OS_ANDROID
    });
    connect(fontPointSizeSpinBox, &QSpinBox::valueChanged, [this](int i)
    {
        testFont.setPointSize(i);
        resultTestLabel->setFont(testFont);
        resultTestLabel->setFixedHeight(QFontMetrics(testFont).height());
        Setting::fontPointSize = i;
#ifdef Q_OS_ANDROID
        Setting::saveToFile();
#endif // Q_OS_ANDROID
    });
    connect(smallFontPointSizeSpinBox, &QSpinBox::valueChanged, [this](int i)
    {
        testFont.setPointSize(i);
        resultTestLabel->setFont(testFont);
        resultTestLabel->setFixedHeight(QFontMetrics(testFont).height());
        Setting::smallFontPointSize = i;
#ifdef Q_OS_ANDROID
        Setting::saveToFile();
#endif // Q_OS_ANDROID
    });

    connect(this->listAllTemplateCheckBox, &QCheckBox::stateChanged, [askRestart](int state)
    {
        Setting::listAllTemplate = (state == Qt::CheckState::Checked);
#ifdef Q_OS_ANDROID
        Setting::saveToFile();
#endif
        askRestart();
    });
    connect(this->getTemplateCodeDataAfterScanQRCodeSuccessfullyCheckBox, &QCheckBox::stateChanged, [askRestart](int state)
    {
        Setting::getTemplateCodeDataAfterScanQRCodeSuccessfully = (state == Qt::CheckState::Checked);
#ifdef Q_OS_ANDROID
        Setting::saveToFile();
#endif
        askRestart();
    });
    connect(this->autoShowDetailWidgetAfterGetTemplateCodeDataSuccessfullyCheckBox, &QCheckBox::stateChanged, [askRestart](int state)
    {
        Setting::autoShowDetailWidgetAfterGetTemplateCodeDataSuccessfully = (state == Qt::CheckState::Checked);
#ifdef Q_OS_ANDROID
        Setting::saveToFile();
#endif
        askRestart();
    });

    connect(this->commonProblemButton, &QPushButton::clicked, [this]
    {
        QMessageBox::about(this, QStringLiteral("常见问题"), QStringLiteral("<h4>1.如何使用</h4>"
                           "<p>①搜索题卡名称</p>"
                           "<p>②或使用微信等软件扫描题卡上的二维码,获取题卡编号</p>"));
    });
    connect(this->knownProblemButton, &QPushButton::clicked, [this]
    {
        QMessageBox::about(this, QStringLiteral("已知问题"),
                           QStringLiteral("<h4>1.滑动卡顿</h4>"
                                          "<h4>2.次幂显示有问题</h4>"));
    });
#if 0
    connect(this->aboutButton, &QPushButton::clicked, [this]
    {
        QMessageBox::about(this, QStringLiteral("关于"),
                           QStringLiteral(
                               "<p>使用 Qt 框架的跨平台软件,支持 Windows, Android, Linux</p>"
                               "<p>内测版</p>"
                               "<p>作者:LFWQSP2641( <a href=\"%0\">%0</a> )</p>"
                               "<p><b>只在Github有账号,其他平台同名账号与本人无关!</b></p>"
                               "<p>Copyright © 2022 - 2023 LFWQSP2641.All Rights Reserved.</p>"
                               "<br />"
                               "<p>鸣谢:</p>"
                               "<p>Qt ( <a href=\"%1\">%1</a> )</p>"
                               "<p>android_openssl ( <a href=\"%2\">%2</a> )</p>"
                               "<p>Qt-AES ( <a href=\"%3\">%3</a> )</p>"
                           ).arg(QStringLiteral("https://github.com/LFWQSP2641"),
                                 QStringLiteral("https://www.qt.io"),
                                 QStringLiteral("https://github.com/KDAB/android_openssl"),
                                 QStringLiteral("https://github.com/bricke/Qt-AES")));
    });
#else
    this->aboutButton->setEnabled(false);
#endif
    connect(this->aboutQtButton, &QPushButton::clicked, [this]
    {
        QMessageBox::aboutQt(this);
    });
    connect(this->cleanTempButton, &QPushButton::clicked, [this]
    {
        const auto result{QMessageBox::question(this, QStringLiteral("question"), QStringLiteral("程序将会自动关闭,是否继续?"))};
        if(result == QMessageBox::Yes)
        {
            Global::deleteDir(Global::tempPath());
            QApplication::exit();
        }
    });
    connect(checkNewVersionButton, &QPushButton::clicked, [this]
    {
        checkNewVersionButton->setText(QStringLiteral("正在检查..."));
        checkNewVersionButton->setEnabled(false);
        if(autoUpdate.isFinished())
        {
            autoUpdate.checkUpdate();
        }
    });
    connect(&autoUpdate, &AutoUpdate::finished, &autoUpdate, &AutoUpdate::showResultWidget);
    connect(&autoUpdate, &AutoUpdate::finished, [this]
    {
        checkNewVersionButton->setText(QStringLiteral("检查更新"));
        checkNewVersionButton->setEnabled(true);
    });
}

void SettingWidget::setUserList()
{
    userListComboBox->clear();
    if(Setting::logined())
    {
        for(const auto &i : Setting::userDataList)
        {
            userListComboBox->addItem(i.detailData().value(QStringLiteral("realName")).toString() + QStringLiteral("  ") + i.username());
        }
    }
    else
    {
        userListComboBox->addItem(QStringLiteral("未登录"));
    }
    userListComboBox->setCurrentIndex(0);
}

void SettingWidget::refreshTempSize()
{
    auto size{ Global::getDirSize(Global::tempPath()) };
    showTempSize->setText(QStringLiteral("缓存大小:%0").arg(this->locale().formattedDataSize(size)));
}
