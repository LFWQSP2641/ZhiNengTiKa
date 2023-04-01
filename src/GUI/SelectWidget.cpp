#include "SelectWidget.h"
#include "../StaticClass/Global.h"
#include "../StaticClass/XinjiaoyuNetwork.h"
#include "../StaticClass/Setting.h"
#include "../StaticClass/CallAndroidNativeComponent.h"
#include "../StaticClass/QRCodeScanner.h"
#include "../GUI/TemplateDetailWidget.h"
#include "../GUI/SearchWidget.h"
#include "../GUI/MultipleSubjectsTemplateListView.h"
#include "../Logic/WebRawData.h"

SelectWidget::SelectWidget(QWidget *parent)
    : QWidget{ parent }
{
    mainLayout = new QVBoxLayout(this);
    multipleSubjectsTemplateListView = new MultipleSubjectsTemplateListView(this);
    OKButton = new QPushButton(QStringLiteral("确定"), this);
    searchButton = new QPushButton(QStringLiteral("搜索"), this);
//    previousPageButton = new QPushButton(QStringLiteral("上一页"), this);
//    nextPageButton = new QPushButton(QStringLiteral("下一页"), this);
    scanQRCodeButton = new QPushButton(QStringLiteral("扫码"), this);
    templateCodeLineEdit = new QLineEdit(this);

    templateCodeLineEdit->setPlaceholderText(QStringLiteral("题卡编号"));
    OKButton->setEnabled(false);

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
    multipleSubjectsTemplateListView->setFont(smallFont);

//    previousPageButton->setVisible(false);
//    nextPageButton->setVisible(false);

    auto addHBoxLayoutWithTwoWidget{[](QWidget * widget1, QWidget * widget2)
    {
        auto layout{new QHBoxLayout};
        layout->addWidget(widget1);
        layout->addWidget(widget2);
        return layout;
    }};
    mainLayout->addLayout(addHBoxLayoutWithTwoWidget(scanQRCodeButton, searchButton));
    mainLayout->addWidget(multipleSubjectsTemplateListView);
//    mainLayout->addLayout(addHBoxLayoutWithTwoWidget(previousPageButton, nextPageButton));
    mainLayout->addWidget(templateCodeLineEdit);
    mainLayout->addWidget(OKButton);


    connect(OKButton, &QPushButton::clicked, this, &SelectWidget::OKButtonPushed);
    connect(searchButton, &QPushButton::clicked, this, &SelectWidget::searchButtonPushed);
//    connect(previousPageButton, &QPushButton::clicked, this, &SelectWidget::toPreviousPage);
//    connect(nextPageButton, &QPushButton::clicked, this, &SelectWidget::toNextPageButton);
    connect(scanQRCodeButton, &QPushButton::clicked, this, &SelectWidget::scanQRCode);
    connect(templateCodeLineEdit, &QLineEdit::textChanged, [this]
    {
        this->OKButton->setEnabled(true);
    });
    connect(this->multipleSubjectsTemplateListView, &MultipleSubjectsTemplateListView::templateNameClicked, [this](const QString & templateCode)
    {
        this->templateCodeLineEdit->setText(templateCode);
    });
}

//void SelectWidget::toPreviousPage()
//{
//    static_cast<QListWidget *>(listWidgetTabWidget->currentWidget())
//    ->verticalScrollBar()
//    ->setSliderPosition(
//        static_cast<QListWidget *>(listWidgetTabWidget->currentWidget())
//        ->verticalScrollBar()
//        ->sliderPosition() -
//        listWidgetTabWidget->height());
//}

//void SelectWidget::toNextPageButton()
//{
//    static_cast<QListWidget *>(listWidgetTabWidget->currentWidget())
//    ->verticalScrollBar()
//    ->setSliderPosition(
//        static_cast<QListWidget *>(listWidgetTabWidget->currentWidget())
//        ->verticalScrollBar()
//        ->sliderPosition() +
//        listWidgetTabWidget->height());
//}

void SelectWidget::searchButtonPushed()
{
    auto searchWidget{new SearchWidget};
    searchWidget->setAttribute(Qt::WA_DeleteOnClose);
    searchWidget->setAttribute(Qt::WA_QuitOnClose, false);
    connect(searchWidget, &SearchWidget::searchFinished, [this, searchWidget](const QString & templateCode)
    {
        this->templateCodeLineEdit->setText(templateCode);
        this->OKButtonPushed();
        searchWidget->close();
    });
    searchWidget->show();
}

void SelectWidget::OKButtonPushed()
{
    const auto analysisWebRawData{getTemplateCodeData(templateCodeLineEdit->text().trimmed())};
    if(analysisWebRawData.isEmpty())
    {
        return;
    }

#ifdef Q_OS_ANDROID
    emit searchFinished(analysisWebRawData);
#else // Q_OS_ANDROID
    auto templateDetailWidget {new TemplateDetailWidget(analysisWebRawData)};
    templateDetailWidget->setAttribute(Qt::WA_DeleteOnClose);
    templateDetailWidget->setAttribute(Qt::WA_QuitOnClose, false);
    templateDetailWidget->show();
#endif // Q_OS_ANDROID
}

AnalysisWebRawData SelectWidget::getTemplateCodeData(const QString &templateCode)
{
    if(templateCode.isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral(""), QStringLiteral("题卡编号不能为空"));
        return AnalysisWebRawData();
    }
    auto appendTemplateList{[&templateCode, this](const QString & templateName)
    {
        const QString data{templateName + QStringLiteral("\n") + templateCode + QStringLiteral("\n")};
        QFile f(Global::tempPath() + QDir::separator() + QStringLiteral("templateList_undefined"));
        f.open(QFile::ReadWrite | QFile::Append);
        f.write(data.toUtf8());
        f.close();
        this->multipleSubjectsTemplateListView->addNewTemplate(QPair<QString, QString>(templateName, templateCode));
    }};
    WebRawData webRawData(templateCode);
    if(!webRawData.getValid())
    {
        QMessageBox::warning(this, QStringLiteral(""), webRawData.getErrorStr());
        return AnalysisWebRawData();
    }
    else if(webRawData.getExternal())
    {
        if (!this->multipleSubjectsTemplateListView
                ->getMultipleSubjectsTemplateListModelList()
                .at(MultipleSubjectsTemplateListModelList::Subjects::Undefined)
                ->hasTemplateCode(templateCode))
        {
            appendTemplateList(webRawData.getTemplateName());
        }
    }
    else if(webRawData.getNetwork())
    {
        appendTemplateList(webRawData.getTemplateName());
    }
    return webRawData.toAnalysisWebRawData();
}

void SelectWidget::scanQRCode()
{
    scanQRCodeButton->setEnabled(false);

    QMessageBox msgBox1;
    msgBox1.setText(QStringLiteral("请选择扫码方式:"));
    const auto takePhotoBtn{ msgBox1.addButton(QStringLiteral("拍照"), QMessageBox::AcceptRole) };
    const auto selectFileBtn{ msgBox1.addButton(QStringLiteral("选择文件"), QMessageBox::AcceptRole) };
    msgBox1.addButton(QStringLiteral("取消"), QMessageBox::RejectRole)->setVisible(false);
    msgBox1.exec();

    QByteArray decodeResult;

    if(msgBox1.clickedButton() == takePhotoBtn)
    {
#ifdef Q_OS_ANDROID
        auto image {CallAndroidNativeComponent::takePhoto()};
        if(image.isNull())
        {
            scanQRCodeButton->setEnabled(true);
            return;
        }
        QMessageBox msgBox2;
        msgBox2.setText(QStringLiteral("解析中..."));
        msgBox2.show();
        try
        {
            decodeResult = QRCodeScanner::scanQRCode(image, "JPEG");
        }
        catch (const std::exception &e)
        {
            QMessageBox::critical(Q_NULLPTR, QStringLiteral("critical"), e.what());
            scanQRCodeButton->setEnabled(true);
            return;
        }

        msgBox2.close();
#endif // Q_OS_ANDROID
    }
    else if(msgBox1.clickedButton() == selectFileBtn)
    {
        const auto imagePath{QFileDialog::getOpenFileName(this, QStringLiteral("选择文件"), QString(), QStringLiteral("Images (*.bmp *.gif *.jpg *.jpeg *.png *.tiff *.pbm *.pgm *.ppm *.xbm *.xpm)"))};
        if(imagePath.isEmpty())
        {
            scanQRCodeButton->setEnabled(true);
            return;
        }
        QMessageBox msgBox2;
        msgBox2.setText(QStringLiteral("解析中..."));
        msgBox2.show();
        try
        {
            decodeResult = QRCodeScanner::scanQRCode(imagePath);
        }
        catch (const std::exception &e)
        {
            QMessageBox::critical(Q_NULLPTR, QStringLiteral("critical"), e.what());
            scanQRCodeButton->setEnabled(true);
            return;
        }
        msgBox2.close();
    }
    else
    {
        scanQRCodeButton->setEnabled(true);
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
    this->OKButtonPushed();
    scanQRCodeButton->setEnabled(true);
}
