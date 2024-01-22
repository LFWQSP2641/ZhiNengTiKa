#include "SelectWidget.h"
#include "../Logic/TemplateFetcher.h"
#include "../GUI/TemplateDetailWidget.h"
#include "../GUI/SearchWidget.h"
#include "../GUI/MultipleSubjectsTemplateListView.h"
#include "../GUI/QRCodeScannerWidget.h"
#include "LibZXingCpp/ZXingResult.h"

SelectWidget::SelectWidget(QWidget *parent)
    : QWidget{ parent }
{
    mainLayout = new QVBoxLayout(this);
    multipleSubjectsTemplateListView = new MultipleSubjectsTemplateListView(this);
    OKButton = new QPushButton(QStringLiteral("确定"), this);
    searchButton = new QPushButton(QStringLiteral("搜索"), this);
    scanQRCodeButton = new QPushButton(QStringLiteral("扫码"), this);
    templateCodeLineEdit = new QLineEdit(this);
    fetcher = new TemplateFetcher(this);
    obtainTemplateFromNetworkMessageBox = new QMessageBox(this);
    obtainTemplateFromNetworkMessageBox->setText(QStringLiteral("获取中..."));

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
        currentListViewTemplateSummary = TemplateSummary();
        this->OKButton->setEnabled(true);
    });
    connect(this->multipleSubjectsTemplateListView, &MultipleSubjectsTemplateListView::templateNameClicked, [this](const TemplateSummary & templateSummary)
    {
        currentListViewTemplateSummary = templateSummary;
        this->templateCodeLineEdit->setText(templateSummary.getTemplateCode());
    });
    connect(fetcher, &TemplateFetcher::templateAnalysisReady, this, &SelectWidget::showTemplateDetailWidget);
    connect(fetcher, &TemplateFetcher::error, this, [this](const QString & msg)
    {
        obtainTemplateFromNetworkMessageBox->close();
        QMessageBox::warning(this, QStringLiteral("获取题卡信息错误"), msg);
    });
    connect(fetcher, &TemplateFetcher::obtainTemplateFromNetwork, obtainTemplateFromNetworkMessageBox, &QMessageBox::show);
}

void SelectWidget::onSearchButtonPush()
{
    auto searchWidget{new SearchWidget};
    searchWidget->setAttribute(Qt::WA_DeleteOnClose);
    searchWidget->setAttribute(Qt::WA_QuitOnClose, false);
    connect(searchWidget, &SearchWidget::searchFinished, [this, searchWidget](const TemplateSummary & templateSummary)
    {
        searchWidget->close();
        fetcher->handleTemplateRequest(templateSummary);
    });
    searchWidget->show();
}

void SelectWidget::onScanQRCodeButtonPush()
{
    auto scannerWidget{new QRCodeScannerWidget};
    scannerWidget->setAttribute(Qt::WA_DeleteOnClose);
    scannerWidget->setAttribute(Qt::WA_QuitOnClose, false);
    connect(scannerWidget, &QRCodeScannerWidget::scanningFinished, [this, scannerWidget](bool success, const ZXingResult & result)
    {
        if(success)
        {
            scannerWidget->close();
            fetcher->handleTemplateRequestByCode(result.getText());
        }
    });
    scannerWidget->resize(this->size());
    scannerWidget->show();
}

void SelectWidget::onOKButtonPush()
{
    if(currentListViewTemplateSummary.isEmpty())
    {
        fetcher->handleTemplateRequest(currentListViewTemplateSummary);
    }
    else
    {
        fetcher->handleTemplateRequestByCode(templateCodeLineEdit->text().trimmed());
    }
}

void SelectWidget::showTemplateDetailWidget(const TemplateAnalysis &templateAnalysis)
{
    obtainTemplateFromNetworkMessageBox->close();

    this->multipleSubjectsTemplateListView->addNewTemplate(templateAnalysis);

    auto templateDetailWidget{ new TemplateDetailWidget(templateAnalysis) };
    templateDetailWidget->setAttribute(Qt::WA_DeleteOnClose);
    templateDetailWidget->setAttribute(Qt::WA_QuitOnClose, false);
    templateDetailWidget->resize(this->size());
    templateDetailWidget->show();

}
