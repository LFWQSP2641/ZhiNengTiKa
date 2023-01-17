#include "SearchWidget.h"
#include "../StaticClass/Global.h"
#include "../StaticClass/XinjiaoyuNetwork.h"
#include "../StaticClass/Setting.h"
#include "../StaticClass/CallAndroidNativeComponent.h"
#include "../StaticClass/QRCodeScanner.h"

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget{ parent }
{
    mainLayout = new QVBoxLayout(this);
    listWidget = new QListWidget(this);
    allListWidget = new QTabWidget(this);
    resetButton = new QPushButton(QStringLiteral("重置"), this);
    OKButton = new QPushButton(QStringLiteral("确定"), this);
    searchButton = new QPushButton(QStringLiteral("搜索"), this);
    previousPageButton = new QPushButton(QStringLiteral("上一页"), this);
    nextPageButton = new QPushButton(QStringLiteral("下一页"), this);
    selectTemplateIdButton = new QPushButton(QStringLiteral("获取"), this);
    scanQRCodeButton = new QPushButton(QStringLiteral("扫码"), this);
    searchLineEdit = new QLineEdit(this);
    templateCodeLineEdit = new QLineEdit(this);

    //TODO 重构导入题卡列表
    const auto subjects{ QStringList() << QStringLiteral("语文") << QStringLiteral("数学") << QStringLiteral("英语") << QStringLiteral("物理") << QStringLiteral("化学") << QStringLiteral("生物") };
    auto fileListNames{ QStringList() << QStringLiteral("templateList_chinese") << QStringLiteral("templateList_mathematics") << QStringLiteral("templateList_english") << QStringLiteral("templateList_physics") << QStringLiteral("templateList_chemistry") << QStringLiteral("templateList_biography") };

    auto addListWidget{[this](const QString & filePath, const QString & name, bool listAll)
    {
        auto tempListWidget{new QListWidget};
        if(listAll)
        {
            QFile file { filePath };
            if(file.exists())
            {
                file.open(QFile::ReadOnly);
                QString data{ file.readAll() };
                file.close();
                auto tempTemplateList{data.split(QStringLiteral("\n"), Qt::SkipEmptyParts)};
                for (auto i{ 0 }; i < tempTemplateList.size(); ++i)
                {
                    if(i & 1)
                    {
                        templateCodeList.append(tempTemplateList.at(i));
                    }
                    else
                    {
                        templateNameList.append(tempTemplateList.at(i));
                        tempListWidget->addItem(tempTemplateList.at(i));
                    }
                }
            }
        }
        else
        {
            QFile fileAll { filePath };
            if(fileAll.exists())
            {
                fileAll.open(QFile::ReadOnly);
                QString dataAll{ fileAll.readAll() };
                fileAll.close();
                auto tempTemplateListAll{dataAll.split(QStringLiteral("\n"), Qt::SkipEmptyParts)};
                for (auto i{ 0 }; i < tempTemplateListAll.size(); ++i)
                {
                    if(i & 1)
                    {
                        templateCodeList.append(tempTemplateListAll.at(i));
                    }
                    else
                    {
                        templateNameList.append(tempTemplateListAll.at(i));
                    }
                }
            }
            QFile fileCurrent { filePath + QStringLiteral("_current") };
            if(fileCurrent.exists())
            {
                fileCurrent.open(QFile::ReadOnly);
                QString dataCurrent{ fileCurrent.readAll() };
                fileCurrent.close();
                auto tempTemplateListCurrent{dataCurrent.split(QStringLiteral("\n"), Qt::SkipEmptyParts)};
                for (auto i{ 0 }; i < tempTemplateListCurrent.size(); i += 2)
                {
                    tempListWidget->addItem(tempTemplateListCurrent.at(i));
                }
            }
        }
        QScroller::grabGesture(tempListWidget->viewport(), QScroller::TouchGesture);
        tempListWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        tempListWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        tempListWidget->setAutoScroll(false);
        allListWidget->addTab(tempListWidget, name);
        connect(tempListWidget, &QListWidget::itemClicked, this, &SearchWidget::currentItemChanged);
    }};

    for(auto i(0); i < 6; ++i)
    {
#ifdef Q_OS_ANDROID
        const QString filePath { QStringLiteral("assets:/templateList/") + fileListNames.at(i) };
#else
        const QString filePath { QStringLiteral(":/template/templateList/") + fileListNames.at(i) };
#endif
        const QString tabName {subjects.at(i)};
        addListWidget(filePath, tabName, Setting::listAllTemplate);
    }

    addListWidget(Global::tempPath() + QDir::separator() + QStringLiteral("templateList_undefined"), QStringLiteral("undefined"), true);

    auto addHBoxLayoutWithTwoWidget{[](QWidget * widget1, QWidget * widget2)
    {
        auto layout{new QHBoxLayout};
        layout->addWidget(widget1);
        layout->addWidget(widget2);
        return layout;
    }};
    auto addHBoxLayoutWithThreeWidget{[](QWidget * widget1, QWidget * widget2, QWidget * widget3)
    {
        auto layout{new QHBoxLayout};
        layout->addWidget(widget1);
        layout->addWidget(widget2);
        layout->addWidget(widget3);
        return layout;
    }};

    searchLineEdit->setPlaceholderText(QStringLiteral("题卡名称"));
    templateCodeLineEdit->setPlaceholderText(QStringLiteral("题卡编号"));
    listWidget->setVisible(false);
    OKButton->setEnabled(false);
    QScroller::grabGesture(listWidget->viewport(), QScroller::TouchGesture);
    listWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    listWidget->setAutoScroll(false);

    QFont smallFont;
    const auto defaultFontPixelSize{QWidget::font().pixelSize()};
    if(Setting::smallFontPointSize != 0)
    {
        smallFont.setPointSize(Setting::smallFontPointSize);
    }
    else
    {
        smallFont.setPixelSize(defaultFontPixelSize / 2);
    }
    allListWidget->setFont(smallFont);
    listWidget->setFont(smallFont);

    previousPageButton->setVisible(false);
    nextPageButton->setVisible(false);

    mainLayout->addLayout(addHBoxLayoutWithTwoWidget(searchLineEdit, searchButton));
    mainLayout->addLayout(addHBoxLayoutWithThreeWidget(templateCodeLineEdit, scanQRCodeButton, selectTemplateIdButton));
    mainLayout->addWidget(resetButton);
    mainLayout->addWidget(allListWidget);
    mainLayout->addWidget(listWidget);
    mainLayout->addLayout(addHBoxLayoutWithTwoWidget(previousPageButton, nextPageButton));
    mainLayout->addWidget(OKButton);


    connect(OKButton, &QPushButton::clicked, this, &SearchWidget::OKButtonPushed);
    connect(searchButton, &QPushButton::clicked, this, &SearchWidget::searchButtonPushed);
    connect(resetButton, &QPushButton::clicked, this, &SearchWidget::reset);
    connect(listWidget, &QListWidget::itemClicked, this, &SearchWidget::currentItemChanged);
    connect(searchLineEdit, &QLineEdit::returnPressed, this, &SearchWidget::searchButtonPushed);
    connect(previousPageButton, &QPushButton::clicked, this, &SearchWidget::toPreviousPage);
    connect(nextPageButton, &QPushButton::clicked, this, &SearchWidget::toNextPageButton);
    connect(selectTemplateIdButton, &QPushButton::clicked, this, &SearchWidget::selectTemplateIdButtonPushed);
    connect(scanQRCodeButton, &QPushButton::clicked, [this]
    {
        scanQRCodeButton->setEnabled(false);

        QMessageBox msgBox1;
        const auto takePhotoBtn{ msgBox1.addButton(QStringLiteral("拍照"), QMessageBox::AcceptRole) };
        const auto selectFileBtn{ msgBox1.addButton(QStringLiteral("选择文件"), QMessageBox::AcceptRole) };
        msgBox1.exec();

        QByteArray decodeResult;

        if(msgBox1.clickedButton() == takePhotoBtn)
        {
#ifdef Q_OS_ANDROID
            auto image {CallAndroidNativeComponent::takePhoto()};
            if(image.isNull())
            {
                return;
            }
            QMessageBox msgBox2;
            msgBox2.setText(QStringLiteral("解析中..."));
            msgBox2.show();
            decodeResult = QRCodeScanner::scanQRCode(image, "JPEG");
            msgBox2.close();
#endif // Q_OS_ANDROID
        }
        else if(msgBox1.clickedButton() == selectFileBtn)
        {
            const auto imagePath{QFileDialog::getOpenFileName(this, QStringLiteral("选择文件"), QString(), QStringLiteral("Images (*.bmp *.gif *.jpg *.jpeg *.png *.tiff *.pbm *.pgm *.ppm *.xbm *.xpm)"))};
            if(imagePath.isEmpty())
            {
                return;
            }
            QMessageBox msgBox2;
            msgBox2.setText(QStringLiteral("解析中..."));
            msgBox2.show();
            decodeResult = QRCodeScanner::scanQRCode(imagePath, "JPEG");
            msgBox2.close();
        }
        else
        {
            return;
        }

        if(decodeResult.isEmpty())
        {
            QMessageBox::warning(this, QStringLiteral("warning"), QStringLiteral("扫描失败\n"
                                 "请确保二维码清晰可见"));
            scanQRCodeButton->setEnabled(true);
            return;
        }
        qDebug() << decodeResult;
        this->templateCodeLineEdit->setText(decodeResult);
        scanQRCodeButton->setEnabled(true);
    });
}

void SearchWidget::reset()
{
    this->OKButton->setEnabled(false);
    listWidget->setVisible(false);
    allListWidget->setVisible(true);
    allListWidget->setCurrentIndex(0);
    searchLineEdit->clear();
    templateCodeLineEdit->clear();

    for(auto i{0};; ++i)
    {
        const auto tempListWidget{static_cast<QListWidget*>(allListWidget->widget(i))};
        if(tempListWidget == nullptr)
        {
            break;
        }
        const auto currentItem{tempListWidget->currentItem()};
        if(currentItem != nullptr)
        {
            currentItem->setFlags(Qt::ItemIsEnabled);
        }
    }
}

void SearchWidget::toPreviousPage()
{
    if (allListWidget->isVisible())
    {
        static_cast<QListWidget*>(allListWidget->currentWidget())->verticalScrollBar()->setSliderPosition(static_cast<QListWidget*>(allListWidget->currentWidget())->verticalScrollBar()->sliderPosition() - allListWidget->height());
    }
    else if (listWidget->isVisible())
    {
        listWidget->verticalScrollBar()->setSliderPosition(listWidget->verticalScrollBar()->sliderPosition() - allListWidget->height());
    }
}

void SearchWidget::toNextPageButton()
{
    if (allListWidget->isVisible())
    {
        static_cast<QListWidget*>(allListWidget->currentWidget())->verticalScrollBar()->setSliderPosition(static_cast<QListWidget*>(allListWidget->currentWidget())->verticalScrollBar()->sliderPosition() + allListWidget->height());
    }
    else if (listWidget->isVisible())
    {
        listWidget->verticalScrollBar()->setSliderPosition(listWidget->verticalScrollBar()->sliderPosition() + allListWidget->height());
    }
}

void SearchWidget::searchButtonPushed()
{
    auto searchText{searchLineEdit->text().trimmed()};
    if(searchText.isEmpty())
    {
        allListWidget->setVisible(true);
        listWidget->setVisible(false);
    }
    else
    {
        auto list{ templateNameList.filter(searchText, Qt::CaseInsensitive) };
        if (list.isEmpty())
        {
            QMessageBox::information(this, QStringLiteral("information"), QStringLiteral("搜索结果为空"));
            allListWidget->setVisible(true);
            listWidget->setVisible(false);
        }
        else
        {
            listWidget->clear();
            for (auto i{ list.begin() }; i != list.end(); ++i)
            {
                listWidget->addItem(*i);
            }
            allListWidget->setVisible(false);
            listWidget->setVisible(true);
        }
    }
}

void SearchWidget::cancleButtonPushed()
{
    this->hide();
}

void SearchWidget::OKButtonPushed()
{
    this->hide();
    QApplication::processEvents();
    emit searchFinished(this->webRawData, this->templateName, this->templateCode);
}

void SearchWidget::selectTemplateIdButtonPushed()
{
    templateCode = templateCodeLineEdit->text().trimmed();
    if(templateCode.isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("warning"), QStringLiteral("不能为空"));
        return;
    }
#ifdef Q_OS_ANDROID
    QFile file { QStringLiteral("assets:/templateData/") + templateCode };
#else
    QFile file { QStringLiteral(":/template/templateData/") + templateCode };
#endif
    QFile fileTemp { Global::tempPath() + QDir::separator() + QStringLiteral("TemplateFile") + QDir::separator() + templateCode };
    if (file.exists())
    {
        file.open(QFile::ReadOnly);
        webRawData = file.readAll();
        file.close();
    }
    else if (fileTemp.exists())
    {
        fileTemp.open(QFile::ReadOnly);
        webRawData = fileTemp.readAll();
        fileTemp.close();
    }
    else
    {
        try
        {
            webRawData = XinjiaoyuNetwork::getTemplateCodeData(templateCode);
        }
        catch (const std::exception &e)
        {
            QMessageBox::critical(Q_NULLPTR, QStringLiteral("critical"), e.what());
            return;
        }
        if(!webRawData.isEmpty())
        {
            fileTemp.open(QFile::WriteOnly);
            fileTemp.write(webRawData);
            fileTemp.close();
        }
        else
        {
            return;
        }
    }
    templateName = QJsonDocument::fromJson(webRawData).object().value(QStringLiteral("data")).toObject().value(QStringLiteral("templateName")).toString();
    this->searchLineEdit->setText(templateName);
    if(templateCodeList.indexOf(templateCode) == -1)
    {
        const QString data{templateName + QStringLiteral("\n") + templateCode + QStringLiteral("\n")};
        QFile f(Global::tempPath() + QDir::separator() + QStringLiteral("templateList_undefined"));
        f.open(QFile::ReadWrite | QFile::Append);
        f.write(data.toUtf8());
        f.close();
        templateNameList.append(templateName);
        templateCodeList.append(templateCode);
        //NOTE 和总数目相关,undefined在第7个
        auto newItem{new QListWidgetItem(templateName, static_cast<QListWidget*>(allListWidget->widget(6)))};
        static_cast<QListWidget*>(allListWidget->widget(6))->setCurrentItem(newItem);
        allListWidget->setCurrentIndex(6);
    }
    OKButton->setEnabled(true);
}

void SearchWidget::currentItemChanged(QListWidgetItem *item)
{
    const auto currentItemText{ item->text() };
    this->searchLineEdit->setText(currentItemText);
    templateCode = templateCodeList.at(templateNameList.indexOf(currentItemText));
    this->templateCodeLineEdit->setText(templateCode);
    selectTemplateIdButtonPushed();
    OKButton->setEnabled(true);
}
