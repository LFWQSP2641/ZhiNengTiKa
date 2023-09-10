#include "SelectWidget.h"
#include "../StaticClass/Global.h"
#include "../GUI/TemplateDetailWidget.h"
#include "../GUI/SearchWidget.h"
#include "../GUI/MultipleSubjectsTemplateListView.h"
#include "../GUI/QRCodeScannerWidget.h"

SelectWidget::SelectWidget(QWidget *parent)
    : QWidget{ parent }
{
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

    connect(OKButton, &QPushButton::clicked, this, &SelectWidget::onOKButtonPush);
    connect(searchButton, &QPushButton::clicked, this, &SelectWidget::onSearchButtonPush);
    connect(scanQRCodeButton, &QPushButton::clicked, this, &SelectWidget::onScanQRCodeButtonPush);
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

void SelectWidget::onSearchButtonPush()
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

void SelectWidget::onScanQRCodeButtonPush()
{
    auto scannerWidget{new QRCodeScannerWidget};
    scannerWidget->setAttribute(Qt::WA_DeleteOnClose);
    scannerWidget->setAttribute(Qt::WA_QuitOnClose, false);
    connect(scannerWidget, &QRCodeScannerWidget::scanningFinished, [this, scannerWidget](bool success, const QString & result)
    {
        if(success)
        {
            scannerWidget->close();
            const TemplateRawData templateRawData(result);
            if(!templateRawData.isValid())
            {
                QMessageBox::warning(this, QStringLiteral(""), templateRawData.getErrorStr());
                return;
            }
            showTemplateDetailWidget(TemplateAnalysis(templateRawData));
        }
    });
    scannerWidget->show();
}

void SelectWidget::onOKButtonPush()
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
