#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include "../Logic/TemplateAnalysis.h"
class MultipleSubjectsTemplateListView;

class SelectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectWidget(QWidget *parent = nullptr);

protected:
    QVBoxLayout *mainLayout;
    MultipleSubjectsTemplateListView *multipleSubjectsTemplateListView;
    QPushButton *OKButton;
    QPushButton *searchButton;
    QLineEdit *templateCodeLineEdit;
    QPushButton *scanQRCodeButton;
    TemplateFetcher *fetcher;
    TemplateSummary currentListViewTemplateSummary;
    QMessageBox *obtainTemplateFromNetworkMessageBox = nullptr;

protected slots:
    void onSearchButtonPush();
    void onScanQRCodeButtonPush();
    void onOKButtonPush();
    void showTemplateDetailWidget(const TemplateAnalysis &templateAnalysis);
};

#endif // SEARCHDIALOG_H
