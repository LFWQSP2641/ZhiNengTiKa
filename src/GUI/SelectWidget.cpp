#include "SelectWidget.h"
#include "../Logic/QRCodeScanner.h"
#include "../StaticClass/Global.h"
#include "../StaticClass/CallAndroidNativeComponent.h"
#include "../GUI/TemplateDetailWidget.h"
#include "../GUI/SearchWidget.h"
#include "../GUI/MultipleSubjectsTemplateListView.h"

SelectWidget::SelectWidget(QWidget *parent)
    : QWidget{ parent }
{
    qrCodeScanner = new QRCodeScanner(this);
    mainLayout = new QVBoxLayout(this);
    multipleSubjectsTemplateListView = new MultipleSubjectsTemplateListView(this);
    OKButton = new QPushButton(QStringLiteral("确定"), this);
    searchButton = new QPushButton(QStringLiteral("搜索"), this);
    scanQRCodeButton = new QPushButton(QStringLiteral("扫码"), this);
    templateCodeLineEdit = new QLineEdit(this);

    templateCodeLineEdit->setPlaceholderText(QStringLiteral("题卡编号"));
    OKButton->setEnabled(false);

    auto addHBoxLayoutWithTwoWidget{[](QWidget * widget1, QWidget * widget2)
    {
        auto layout{new QHBoxLayout};
        layout->addWidget(widget1);
        layout->addWidget(widget2);
        return layout;
    }};
    mainLayout->addLayout(addHBoxLayoutWithTwoWidget(scanQRCodeButton, searchButton));
    mainLayout->addWidget(multipleSubjectsTemplateListView);
    mainLayout->addWidget(templateCodeLineEdit);
    mainLayout->addWidget(OKButton);


    connect(qrCodeScanner, &QRCodeScanner::analysisFinished, [this](bool success, bool exception, const QString & decodeResult)
    {
        if(qrCodeScannerStateMessageBox != nullptr)
        {
            qrCodeScannerStateMessageBox->close();
        }
        if(!success)
        {
            QMessageBox::warning(this, QStringLiteral("解析二维码"), QStringLiteral("扫描失败\n"
                                 "请确保二维码清晰可见").append(decodeResult));
            scanQRCodeButton->setEnabled(true);
            return;
        }
        qDebug() << decodeResult;
        this->templateCodeLineEdit->setText(decodeResult);
        this->OKButtonPush();
        scanQRCodeButton->setEnabled(true);
    });
    connect(qrCodeScanner, &QRCodeScanner::apiInitializing, [this]
    {
        if(qrCodeScannerStateMessageBox != nullptr)
        {
            qrCodeScannerStateMessageBox->setText(QStringLiteral("初始化API参数"));
        }
    });
    connect(qrCodeScanner, &QRCodeScanner::initializeApiFinished, [this](bool success)
    {
        if(qrCodeScannerStateMessageBox != nullptr)
        {
            if(success)
            {
                qrCodeScannerStateMessageBox->setText(QStringLiteral("解析中..."));
            }
            else
            {
                qrCodeScannerStateMessageBox->setText(QStringLiteral("初始化API参数失败"));
            }
        }
    });
    connect(OKButton, &QPushButton::clicked, this, &SelectWidget::OKButtonPush);
    connect(searchButton, &QPushButton::clicked, this, &SelectWidget::searchButtonPush);
    connect(scanQRCodeButton, &QPushButton::clicked, this, &SelectWidget::scanQRCode);
    connect(templateCodeLineEdit, &QLineEdit::textChanged, [this]
    {
        this->OKButton->setEnabled(true);
    });
    connect(this->multipleSubjectsTemplateListView, &MultipleSubjectsTemplateListView::templateNameClicked, [this](const TemplateSummary & templateSummary)
    {
        this->templateCodeLineEdit->setText(templateSummary.getTemplateCode());
    });
}

TemplateAnalysis SelectWidget::getTemplateAnalysis(const QString &templateCode)
{
    if(templateCode.isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral(""), QStringLiteral("题卡编号不能为空"));
        return TemplateAnalysis();
    }
    auto appendTemplateList{[&templateCode, this](const QString & templateName)
    {
        const QString data{QString(templateName).append(QStringLiteral("\n")).append(templateCode).append(QStringLiteral("\n"))};
        QFile f(Global::tempPath().append(QStringLiteral("/")).append(QStringLiteral("templateList_undefined")));
        f.open(QFile::ReadWrite | QFile::Append);
        f.write(data.toUtf8());
        f.close();
        this->multipleSubjectsTemplateListView->addNewTemplate(TemplateSummary(templateName, templateCode));
    }};
    TemplateRawData templateRawData(templateCode);
    if(!templateRawData.isValid())
    {
        QMessageBox::warning(this, QStringLiteral(""), templateRawData.getErrorStr());
        return TemplateAnalysis();
    }
    else if(templateRawData.isExternal())
    {
        if (!this->multipleSubjectsTemplateListView
                ->getMultipleSubjectsTemplateListModelList()
                .at(MultipleSubjectsTemplateListModelList::Subjects::Undefined)
                ->hasTemplateCode(templateCode))
        {
            appendTemplateList(templateRawData.getTemplateName());
        }
    }
    else if(templateRawData.isNetwork())
    {
        appendTemplateList(templateRawData.getTemplateName());
    }
    return TemplateAnalysis(templateRawData);
}

void SelectWidget::searchButtonPush()
{
    auto searchWidget{new SearchWidget};
    searchWidget->setAttribute(Qt::WA_DeleteOnClose);
    searchWidget->setAttribute(Qt::WA_QuitOnClose, false);
    connect(searchWidget, &SearchWidget::searchFinished, [this, searchWidget](const TemplateSummary & templateSummary)
    {
        const TemplateRawData templateRawData(templateSummary);
        searchWidget->close();
        if(!templateRawData.isValid())
        {
            QMessageBox::warning(this, QStringLiteral(""), templateRawData.getErrorStr());
            return;
        }
        showTemplateDetailWidget(TemplateAnalysis(templateRawData));
    });
    searchWidget->show();
}

void SelectWidget::OKButtonPush()
{
    const auto templateCode(templateCodeLineEdit->text().trimmed());
    const auto templateSummary(this->multipleSubjectsTemplateListView->getCurrentTemplateSummary());
    if(templateCode == templateSummary.getTemplateCode())
    {
        const TemplateRawData templateRawData(templateSummary);
        if(!templateRawData.isValid())
        {
            QMessageBox::warning(this, QStringLiteral(""), templateRawData.getErrorStr());
            return;
        }
        showTemplateDetailWidget(TemplateAnalysis(templateRawData));
    }
    else
    {
        showTemplateDetailWidget(getTemplateAnalysis(templateCode));
    }
}

void SelectWidget::showTemplateDetailWidget(const TemplateAnalysis &templateAnalysis)
{
    if(!templateAnalysis.isValid())
    {
        return;
    }
    auto templateDetailWidget{ new TemplateDetailWidget(QSharedPointer<TemplateAnalysis>(new TemplateAnalysis(templateAnalysis))) };
    templateDetailWidget->setAttribute(Qt::WA_DeleteOnClose);
    templateDetailWidget->setAttribute(Qt::WA_QuitOnClose, false);
    templateDetailWidget->resize(this->size());
    templateDetailWidget->show();
}

void SelectWidget::scanQRCode()
{
    scanQRCodeButton->setEnabled(false);

    QMessageBox msgBox1;
    msgBox1.setText(QStringLiteral("请选择扫码方式:"));
//    const auto takePhotoBtn{ msgBox1.addButton(QStringLiteral("拍照"), QMessageBox::AcceptRole) };
    const auto selectFileBtn{ msgBox1.addButton(QStringLiteral("选择文件"), QMessageBox::AcceptRole) };
    msgBox1.addButton(QStringLiteral("取消"), QMessageBox::RejectRole)->setVisible(false);
    msgBox1.exec();

    if(msgBox1.clickedButton() == selectFileBtn)
    {
        const auto imagePath(QFileDialog::getOpenFileName(this, QStringLiteral("选择文件"), QString(), QStringLiteral("Images (*.bmp *.gif *.jpg *.jpeg *.png *.tiff *.pbm *.pgm *.ppm *.xbm *.xpm)")));
        const QImage image{imagePath};
        if(!image.isNull())
        {
            qrCodeScanner->scanQRCode(image);
        }
    }
//    else if(msgBox1.clickedButton() == takePhotoBtn)
//    {
//        decodeResult = QRCodeScanner::scanQRCodeByTakePhoto();
//    }
    else
    {
        scanQRCodeButton->setEnabled(true);
        return;
    }

    qrCodeScannerStateMessageBox = new QMessageBox(this);
//    connect(qrCodeScannerStateMessageBox, &QMessageBox::finished, qrCodeScannerStateMessageBox, &QMessageBox::deleteLater);
    connect(qrCodeScannerStateMessageBox, &QMessageBox::finished, []
    {
        qDebug() << "qrCodeScannerStateMessageBox finished";
    });
    qrCodeScannerStateMessageBox->setWindowTitle(QStringLiteral("扫码二维码"));
    qrCodeScannerStateMessageBox->setText(QStringLiteral("解析中..."));
    qrCodeScannerStateMessageBox->setStandardButtons(QMessageBox::Button::Ok);
    qrCodeScannerStateMessageBox->show();
}
